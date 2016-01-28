#include "info.h"
#include "utils/jsonmessages.h"

int api_info(struct http_request *req)
{
    const char * str_api_info = gen_api_info_str();
    //JSON content.
    //http_response_header(req, "Content-Type", "text/plain");
    http_response(req, 200, str_api_info, (unsigned)strlen(str_api_info));
    kore_log(0,"Api info sent");
    return (KORE_RESULT_OK);
}
