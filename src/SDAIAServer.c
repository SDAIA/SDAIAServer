#include "SDAIAServer.h"

int init()
{
    //TODO: Start connection to MySQL.
    if(read_mysql_cfg("conf/mysql.conf", my_db_cfg) != 0)
    {
		kore_log(LOG_ERR, "MySQL database configuration file is required to work. Sending quit signal to parent.");
		kill(getppid(), SIGQUIT);
		exit(ERR_NO_REQUIRED);
	}
    return (KORE_RESULT_OK);
}


//Functions called from page requests
int get_index_page(struct http_request *req)
{
    http_response(req, 200, asset_index_html, asset_len_index_html);
    return (KORE_RESULT_OK);
}
