#ifndef INFO_H_

#include <kore/kore.h>
#include <kore/http.h>
#include "utils/jsonmessages.h"

#define INFO_H_

#define API_NAME "SDAIA API"
#define API_VER "0.0.0"
#define API_DATE "20160217"

int api_info(struct http_request *);

#endif // INFO_H_
