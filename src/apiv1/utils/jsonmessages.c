#include "jsonmessages.h"

const char * gen_api_info_str()
{
    //Generate JSON.
    json_object * api_info = json_object_new_object();
    json_object * api_name = json_object_new_string("SDAIA api");
    json_object * api_version = json_object_new_string("0.0.0");
    json_object * api_release_date = json_object_new_string("20160127");
    json_object_object_add(api_info, "name", api_name);
    json_object_object_add(api_info, "version", api_version);
    json_object_object_add(api_info, "releaseDate", api_release_date);
    //Generate string from JSON object.
    return json_object_to_json_string_ext(api_info, JSON_C_TO_STRING_PRETTY);
}
