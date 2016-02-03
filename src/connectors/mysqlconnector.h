#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libconfig.h>
#include "paths.h"

#define MYSQL_CONNECTOR_H_

#define MYSQL_OK 0
#define MYSQL_READ_ERR 1
#define MYSQL_CONNECT_ERR 2

typedef struct
{
    const char *server;
    const char *user;
    const char *password;
    const char *database;
    int32_t port;
    const char *unix_socket;
    long long flags;
} DB_CONN_CFG;

int read_mysql_cfg(char *, DB_CONN_CFG *);
int connect_mysql(MYSQL*, DB_CONN_CFG *);
int close_mysql(MYSQL *);

#endif // MYSQL_CONNECTOR_H_
