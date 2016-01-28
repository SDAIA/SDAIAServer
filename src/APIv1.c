#include "APIv1.h"

//Test
int page(struct http_request *req)
{
    http_response(req, 200, asset_meme_jpg, asset_len_meme_jpg);
    kore_log(0,"Nice meme sent");
    return (KORE_RESULT_OK);
}
