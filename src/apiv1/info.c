#include "info.h"
#include "utils/jsonmessages.h"

int api_info(struct http_request *req)
{
    char * str_api_info = gen_api_info_str(API_NAME, API_VER, API_DATE);
    //JSON content.
    //http_response_header(req, "Content-Type", "text/plain");
    http_response(req, HTTP_STATUS_OK, str_api_info, (unsigned)strlen(str_api_info));
    kore_log(LOG_NOTICE, "[%s] %s -> %d", pretty_method(req->method), req->path, HTTP_STATUS_OK);
    return (KORE_RESULT_OK);
}
