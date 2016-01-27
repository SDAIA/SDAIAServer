#include "info.h"

int get_api_info(struct http_request *req){
    //Generate JSON.
    json_object * api_info = json_object_new_object();
    json_object * api_name = json_object_new_string("SDAIA api");
    json_object * api_version = json_object_new_string("0.0.0");
    json_object * api_release_date = json_object_new_string("20160127");
    json_object_object_add(api_info, "name", api_name);
    json_object_object_add(api_info, "version", api_version);
    json_object_object_add(api_info, "releaseDate", api_release_date);
    //Generate string from JSON object.
    const char *str_api_info = json_object_to_json_string_ext(api_info, JSON_C_TO_STRING_PRETTY);
    //JSON content.
    //http_response_header(req, "Content-Type", "text/plain");
    http_response(req, 200, str_api_info, (unsigned)strlen(str_api_info));
    kore_log(0,"Api info sent");
    return (KORE_RESULT_OK);
}
