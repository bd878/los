#ifndef _LIB_DB_H
#define _LIB_DB_H

typedef void *DBHANDLE;

DBHANDLE     db_open(const char *, int, ...);
void         db_close(DBHANDLE);
char        *db_fetch(DBHANDLE, const char *);
int          db_store(DBHANDLE, const char *, const char *, int);
int          db_delete(DBHANDLE, const char *);
void         db_rewind(DBHANDLE);
char        *db_nextrec(DBHANDLE, char *);

/*
 * Flags for db_store() func
 */
#define DB_INSERT  1 /* insert new record */
#define DB_REPLACE 2 /* replace existing record */
#define DB_STORE   3 /* replace or insert new record */

/*
 * Implementation constraints
 */
#define IDXLEN_MIN    6 /* key, separator, offset, separator, length, \n */
#define IDXLEN_MAX 1024 /* randomly selected */
#define DATLEN_MIN    2 /* bytes, new line */
#define DATLEN_MAX 1024 /* randomly selected */

#endif
