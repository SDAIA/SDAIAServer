#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libconfig.h>
#include "paths.h"

#define MYSQL_CONNECTOR_H_

#define REQ_STATE_INIT 0
#define REQ_STATE_QUERY 1
#define REQ_STATE_DB_WAIT 2
#define REQ_STATE_DB_READ 3
#define REQ_STATE_ERROR 4
#define REQ_STATE_DONE 5

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

int init_mysql_cfg(char *, DB_CONN_CFG *);

static int mysql_request_perform_init(struct http_request *);
static int mysql_request_perform_query(struct http_request *);
static int mysql_request_db_wait(struct http_request *);
static int mysql_request_db_read(struct http_request *);
static int mysql_request_error(struct http_request *);
static int mysql_request_done(struct http_request *);

static struct http_state mysql_states[] = {
	{"REQ_STATE_INIT", mysql_request_perform_init},
	{"REQ_STATE_QUERY", mysql_request_perform_query},
	{"REQ_STATE_DB_WAIT", mysql_request_db_wait},
	{"REQ_STATE_DB_READ", mysql_request_db_read},
	{"REQ_STATE_ERROR", mysql_request_error},
	{"REQ_STATE_DONE", mysql_request_done},
};

#define mysql_states_size (sizeof(mysql_states) / sizeof(mysql_states[0]))

#endif // MYSQL_CONNECTOR_H_
