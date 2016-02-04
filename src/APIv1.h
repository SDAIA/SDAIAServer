#ifndef API_V1_H_

#include "apiv1/info.h"
#include "apiv1/users.h"
#include "assets.h"
#include "connectors/mysqlconnector.h"


#define API_V1_H_

int page(struct http_request *);

#endif //API_V1_H_
