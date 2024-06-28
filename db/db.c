#include "lib.h"
#include "lib_db.h"
#include <fcntl.h>   /* flags for open & db_open */
#include <stdarg.h>
#include <errno.h>
#include <sys/uio.h> /* struct iovec */

/*
 * Internal constants, related to .idx file
 * Used on each new record in .idx and data files
 */
#define IDXLEN_SZ  4   /* .idx record size */
                       /* in ASCII symbols */
#define SEP       ':'  /* separator char in .idx record */
#define SPACE     ' '  /* space char */
#define NEWLINE   '\n' /* new line char */

/*
 * Following records required
 * to handle chains in hash-table
 * and record in .idx file
 */
#define PTR_SZ        7 /* field pointer size in chain */
#define PTR_MAX  999999 /* max offset */
                        /* in file = 10**PTR_SZ - 1 */
#define NHASH_DEF   137 /* default hash-table size */
#define FREE_OFF      0 /* head of free-records list */
                        /* in .idx file */
#define HASH_OFF PTR_SZ /* head of hash-table in .idx file */

typedef unsigned long DBHASH;
typedef unsigned long COUNT;  /* unsigned counter */

/*
 * DB internals
 */
typedef struct {
  int     idxfd;  /* .idx file descriptor */
  int     datfd;  /* data file descriptor */
  char  *idxbuf;  /* buffer for index record */
  char  *datbuf;  /* buffer for data record */
  char    *name;  /* db name */
  off_t  idxoff;  /* index record offset */
                  /* key starts at (idxoff + PTR_SZ + IDXLEN_SZ */
  size_t idxlen;  /* index record len */
                  /* apart IDXLEN_SZ bytes at the beginning, */
                  /* includes nl symbol at the end */
  off_t dataoff;  /* data record offset */
  size_t datlen;  /* data record len */
                  /* nl at the end included */
  off_t  ptrval;  /* pointer to .idx chain */
  off_t  ptroff;  /* pointer offset of previous ptrval */
  off_t  chainoff;        /* chain offset for this .idx record */
  off_t  hashoff;         /* hash table offset in .idx file */
  DBHASH nhash;           /* current hash-table size */
  COUNT  cnf_delok;       /* successfull delete operations counter */
  COUNT  cnt_delerr;      /* erroneous delete operations counter */
  COUNT  cnt_fetchok;     /* successfull "get" operations counter */
  COUNT  cnt_fetcherr;    /* erroneous "get" operations counter */
  COUNT  cnt_nextrec;     /* nextrec */
  COUNT  cnt_stor1;       /* store: DB_INSERT, no empty records, insert */
  COUNT  cnt_stor2;       /* store: DB_INSERT, has empty records */
                          /* use them */
  COUNT  cnt_stor3;       /* store: DB_REPLACE, different length, insert */
  COUNT  cnt_store4;      /* store: DB_REPLACE, same length, rewrite */
  COUNT  cnt_storeerr;    /* erroneous inserts counter */
} DB;

/*
 * Internal functions
 */
static DB        *_db_alloc(int);
static void       _db_dodelete(DB *);
static int        _db_find_and_lock(DB *, const char *, int);
static int        _db_findfree(DB *, int, int); 
static void       _db_free(DB *);
static DBHASH     _db_hash(DB *, const char *);
static char      *_db_readdat(DB *);
static off_t      _db_readidx(DB *, off_t);
static off_t      _db_readptr(DB *, off_t);
static void       _db_writedat(DB *, const char *, off_t, int);
static void       _db_writeidx(DB *, const char *, off_t, int, off_t);
static void       _db_writeptr(DB *, off_t, off_t);

/*
 * Opens and creates db. Args are similar to open(2).
 */
