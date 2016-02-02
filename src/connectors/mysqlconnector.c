#include "mysqlconnector.h"

int read_mysql_cfg(char * cfg_file_name, DB_CONN_CFG db_conn_cfg)
{
    config_t cfg, *cf;

    cf = &cfg;
    config_init(cf);

    if(!config_read_file(cf, MYSQL_CFG_PATH))
    {
        kore_log(LOG_ERR, "%s:%d - %s\n",
            config_error_file(cf),
            config_error_line(cf),
            config_error_text(cf));
        config_destroy(cf);
        return(MYSQL_CFG_READ_ERR);
    }

    if(!config_lookup_string(cf, "server", &db_conn_cfg.server))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: server is not defined in conf.");
    }

    if(!config_lookup_string(cf, "user", &db_conn_cfg.user))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: user is not defined in conf.");
    }

    if(!config_lookup_string(cf, "password", &db_conn_cfg.password))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: password is not defined in conf.");
    }

    if(!config_lookup_string(cf, "database", &db_conn_cfg.database))
    {
        kore_log(LOG_WARNING, "In read_mysql_cfg: database is not defined in conf.");
    }

    if(!config_lookup_int(cf, "port", &db_conn_cfg.port))
    {
        kore_log(LOG_NOTICE, "In read_mysql_cfg: port is not defined in conf.");
    }

    if(!config_lookup_string(cf, "unix_socket", &db_conn_cfg.unix_socket))
    {
        kore_log(LOG_NOTICE, "In read_mysql_cfg: unix_socket is not defined in conf.");
    }

    if(!config_lookup_long(cf, "flags", &db_conn_cfg.flags))
    {
        kore_log(LOG_NOTICE, "In read_mysql_cfg: flags is not defined in conf.");
    }

    config_destroy(cf);
    return(MYSQL_CFG_READ_OK);
}

int connect_mysql(MYSQL * conn, DB_CONN_CFG db_conn_cfg)
{
    conn = mysql_init(conn);
    if(!mysql_real_connect(conn, db_conn_cfg.server, db_conn_cfg.user,
        db_conn_cfg.password, db_conn_cfg.database, 0, NULL, 0))
    {
        kore_log(LOG_ERR, "%s\n", mysql_error(conn));
        return(MYSQL_CONNECTION_ERR);
    }

    return(MYSQL_CONNECTION_OK);

}

int close_mysql(MYSQL * conn)
{
    return 0;
}
