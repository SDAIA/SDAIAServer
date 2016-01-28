#include "SDAIAServer.h"

int init()
{
    //TODO: Start connection to MySQL.
    kore_log(0,"TODO: MySQL connection.");
    return (KORE_RESULT_OK);
}


//Functions called from page requests
int get_index_page(struct http_request *req)
{
    http_response(req, 200, asset_index_html, asset_len_index_html);
    return (KORE_RESULT_OK);
}
