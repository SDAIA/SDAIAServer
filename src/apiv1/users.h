#ifndef USERS_H_

#include <kore/kore.h>
#include <kore/http.h>
#include "connectors/mysqlconnector.h"
#include "utils/jsonmessages.h"
//#include "utils/prettycodes.h"

#define USERS_H_

int api_users(struct http_request *);

#endif // USERS_H_
