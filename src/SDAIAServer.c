#include "SDAIAServer.h"

int load_prereq()
{
    //TODO: Start connection to MySQL.
    //Set NULL to MYSQL connection to avoid crashes.

    if(init_mysql_cfg(MYSQL_CFG_PATH, db_cfg) != 0)
    {
        kore_log(LOG_ERR,
            "MySQL database configuration file is required to work. SIGQUIT to parent.");
        kill(getppid(), SIGQUIT);
        exit(EXIT_FAILURE);
    }
    //if(connect_mysql(connection, db_cfg) != 0)
    //{
        //kore_log(LOG_ERR,
            //"Cannot connect to database. Check configuration file and MySQL server. SIGQUIT to parent.");
        //kill(getppid(), SIGQUIT);
        //exit(EXIT_FAILURE);
    //}
    //else
    //{
        //kore_log(LOG_NOTICE, "Connected to MySQL server database.");
    //}
    return (KORE_RESULT_OK);
}


//Functions called from page requests
int get_index_page(struct http_request *req)
{
    http_response(req, 200, asset_index_html, asset_len_index_html);
    return (KORE_RESULT_OK);
}
