#ifndef JSON_READER_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <json/json.h>

#define JSON_READER_H_

int parse_json_post_body(char *, char **, char **);

#endif // JSON_READER_H_
