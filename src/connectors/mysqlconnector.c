#include "mysqlconnector.h"

struct http_state mysql_states[] = {
    {"REQ_STATE_INIT", mysql_request_perform_init},
    {"REQ_STATE_SWITCH", mysql_request_perform_switch},
    {"REQ_STATE_SELECT", mysql_request_perform_select},
    {"REQ_STATE_GETDATA", mysql_request_perform_getdata},
    {"REQ_STATE_INSERT", mysql_request_perform_insert},
    {"REQ_STATE_UPDATE", mysql_request_perform_update},
    {"REQ_STATE_DELETE", mysql_request_perform_delete},
    {"REQ_STATE_DONE", mysql_request_perform_done},
    {"REQ_STATE_ERROR", mysql_request_perform_error}
};

int init_mysql_cfg(char * cfg_file_name)
{
    config_t cfg;
    const char *server, *user, *password, *database, *unix_socket;
    int port_cfg_ret, sock_cfg_ret;

    config_init(&cfg);

    if(!config_read_file(&cfg, cfg_file_name))
    {
        kore_log(LOG_ERR, "%s:%d - %s\n",
            config_error_file(&cfg),
            config_error_line(&cfg),
            config_error_text(&cfg));
        config_destroy(&cfg);
        return(MYSQL_READ_ERR);
    }

    if(config_lookup_string(&cfg, "server", &server) &&
                config_lookup_string(&cfg, "user", &user) &&
                config_lookup_string(&cfg, "password", &password) &&
                config_lookup_string(&cfg, "database", &database))
        {
                db_cfg.server = kore_malloc(sizeof(char) * HOST_NAME_MAX);
                db_cfg.user = kore_malloc(sizeof(char) * MAX_LENGTH_USER);
                db_cfg.password = kore_malloc(sizeof(char) * MAX_LENGTH_PASS);
                db_cfg.database = kore_malloc(sizeof(char) * MAX_LENGTH_DB_NAME);
                strcpy(db_cfg.server, server);
                strcpy(db_cfg.user, user);
                strcpy(db_cfg.password, password);
                strcpy(db_cfg.database, database);
        }
        else
        {
                kore_log(LOG_ERR, "In init_mysql_cfg: Lack of required configuration elements.");
        config_destroy(&cfg);
        return(MYSQL_CFG_ERR);
    }

    port_cfg_ret = config_lookup_int(&cfg, "port", &db_cfg.port);
    sock_cfg_ret = config_lookup_string(&cfg, "unix_socket", &unix_socket);

    if(!port_cfg_ret && !sock_cfg_ret)
    {
            kore_log(LOG_ERR, "In init_mysql_cfg: Lack of port or unix_socket configuration elements.");
            config_destroy(&cfg);
            return(MYSQL_CFG_ERR);
    }

    if(sock_cfg_ret)
    {
            db_cfg.unix_socket = kore_malloc(sizeof(char) * PATH_MAX);
        strcpy(db_cfg.unix_socket, unix_socket);
    }
    else
    {
                db_cfg.unix_socket = NULL;
        }

        if(!port_cfg_ret)
        {
                db_cfg.port = 0;
        }

    if(!config_lookup_int64(&cfg, "flags", &db_cfg.flags))
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: Variable flags is not defined in conf.");
        db_cfg.flags = 0;
    }

    kore_log(LOG_DEBUG, "Server: %s\nUser: %s\nDatabase: %s\nPort: %d\nSocket: %s\n",
        db_cfg.server, db_cfg.user, db_cfg.database, db_cfg.port, db_cfg.unix_socket);

    config_destroy(&cfg);
    return(MYSQL_READ_OK);
}

int mysql_request_perform_init(struct http_request *req)
{
    MYSQL *state;

    /* Setup state context. */
    if(req->hdlr_extra == NULL)
    {
        state = kore_malloc(sizeof(MYSQL));
        req->hdlr_extra = state;
    }
    else
    {
        state = req->hdlr_extra;
    }

    kore_log(LOG_DEBUG, "Server: %s\nUser: %s\nDatabase: %s\nPort: %d\nSocket: %s\n",
        db_cfg.server, db_cfg.user, db_cfg.database, db_cfg.port, db_cfg.unix_socket);

    /* Initialize MySQL */
    state = mysql_init(state);
    if(state == NULL)
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    if(mysql_real_connect(state, db_cfg.server, db_cfg.user,
        db_cfg.password, db_cfg.database, db_cfg.port,
        db_cfg.unix_socket, db_cfg.flags) == NULL)
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }
    else
    {
        kore_log(LOG_NOTICE, "Connected to MySQL database.");
        req->fsm_state = REQ_STATE_SWITCH;
    }

    return(HTTP_STATE_CONTINUE);
}

