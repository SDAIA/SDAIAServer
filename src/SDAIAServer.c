#include "SDAIAServer.h"

int load_prereq()
{
    //TODO: Start connection to MySQL.
    if(read_mysql_cfg(MYSQL_CFG_PATH, my_db_cfg) != 0)
    {
        kore_log(LOG_ERR, "MySQL database configuration file is required to work. Sending quit signal to parent.");
        kill(getppid(), SIGQUIT);
        exit(EXIT_FAILURE);
    }
    return (KORE_RESULT_OK);
}


//Functions called from page requests
int get_index_page(struct http_request *req)
{
    http_response(req, 200, asset_index_html, asset_len_index_html);
    return (KORE_RESULT_OK);
}
