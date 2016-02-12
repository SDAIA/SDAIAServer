#include "jsonmessages.h"

const char * gen_api_info_str(char *api_name, char *api_ver, char *api_date)
{
    //Generate JSON.
    json_object * api_info = json_object_new_object();
    json_object * api_name_json = json_object_new_string(api_name);
    json_object * api_ver_json = json_object_new_string(api_ver);
    json_object * api_date_json = json_object_new_string(api_date);
    json_object_object_add(api_info, "name", api_name_json);
    json_object_object_add(api_info, "version", api_ver_json);
    json_object_object_add(api_info, "release_date", api_date_json);
    //Generate string from JSON object.
    return json_object_to_json_string_ext(api_info, JSON_C_TO_STRING_PRETTY);
}

/* MYSQL RESULTS */
const char * gen_mysql_result(MYSQL_RES *result)
{
    MYSQL_ROW row;
    MYSQL_FIELD *mysql_field = NULL;

    int i;
    int num_fields = mysql_num_fields(result);
    char *fields[num_fields];

    //Populate field names.
    for(i=0; i<num_fields; i++)
    {
        mysql_field = mysql_fetch_field(result);
        fields[i] = kore_malloc(sizeof(char) * MAX_LENGTH_COL_NAME);
        memcpy(fields[i], mysql_field->name, strlen(mysql_field->name));
    }

    //Populate JSON.
    json_object * mysql_result = json_object_new_array();
    while((row = mysql_fetch_row(result)))
    {
        json_object * mysql_row = json_object_new_object();
        for(i=0; i<num_fields; i++)
        {
            json_object * row_field;
            if(row[i] == NULL)
            {
                row_field = json_object_new_string("NULL");
            }
            else
            {
                row_field = json_object_new_string(row[i]);
            }
            json_object_object_add(mysql_row, fields[i], row_field);
        }
        json_object_array_add(mysql_result, mysql_row);
    }

    //Free mem allocated.
    for(i=0; i<num_fields; i++)
    {
        kore_mem_free(fields[i]);
    }

    return json_object_to_json_string_ext(mysql_result, JSON_C_TO_STRING_PRETTY);

}

/* HTTP ERROR STATUS */
const char * gen_api_err_forbidden(struct http_request *req)
{
    json_object * api_forbidden = json_object_new_object();
    json_object * api_err_code = json_object_new_int(HTTP_STATUS_FORBIDDEN);
    json_object * api_err_pretty = json_object_new_string(pretty_codes(HTTP_STATUS_FORBIDDEN));
    json_object * api_method = json_object_new_string(pretty_method(req->method));
    json_object * api_path = json_object_new_string(req->path);
    json_object_object_add(api_forbidden, "status", api_err_code);
    json_object_object_add(api_forbidden, "status_str", api_err_pretty);
    json_object_object_add(api_forbidden, "method", api_method);
    json_object_object_add(api_forbidden, "path", api_path);
    return json_object_to_json_string_ext(api_forbidden, JSON_C_TO_STRING_PRETTY);
}

const char * gen_api_err_notfound(struct http_request *req)
{
    json_object * api_not_found = json_object_new_object();
    json_object * api_err_code = json_object_new_int(HTTP_STATUS_NOT_FOUND);
    json_object * api_err_pretty = json_object_new_string(pretty_codes(HTTP_STATUS_NOT_FOUND));
    json_object * api_method = json_object_new_string(pretty_method(req->method));
    json_object * api_path = json_object_new_string(req->path);
    json_object_object_add(api_not_found, "status", api_err_code);
    json_object_object_add(api_not_found, "status_str", api_err_pretty);
    json_object_object_add(api_not_found, "method", api_method);
    json_object_object_add(api_not_found, "path", api_path);
    return json_object_to_json_string_ext(api_not_found, JSON_C_TO_STRING_PRETTY);
}

const char * gen_api_err_server_mysql(struct http_request *req)
{
}
