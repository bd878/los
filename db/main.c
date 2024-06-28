#include "lib.h"
#include "lib_db.h"
#include <fcntl.h>

int
main(void)
{
  DBHANDLE db;

  if ((db = db_open("db4", O_RDWR | O_CREAT | O_TRUNC,
                    FILE_MODE)) == NULL)
    err_sys("failed to call db_open");

  if (db_store(db, "Alpha", "data1", DB_INSERT) != 0)
    err_quit("db_store returned error on first record");
  if (db_store(db, "beta", "Data for beta", DB_INSERT) != 0)
    err_quit("db_store returned error on second record");
  if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
    err_quit("db_store returned error on third record");

  db_close(db);
  exit(0);
}
