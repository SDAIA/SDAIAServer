#ifndef MYSQL_CONNECTOR_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libconfig.h>
#include <limits.h>
#include "paths.h"
#include "connectors/mysqllimits.h"
#include "utils/strsplit.h"
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

struct rstate
{
    int cnt;
    MYSQL sql;
};

struct DB_CONN_CFG db_cfg;

int init_mysql_cfg(char *);

static int mysql_request_perform_init(struct http_request *req)
{
    struct rstate *state;

    /* Setup state context. */
    if(req->hdlr_extra == NULL)
    {
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
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    if(mysql_real_connect(&state->sql, db_cfg.server, db_cfg.user,
        db_cfg.password, db_cfg.database, db_cfg.port,
        db_cfg.unix_socket, db_cfg.flags) == NULL)
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }
    else
    {
        req->fsm_state = REQ_STATE_SWITCH;
        kore_log(LOG_NOTICE, "Connected to MySQL database.");
    }

    return(HTTP_STATE_CONTINUE);
}

static int mysql_request_perform_switch(struct http_request *req)
{
    //struct rstate *state = req->hdlr_extra;

    // Associating HTTP methods with SQL methods. (REST / CRUD API)
    switch(req->method)
    {
        case HTTP_METHOD_GET:
            req->fsm_state = REQ_STATE_SELECT;
            break;
        case HTTP_METHOD_POST:
            req->fsm_state = REQ_STATE_INSERT;
            break;
        case HTTP_METHOD_PUT:
            req->fsm_state = REQ_STATE_UPDATE;
            break;
        case HTTP_METHOD_DELETE:
            req->fsm_state = REQ_STATE_DELETE;
            break;
        default:
            req->fsm_state = REQ_STATE_ERROR;
            break;
    }

    return(HTTP_STATE_CONTINUE);
}

static int mysql_request_perform_select(struct http_request *req)
{
    struct rstate *state = req->hdlr_extra;

    char *sql_statement = kore_malloc(sizeof(char) * MAX_LENGTH_QUERY);
    char **path_tokens = str_split(req->path, '/');
    char *table = NULL;
    char *id = NULL;
    char *columns = NULL;
    char *adm = NULL;

    //Building query
    /* *(path_tokens + 0) == API base path
     * *(path_tokens + 1) == Table reference.
     * *(path_tokens + 2) == Id reference.
     * arguments == options
     */
    if(!(*(path_tokens + 1)))
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    table = *(path_tokens + 1);

    if(*(path_tokens + 2))
    {
        id = *(path_tokens + 2);
    }

    http_argument_get_byte("adm", adm);
    if(strcmp(table, "users")){
        if(strcmp(adm, "1"))
        {
            columns = "userid, username, email, create_time, GROUP_CONCAT(groupname SEPARATOR \',\')";
        }
        else
        {
            columns = "userid, username, create_time, GROUP_CONCAT(groupname SEPARATOR \',\')";
        }
    }
    else
    {
        columns = "*";
    }

    strcpy(sql_statement, "SELECT ");
    strcat(sql_statement, columns);
    strcat(sql_statement, " FROM ");
    strcat(sql_statement, table);
    if(strcmp(table, "users"))
    {
        strcat(sql_statement, " INNER JOIN users_has_groups ug ON users.userid = ug.users_userid");
        strcat(sql_statement, " INNER JOIN groups ON groups.groupid = ug.groups_groupid");
        if(id)
        {
            strcat(sql_statement, " WHERE userid = ");
            strcat(sql_statement, id);
        }
    }
    strcat(sql_statement, " ;");

    //Exec query
    if(mysql_query(&state->sql, sql_statement) != 0){
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    kore_mem_free(sql_statement);
    req->fsm_state = REQ_STATE_GETDATA;
    return(HTTP_STATE_CONTINUE);
}

static int mysql_request_perform_getdata(struct http_request *req)
{
    struct rstate *state = req->hdlr_extra;

    MYSQL_RES *result;
    char *json_result;

    result = mysql_store_result(&state->sql);
    json_result = gen_mysql_result(result);

    http_response(req, 200, json_result, (unsigned)strlen(json_result));

    mysql_free_result(result);

    req->fsm_state = REQ_STATE_DONE;

    return(HTTP_STATE_CONTINUE);
}

static int mysql_request_perform_insert(struct http_request *req)
{
    struct rstate *state = req->hdlr_extra;

    char *sql_statement = kore_malloc(sizeof(char) * MAX_LENGTH_QUERY);


}

static int mysql_request_perform_done(struct http_request *req)
{
    struct rstate *state = req->hdlr_extra;

    mysql_close(&state->sql);
    return(HTTP_STATE_COMPLETE);
}

static int mysql_request_perform_error(struct http_request *req)
{
    struct rstate *state = req->hdlr_extra;

    char *err_mysql = gen_api_err_server_mysql(req);

    http_response(req, 500, err_mysql, (unsigned)strlen(err_mysql));

    mysql_close(&state->sql);
    return(HTTP_STATE_COMPLETE);
}

extern struct http_state mysql_states[9];

#define mysql_states_size (sizeof(mysql_states) / sizeof(mysql_states[0]))

#endif // MYSQL_CONNECTOR_H_
