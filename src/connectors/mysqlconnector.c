#include "mysqlconnector.h"

struct http_state mysql_states[] = {
    {"REQ_STATE_INIT", mysql_request_perform_init},
    {"REQ_STATE_SWITCH", mysql_request_perform_switch},
    {"REQ_STATE_SELECT", },
    {"REQ_STATE_GETDATA", },
    {"REQ_STATE_INSERT", },
    {"REQ_STATE_UPDATE", },
    {"REQ_STATE_DELETE", },
    {"REQ_STATE_DONE", },
    {"REQ_STATE_ERROR", }
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
    return(MYSQL_OK);
}
