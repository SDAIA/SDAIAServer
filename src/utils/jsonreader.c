#include "jsonreader.h"

int parse_json_post_body(char *post_str, char **names, char **values)
{
    json_object *jobj = json_tokener_parse(post_str);

    enum json_type json_types;
    json_object_object_foreach(jobj, key, val)
