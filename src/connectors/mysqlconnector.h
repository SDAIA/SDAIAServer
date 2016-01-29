#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <err.h>
#include <mysql/mysql.h>

#define MYSQL_CONNECTOR_H_

#define MYSQL_CFG_READ_OK 0
#define MYSQL_CFG_READ_ERROR 1

typedef struct
{
    char *server;
    char *user;
    char *password;
    char *database;
} DB_CONN_CFG;

MYSQL * my_conn;
MYSQL_RES * my_res;
MYSQL_ROW my_row;
DB_CONN_CFG my_db_cfg;

int read_mysql_cfg(char *, DB_CONN_CFG);
int connect_mysql(MYSQL *, DB_CONN_CFG);
int close_mysql(MYSQL *);

#endif // MYSQL_CONNECTOR_H_
