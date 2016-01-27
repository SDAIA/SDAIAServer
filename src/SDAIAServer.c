#include <kore/kore.h>
#include <kore/http.h>

#include "assets.h"

//Functions called from page requests
int get_index_page(struct http_request *);

//Functions called from page requests
int get_index_page(struct http_request *req)
{
    http_response(req, 200, asset_index_html, asset_len_index_html);
    return (KORE_RESULT_OK);
}
