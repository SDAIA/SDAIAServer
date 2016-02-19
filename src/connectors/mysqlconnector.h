#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <my_config.h>
#include <mysql.h>
#include <libconfig.h>
#include <limits.h>
#include <regex.h>
#include "paths.h"
#include "connectors/mysqllimits.h"
#include "utils/jsonmessages.h"

#define MYSQL_CONNECTOR_H_

#define REQ_STATE_INIT 0
#define REQ_STATE_SWITCH 1
#define REQ_STATE_SELECT 2
#define REQ_STATE_GETDATA 3
#define REQ_STATE_INSERT 4
#define REQ_STATE_UPDATE 5
#define REQ_STATE_DELETE 6
#define REQ_STATE_DONE 7
#define REQ_STATE_ERROR 8

#define MYSQL_READ_OK 0
#define MYSQL_READ_ERR 1
#define MYSQL_CFG_ERR 2

struct DB_CONN_CFG
{
    char *server;
    char *user;
    char *password;
    char *database;
    int32_t port;
    char *unix_socket;
    long long flags;
};

struct DB_CONN_CFG db_cfg;

int init_mysql_cfg(char *);

int mysql_request_perform_init(struct http_request *);
int mysql_request_perform_switch(struct http_request *);
int mysql_request_perform_select(struct http_request *);
int mysql_request_perform_getdata(struct http_request *);
int mysql_request_perform_insert(struct http_request *);
int mysql_request_perform_update(struct http_request *);
int mysql_request_perform_delete(struct http_request *);
int mysql_request_perform_done(struct http_request *);
int mysql_request_perform_error(struct http_request *);

extern struct http_state mysql_states[9];

#define mysql_states_size (sizeof(mysql_states) / sizeof(mysql_states[0]))

#endif // MYSQL_CONNECTOR_H_
