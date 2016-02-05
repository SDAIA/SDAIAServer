#include "mysqlconnector.h"

struct http_state mysql_states[] = {
        {"REQ_STATE_INIT", mysql_request_perform_init},
        {"REQ_STATE_QUERY", mysql_request_perform_query},
        {"REQ_STATE_DB_WAIT", mysql_request_db_wait},
        {"REQ_STATE_DB_READ", mysql_request_db_read},
        {"REQ_STATE_ERROR", mysql_request_error},
        {"REQ_STATE_DONE", mysql_request_done},
};

int init_mysql_cfg(char * cfg_file_name)
{
    config_t cfg;

    const char *server, *user, *password, *database, *unix_socket;

    ////Assign memory to strings in cfg struct.
    //db_cfg.server = kore_malloc(sizeof(char) * HOST_NAME_MAX);
    //db_cfg.user = kore_malloc(sizeof(char) * MAX_LENGTH_USER);
    //db_cfg.password = kore_malloc(sizeof(char) * MAX_LENGTH_PASS);
    //db_cfg.database = kore_malloc(sizeof(char) * MAX_LENGTH_DB_NAME);
    //db_cfg.unix_socket = kore_malloc(sizeof(char) * PATH_MAX);

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

    if(config_lookup_string(&cfg, "server", &server))
    {
        db_cfg.server = kore_malloc(sizeof(char) * HOST_NAME_MAX);
        strcpy(db_cfg.server, server);
    }
    else
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: server is not defined in conf.");
        db_cfg.server = NULL;
    }

    if(config_lookup_string(&cfg, "user", &user))
    {
        db_cfg.user = kore_malloc(sizeof(char) * MAX_LENGTH_USER);
        strcpy(db_cfg.user, user);
    }
    else
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: user is not defined in conf.");
        db_cfg.user = NULL;
    }

    if(config_lookup_string(&cfg, "password", &password))
    {
        db_cfg.password = kore_malloc(sizeof(char) * MAX_LENGTH_PASS);
        strcpy(db_cfg.password, password);
    }
    else
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: password is not defined in conf.");
        db_cfg.password = NULL;
    }

    if(config_lookup_string(&cfg, "database", &database))
    {
        db_cfg.database = kore_malloc(sizeof(char) * MAX_LENGTH_DB_NAME);
        strcpy(db_cfg.database, database);
    }
    else
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: database is not defined in conf.");
        db_cfg.database = NULL;
    }

    if(!config_lookup_int(&cfg, "port", &db_cfg.port))
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: port is not defined in conf.");
        db_cfg.port = 0;
    }

    if(config_lookup_string(&cfg, "unix_socket", &unix_socket))
    {
        db_cfg.unix_socket = kore_malloc(sizeof(char) * PATH_MAX);
        strcpy(db_cfg.unix_socket, unix_socket);
    }
    else
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: unix_socket is not defined in conf.");
        db_cfg.unix_socket = NULL;
    }

    if(!config_lookup_int64(&cfg, "flags", &db_cfg.flags))
    {
        kore_log(LOG_WARNING, "In init_mysql_cfg: flags is not defined in conf.");
        db_cfg.flags = 0;
    }

    kore_log(LOG_DEBUG, "Server: %s\nUser: %s\nDatabase: %s\nPort: %d\nSocket: %s\n",
        db_cfg.server, db_cfg.user, db_cfg.database, db_cfg.port, db_cfg.unix_socket);

    config_destroy(&cfg);
    return(MYSQL_OK);
}