int mysql_request_perform_switch(struct http_request *req)
{
    //struct rstate *state = req->hdlr_extra;

    // Associating HTTP methods with SQL methods. (REST / CRUD API)
    kore_log(LOG_NOTICE, "Translation REST -> SQL");
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

int mysql_request_perform_select(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    char *sql_statement = kore_malloc(sizeof(char) * MAX_LENGTH_QUERY);
    char *path = kore_malloc(sizeof(char) * PATH_MAX);
    char *path_tokens[3];
    int selector = -1;
    int adm = 0;
    int tokens = 0;

    //Copy req->path to a temp var.
    strcpy(path, req->path);

    kore_log(LOG_DEBUG, "Building query.");
    kore_log(LOG_DEBUG, "%s", path);
    tokens = kore_split_string(path, "/", path_tokens, 3);
    kore_log(LOG_DEBUG, "Path splitted.");
    //Building query
    /* path_tokens[0] == API base path
     * path_tokens[1] == Table reference.
     * path_tokens[2] == Id reference.
     * arguments == options
     */

    if(tokens < 2)
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    kore_log(LOG_DEBUG, "Checking arguments.");
    http_argument_get_int16(req,"adm", &adm);

    if(strcmp(path_tokens[1], "users") == 0)
    {
        selector = USERS_TABLE;
    }
    else if(strcmp(path_tokens[1], "groups") == 0)
    {
        selector = GROUPS_TABLE;
    }
    else if(strcmp(path_tokens[1], "permissions") == 0)
    {
        selector = PERMISSIONS_TABLE;
    }
    else if(strcmp(path_tokens[1], "applications") == 0)
    {
        selector = APPLICATIONS_TABLE;
    }
    else
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    strcpy(sql_statement, "SELECT ");
    if(adm == 1)
    {
        strcat(sql_statement, priv_cols[selector]);
    }
    else
    {
        strcat(sql_statement, pub_cols[selector]);
    }
    strcat(sql_statement, " FROM ");
    strcat(sql_statement, path_tokens[1]);
    strcat(sql_statement, select_ext[selector]);
    if(tokens == 3)
    {
        strcat(sql_statement, id_str[selector]);
        strcat(sql_statement, path_tokens[2]);
    }
    strcat(sql_statement, " ;");

    //Exec query
    kore_log(LOG_DEBUG, "%s", sql_statement);
    kore_log(LOG_NOTICE, "Executing select query...");
    if(mysql_query(state, sql_statement) != 0){
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    kore_mem_free(sql_statement);
    req->fsm_state = REQ_STATE_GETDATA;
    return(HTTP_STATE_CONTINUE);
}

int mysql_request_perform_getdata(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    MYSQL_RES *result;
    char *json_result;

    result = mysql_store_result(state);
    json_result = gen_mysql_result(result);

    http_response(req, 200, json_result, (unsigned)strlen(json_result));

    mysql_free_result(result);

    req->fsm_state = REQ_STATE_DONE;

    return(HTTP_STATE_CONTINUE);
}

int mysql_request_perform_insert(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    char *sql_statement = kore_malloc(sizeof(char) * MAX_LENGTH_QUERY);
    char **path_tokens = NULL;
    char *table = NULL;
    int selector;

    kore_log(LOG_DEBUG, "Building query.");
    kore_split_string(req->path, "/", path_tokens, 3);
    //Building query
    /* *(path_tokens + 0) == API base path
     * *(path_tokens + 1) == Table reference.
     */

    if(!(*(path_tokens + 1)))
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    table = *(path_tokens + 1);

    if(strcmp(table, "users"))
    {
        selector = USERS_TABLE;
    }
    else if(strcmp(table, "groups"))
    {
        selector = GROUPS_TABLE;
    }
    else if(strcmp(table, "permissions"))
    {
        selector = PERMISSIONS_TABLE;
    }
    else if(strcmp(table, "applications"))
    {
        selector = APPLICATIONS_TABLE;
    }
    else
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    strcpy(sql_statement, "INSERT INTO ");
    strcat(sql_statement, table);
    strcat(sql_statement, "(");
    strcat(sql_statement, pub_cols[selector]);
    strcat(sql_statement, ") VALUES(");
    //TODO: Get values from http_body.
    strcat(sql_statement, ");");

    //Exec query
    kore_log(LOG_DEBUG, "%s", sql_statement);
    kore_log(LOG_NOTICE, "Executing insert query.");
    if(mysql_query(state, sql_statement) != 0){
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    kore_mem_free(sql_statement);
    req->fsm_state = REQ_STATE_DONE;
    return(HTTP_STATE_CONTINUE);

}

int mysql_request_perform_update(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    char *sql_statement = kore_malloc(sizeof(char) * MAX_LENGTH_QUERY);
    char **path_tokens = NULL;
    char *table = NULL;
    char *id = NULL;
    int selector;

    kore_split_string(req->path, "/", path_tokens, 3);
    //Building query
    /* *(path_tokens + 0) == API base path
     * *(path_tokens + 1) == Table reference.
     */

    if(!(*(path_tokens + 1)))
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    table = *(path_tokens + 1);

    if(!(*(path_tokens + 2)))
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    id = *(path_tokens + 2);

    if(strcmp(table, "users"))
    {
        selector = USERS_TABLE;
    }
    else if(strcmp(table, "groups"))
    {
        selector = GROUPS_TABLE;
    }
    else if(strcmp(table, "permissions"))
    {
        selector = PERMISSIONS_TABLE;
    }
    else if(strcmp(table, "applications"))
    {
        selector = APPLICATIONS_TABLE;
    }
    else
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    strcpy(sql_statement, "UPDATE ");
    strcat(sql_statement, table);
    strcat(sql_statement, " SET ");
    //TODO: Get values from http_body.
    if(id)
    {
        strcat(sql_statement, id_str[selector]);
        strcat(sql_statement, id);
    }
    strcat(sql_statement, ";");

    //Exec query
    kore_log(LOG_NOTICE, "Executing update query...");
    if(mysql_query(state, sql_statement) != 0){
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    kore_mem_free(sql_statement);
    req->fsm_state = REQ_STATE_DONE;
    return(HTTP_STATE_CONTINUE);
}

int mysql_request_perform_delete(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    char *sql_statement = kore_malloc(sizeof(char) * MAX_LENGTH_QUERY);
    char **path_tokens = NULL;
    char *table = NULL;
    char *id = NULL;
    int selector;

    kore_split_string(req->path, "/", path_tokens, 3);
    //Building query
    /* *(path_tokens + 0) == API base path
     * *(path_tokens + 1) == Table reference.
     */

    if(!(*(path_tokens + 1)))
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    table = *(path_tokens + 1);

    if(!(*(path_tokens + 2)))
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    id = *(path_tokens + 2);

    if(strcmp(table, "users"))
    {
        selector = USERS_TABLE;
    }
    else if(strcmp(table, "groups"))
    {
        selector = GROUPS_TABLE;
    }
    else if(strcmp(table, "permissions"))
    {
        selector = PERMISSIONS_TABLE;
    }
    else if(strcmp(table, "applications"))
    {
        selector = APPLICATIONS_TABLE;
    }
    else
    {
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    strcpy(sql_statement, "DELETE FROM ");
    strcat(sql_statement, table);
    if(id)
    {
        strcat(sql_statement, id_str[selector]);
        strcat(sql_statement, id);
    }
    strcat(sql_statement, ";");

    //Exec query
    kore_log(LOG_NOTICE, "Executing delete query...");
    if(mysql_query(state, sql_statement) != 0){
        req->fsm_state = REQ_STATE_ERROR;
        return(HTTP_STATE_CONTINUE);
    }

    kore_mem_free(sql_statement);
    req->fsm_state = REQ_STATE_DONE;
    return(HTTP_STATE_CONTINUE);
}

int mysql_request_perform_done(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    mysql_close(state);
    return(HTTP_STATE_COMPLETE);
}

int mysql_request_perform_error(struct http_request *req)
{
    MYSQL *state = req->hdlr_extra;

    char *err_mysql = gen_api_err_server_mysql(req);

    http_response(req, 500, err_mysql, (unsigned)strlen(err_mysql));

    mysql_close(state);
    return(HTTP_STATE_COMPLETE);
}
