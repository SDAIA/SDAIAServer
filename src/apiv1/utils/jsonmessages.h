#ifndef JSON_MESSAGES_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <json/json.h>
#include "prettycodes.h"

#define JSON_MESSAGES_H_

const char * gen_api_info_str(void);

/* HTTP ERROR STATUS */ 
const char * gen_api_err_forbidden(struct http_request *); //403
const char * gen_api_err_notfound(struct http_request *); //404
const char * gen_api_err_server(struct http_request *); //500

#endif // JSON_MESSAGES_H_
