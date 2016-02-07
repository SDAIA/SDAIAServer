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

const char * gen_api_err_notfound(struct http_request * req)
{
	json_object * api_not_found = json_object_new_object();
	json_object * api_err_code = json_object_new_int(HTTP_STATUS_NOT_FOUND);
	json_object * api_method = json_object_new_int(req->method);
	json_object * api_path = json_object_new_string(req->path);
	json_object_object_add(api_not_found, "status", api_err_code);
	json_object_object_add(api_not_found, "method", api_method);
	json_object_object_add(api_not_found, "path", api_path);
	return json_object_to_json_string_ext(api_not_found, JSON_C_TO_STRING_PRETTY);
}