DBHANDLE
db_open(const char *pathname, int oflag, ...)
{
  DB    *db;
  int    len, mode;
  size_t i;
  char   asciiptr[PTR_SZ + 1],
         hash[(NHASH_DEF + 1) * PTR_SZ + 2];
            /* +2 for '\n' and nl */
  struct stat statbuff;

  /*
   * Allocate DB struct in memory and all the neccessary buffers
   */
  len = strlen(pathname);
  if ((db = _db_alloc(len)) == NULL)
    err_dump("db_open: failed to allocate DB");

  db->nhash   = NHASH_DEF;   /* size of hashes */
  db->hashoff = HASH_OFF;    /* start of hash-table in index file */
  strcpy(db->name, pathname);
  strcat(db->name, ".idx");

  if (oflag & O_CREAT) {
    va_list ap;

    va_start(ap, oflag);
    mode = va_arg(ap, int);
    va_end(ap);

    /*
     * Create .idx and .dat files
     */
    db->idxfd = open(db->name, oflag, mode);
    strcpy(db->name + len, ".dat");
    db->datfd = open(db->name, oflag, mode);
  } else {
    /*
     * Open .idx file and .dat file
     */
    db->idxfd = open(db->name, oflag);
    strcpy(db->name + len, ".dat");
    db->datfd = open(db->name, oflag);
  }

  if (db->idxfd < 0 || db->datfd < 0) {
    _db_free(db);
    return(NULL);
  }

  if ((oflag & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC)) {
    /*
     * Init new db.
     * full lock provides atomicity
     */
    if (writew_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
      err_dump("db_open: failed to call writew_lock");

    if (fstat(db->idxfd, &statbuff) < 0)
      err_sys("db_open: failed to call fstat");

    if (statbuff.st_size == 0) {
      /*
       * Create list of (NHASH_DEF + 1) pointers to chains with nil-values.
       * In such case +1 - is a space for the pointer on free list
       * before the table
       */
      sprintf(asciiptr, "%*d", PTR_SZ, 0);
      hash[0] = 0;
      for (i = 0; i < NHASH_DEF + 1; i++)
        strcat(hash, asciiptr);
      strcat(hash, "\n");
      i = strlen(hash);
      if (write(db->idxfd, hash, i) != i)
        err_dump("db_open: failed to init .idx file");
    }
    if (un_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
      err_dump("db_open: failed to call un_lock");
  }
  db_rewind(db);
  return(db);
}

/*
 * Allocate DB in memory and its buffers
 */
static DB *
_db_alloc(int namelen)
{
  DB      *db;

  /*
   * Calloc func allocates memory and fills it with zeros
   */

  if ((db = calloc(1, sizeof(DB))) == NULL)
    err_dump("_db_alloc: failed to allocate DB struct")
  db->idxfd = db->datfd = -1 /* descriptors */

  /*
   * Allocate space for name
   * +5 for ".idx" and ".dat" and '\n' in the end
   */
  if ((db->name = malloc(namelen + 5)) == NULL)
    err_dump("_db_alloc: failed to allocate memory for name")

  /*
   * Allocate memory for index buffer and data buffer
   * +2 for newline symbol and '\n' in the end of string.
   */
  if ((db->idxbuf = malloc(IDXLEN_MAX + 2)) == NULL)
    err_dump("_db_alloc: failed to allocate index buffer")
  if ((db->datbuf = malloc(DATLEN_MAX + 2)) == NULL)
    err_dump("_db_alloc: failed to allocate data buffer")
  return(db);
}

/*
 * Close db connection
 */
void
db_close(DBHANDLE h)
{
  _db_free((DB *)h); /* close fds, free mem */
}

/*
 * Free memory, allocated for DB and bufs
 * Close also file ds
 */
static void
_db_free(DB *db)
{
  if (db->idxfd >= 0)
    close(db->idxfd);
  if (db->datfd >= 0)
    close(db->datfd);

  if (db->idxbuf != NULL)
    free(db->idxbuf);
  if (db->datbuf != NULL)
    free(db->datbuf);
  if (db->name != NULL)
    free(db->name);
  free(db);
}

/*
 * Get one record. Return pointer to data string
 */
char *
db_fetch(DBHANDLE h, const char *key)
{
  DB    *db = h;
  char  *ptr;

  if (_db_find_and_lock(db, key, 0) < 0) {
    ptr = NULL;  /* error, record not found */
    db->cnt_fetcherr++;
  } else {
    ptr = _db_readdat(db); /* return a pointer to data string */
    db->cnt_fetchok++;
  }

  /*
   * Remove block from chain, set on _db_find_and_lock
   */
  if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
    err_dump("db_fetch: failed to call un_lock");
  return(ptr);
}

/*
 * Find given record. Call from db_delete, db_fetch
 * and db_store. Set block on hash-table chain (bucket)
 */
static int
_db_find_and_lock(DB *db, const char *key, int writelock)
{
  off_t     offset, nextoffset;

  /*
   * Find hash value for given key and find offset
   * of this chain in hash-table.
   * Here we start a search. But fefore that we need 
   * to find a hash offset for given key.
   */
  db->chainoff = (_db_hash(db, key) * PTR_SZ) + db->hashoff;
  db->ptroff = db->chainoff;

  /*
   * Set a block here. Calling func should unlock later.
   * Attention: block is set on first byte only
   */
  if (writelock) {
    if (writew_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
      err_dump("_db_find_and_lock: failed to call writew_lock");
  } else {
    if (readw_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
      err_dump("_db_find_and_lock: failed to call readw_lock");
  }

  /*
   * Get offset of first record in this chain from start
   * of index file (may be 0)
   */
  offset = _db_readptr(db, db->ptroff);

  while (offset != 0) {
    nextoffset = _db_readidx(db, offset);
    if (strcmp(db->idxbuf, key) == 0)
      break;    /* found */
    db->ptroff = offset; /* record offset */
    offset = nextoffset; /* go to next record */
  }
  /*
   * offset == 0 is an error (no record)
   */
  return(offset == 0 ? -1 : 0);
}

/*
 * Computes hash by key
 */
static DBHASH
_db_hash(DB *db, const char *key)
{
  DBHASH hval = 0;
  char   c;
  int    i;

  for (i = 1; (c = *key++) != 0; i++)
    hval += c * i; /* ASCII-code * symbol idx */
  return(hval % db->nhash);
}

/*
 * Reads pointer on chain from .index file:
 *  pointer on a list of free records, on a chain from hash-table
 *  or on index record in chain
 */
static off_t
_db_readptr(DB *db, off_t offset)
{
  char asciiptr[PTR_SZ + 1];

  if (lseek(db->idxfd, offset, SEEK_SET) == 1)
    err_dump("_db_readptr: failed to jump on field with pointer");
  if (read(db->idxfd, asciiptr, PTR_SZ) != PTR_SZ)
    err_dump("_db_readptr: failed to read field with pointer");
  asciiptr[PTR_SZ] = 0; /* last '\0' symbol */
  return(atol(asciiptr));
}

/*
 * Reads next index record, starting from offset from index file.
 * Index record writes into buffer db->idxbuf,
 * delimitin chars offset with '\0' byte.
 * If all is ok, offset is written in db->dataoff and db->datalen 
 * and record length from data file
 */
static off_t
_db_readidx(DB *db, off_t offset)
{
  ssize_t         i;
  char           *ptr1, *ptr2;
  char            asciiptr[PTR_SZ + 1], asciilen[IDXLEN_SZ + 1];
  struct iovec    iov[2];

  /*
   * Position in file and record offset. db_nextrec calls
   * this func with offset==0, it means read from previous position.
   * All in all, we should call lseek to read a record.
   */
  if ((db->idxoff = lseek(db->idxfd, offset,
    offset == 0 ? SEEK_CUR : SEEK_SET)) == -1)
    err_dump("_db_readidx: failed to call lseek");

  /*
   * Read record length and pointer on next record
   * from start of next idx record. It helps us
   * read remaining idx record
   */
  iov[0].iov_base = asciiptr;
  iov[0].iov_len  = PTR_SZ;
  iov[1].iov_base = asciilen;
  iov[1].iov_len  = IDXLEN_SZ;
  if ((i = readv(db->idxfd, &iov[0], 2)) != PTR_SZ + IDXLEN_SZ) {
    if (i == 0 && offset == 0)
      return(-1); /* end of file for db_nextrec */
    err_dump("_db_readidx: failed to call readv to read index record");
  }

  /*
   * Return value, always >= 0
   */
  asciiptr[PTR_SZ] = 0;         /* remaining null-symbol */
  db->ptrval = atol(asciiptr);  /* next-record offset in chain */

  asciilen[IDXLEN_SZ] = 0;      /* last null-symbol */
  if ((db->idxlen = atoi(asciilen)) < IDXLEN_MIN ||
    db->idxlen > IDXLEN_MAX(
    err_dump("_db_readidx: wrong record length");

  /*
   * Read record itself. Read it in index buffer,
   * which was allocated on db open
   */
  if ((i = read(db->idxfd, db->idxbuf, db->idxlen)) != db->idxlen)
    err_dump("_db_readidx: failed to read idx record");
  if (db->idxbuf[db->idxlen1] != NEWLINE) /* integrity check */
    err_dump("_db_readidx: no new-line symbol");
  db->idxbuf[db->idxlen-1] = 0;           /* replace NL on '\0' symbol */

  /*
   * Find separator symbols in index record
   */
  if ((ptr1 = strchr(db->idxbuf, SEP)) == NULL)
    err_dump("_db_readidx: no first separator");
  *ptr1++ = 0; /* replace SEP with '\0' symbol */

  if ((ptr2 = strchr(ptr2, SEP)) == NULL)
    err_dump("_db_readidx: no second separator");
  *ptr2++ = 0; /* replace SEP with '\0' symbol */

  if (strchr(ptr2, SEP) != NULL)
    err_dump("_db_readidx: too many separator symbols");

  /*
   * Get offset and record length
   */
  if ((db->dataoff = atol(ptr1)) < 0)
    err_dump("_db_readidx: offset with data < 0");
  if ((db->datalen = atol(ptr2)) <= 0 || db->datalen > DATALEN_MAX)
    err_dump("_db_readidx: wrong data len with data");
  return(db->ptrval);  /* return next record position */
}

/* ... not finished */
