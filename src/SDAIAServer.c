#include <kore/kore.h>
#include <kore/http.h>

#include "assets.h"

//Functions called from page requests
int get_index_page(struct http_request *);

//Functions called from api requests
int get_api_info(struct http_request *);

//Test
int page(struct http_request *);

//Functions called from page requests
int get_index_page(struct http_request *req)
{
    http_response(req, 200, asset_index_html, asset_len_index_html);
    return (KORE_RESULT_OK);
}

//Functions called from api requests
int get_api_info(struct http_request *req)
{
    //JSON content.
    http_response_header(req, "Content-Type", "application/json");
    http_response(req, 200, NULL, 0);
    return (KORE_RESULT_OK);
}


//Test
int page(struct http_request *req) {
    http_response(req, 200, asset_meme_jpg, asset_len_meme_jpg);
    kore_log(0,"Nice meme sent");
    return (KORE_RESULT_OK);
}
