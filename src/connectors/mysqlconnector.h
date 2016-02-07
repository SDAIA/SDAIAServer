#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libconfig.h>
#include <limits.h>
#include "paths.h"

#define MYSQL_CONNECTOR_H_

/* Max length of user, password and database name in MySQL.
 * Before 5.7.8 -> 16
 * After 5.7.8 -> 32
 */
#define MAX_LENGTH_USER 16
#define MAX_LENGTH_PASS 16
#define MAX_LENGTH_DB_NAME 64

#define REQ_STATE_INIT 0
#define REQ_STATE_QUERY 1
#define REQ_STATE_DB_WAIT 2
#define REQ_STATE_DB_READ 3
#define REQ_STATE_ERROR 4
#define REQ_STATE_DONE 5

#define MYSQL_OK 0
#define MYSQL_READ_ERR 1
#define MYSQL_CFG_ERR 2
#define MYSQL_CONNECT_ERR 3

typedef struct
{
    char *server;
    char *user;
    char *password;
    char *database;
    int32_t port;
    char *unix_socket;
    long long flags;
} DB_CONN_CFG;

DB_CONN_CFG db_cfg;

struct rstate
{
    int cnt;
    MYSQL sql;
};

int init_mysql_cfg(char *);

static int mysql_request_perform_init(struct http_request *req)
{
    struct rstate *state;

    /* Setup state context. */
    if(req->hdlr_extra == NULL) {
        state = kore_malloc(sizeof(*state));
        req->hdlr_extra = state;
    }
    else
    {
        state = req->hdlr_extra;
    }

    kore_log(LOG_DEBUG, "Server: %s\nUser: %s\nDatabase: %s\nPort: %d\nSocket: %s\n",
        db_cfg.server, db_cfg.user, db_cfg.database, db_cfg.port, db_cfg.unix_socket);

    /* Initialize MySQL */
    memcpy(&state->sql, mysql_init(&state->sql), sizeof(MYSQL));
    if(&state->sql == NULL)
    {
        kore_log(LOG_ERR, "%s\n", mysql_error(&state->sql));
    }
    if(!mysql_real_connect(&state->sql, db_cfg.server, db_cfg.user,
        db_cfg.password, db_cfg.database, db_cfg.port,
        db_cfg.unix_socket, db_cfg.flags))
    {
        kore_log(LOG_ERR, "%s\n", mysql_error(&state->sql));
        mysql_close(&state->sql);
        req->fsm_state = REQ_STATE_ERROR;
    }
    else
    {
        kore_log(LOG_NOTICE, "Connected to MySQL database.");
        req->fsm_state = REQ_STATE_QUERY;
    }

    return(HTTP_STATE_CONTINUE);
}

static int mysql_request_perform_query(struct http_request *req)
{
    return(MYSQL_OK);
}

static int mysql_request_db_wait(struct http_request *req)
{
    return(MYSQL_OK);
}

static int mysql_request_db_read(struct http_request *req)
{
    return(MYSQL_OK);
}

static int mysql_request_error(struct http_request *req)
{
    return(MYSQL_OK);
}

static int mysql_request_done(struct http_request *req)
{
    return(MYSQL_OK);
}

extern struct http_state mysql_states[6];

#define mysql_states_size (sizeof(mysql_states) / sizeof(mysql_states[0]))

#endif // MYSQL_CONNECTOR_H_
