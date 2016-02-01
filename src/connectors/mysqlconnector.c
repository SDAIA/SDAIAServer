#include "mysqlconnector.h"

int read_mysql_cfg(char * cfg_file_name, DB_CONN_CFG db_conn_cfg)
{
    config_t cfg, *cf;

    cf = &cfg;
    config_init(cf);

    if(!config_read_file(cf, MYSQL_CFG_PATH))
    {
        fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf),
            config_error_line(cf),
            config_error_text(cf));
        config_destroy(cf);
        return(MYSQL_CFG_READ_ERROR);
    }

    if(!config_lookup_string(cf, "server", &db_conn_cfg.server))
    {
        fprintf(stdout, "In read_mysql_cfg: server is not defined in conf.");
    }

    if(!config_lookup_string(cf, "user", &db_conn_cfg.user))
    {
        fprintf(stdout, "In read_mysql_cfg: user is not defined in conf.");
    }

    if(!config_lookup_string(cf, "password", &db_conn_cfg.password))
    {
        fprintf(stdout, "In read_mysql_cfg: password is not defined in conf.");
    }

    if(!config_lookup_string(cf, "database", &db_conn_cfg.database))
    {
        fprintf(stdout, "In read_mysql_cfg: database is not defined in conf.");
    }

    config_destroy(cf);
    return(MYSQL_CFG_READ_OK);
}

int connect_mysql(MYSQL * conn, DB_CONN_CFG db_conn_cfg)
{
    return 0;
}

int close_mysql(MYSQL * conn)
{
    return 0;
}
