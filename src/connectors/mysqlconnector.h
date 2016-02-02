#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <libconfig.h>
#include "paths.h"

#define MYSQL_CONNECTOR_H_

#define MYSQL_CFG_READ_OK 0
#define MYSQL_CFG_READ_ERR 1
#define MYSQL_CONNECTION_OK 0
#define MYSQL_CONNECTION_ERR 2


typedef struct
{
    const char *server;
    const char *user;
    const char *password;
    const char *database;
    unsigned int port;
    const char *unix_socket;
    unsigned long flags;
} DB_CONN_CFG;

MYSQL * my_conn;
MYSQL_RES * my_res;
MYSQL_ROW my_row;
DB_CONN_CFG my_db_cfg = {NULL, NULL, NULL, NULL, 0, NULL, 0};

int read_mysql_cfg(char *, DB_CONN_CFG);
int connect_mysql(MYSQL *, DB_CONN_CFG);
int close_mysql(MYSQL *);

#endif // MYSQL_CONNECTOR_H_
