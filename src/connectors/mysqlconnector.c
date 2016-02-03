#include "mysqlconnector.h"

int init_mysql_cfg(char * cfg_file_name, DB_CONN_CFG* db_conn_cfg)
{
    config_t cfg;
    db_conn_cfg = malloc(sizeof(DB_CONN_CFG));

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

    if(!config_lookup_string(&cfg, "server", &db_conn_cfg->server))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: server is not defined in conf.");
        db_conn_cfg->server = NULL;
    }

    if(!config_lookup_string(&cfg, "user", &db_conn_cfg->user))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: user is not defined in conf.");
        db_conn_cfg->user = NULL;
    }

    if(!config_lookup_string(&cfg, "password", &db_conn_cfg->password))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: password is not defined in conf.");
        db_conn_cfg->password = NULL;
    }

    if(!config_lookup_string(&cfg, "database", &db_conn_cfg->database))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: database is not defined in conf.");
        db_conn_cfg->database = NULL;
    }

    if(!config_lookup_int(&cfg, "port", &db_conn_cfg->port))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: port is not defined in conf.");
        db_conn_cfg->port = 0;
    }

    if(!config_lookup_string(&cfg, "unix_socket", &db_conn_cfg->unix_socket))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: unix_socket is not defined in conf.");
        db_conn_cfg->unix_socket = NULL;
    }

    if(!config_lookup_int64(&cfg, "flags", &db_conn_cfg->flags))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: flags is not defined in conf.");
        db_conn_cfg->flags = 0;
    }
    config_destroy(&cfg);
    return(MYSQL_OK);
}

//int connect_mysql(MYSQL *conn, DB_CONN_CFG *db_conn_cfg)
//{
    //conn = mysql_init(conn);
    //// Worker crashes when calls mysql_real_connect. (?)
    //if(!mysql_real_connect(conn, db_conn_cfg->server, db_conn_cfg->user,
        //db_conn_cfg->password, db_conn_cfg->database, db_conn_cfg->port,
        //db_conn_cfg->unix_socket, db_conn_cfg->flags))
    //{
        //kore_log(LOG_ERR, "%s\n", mysql_error(conn));
        //return(MYSQL_CONNECT_ERR);
    //}
    //return(MYSQL_OK);
//}

//int close_mysql(MYSQL * conn)
//{
    //return 0;
//}
