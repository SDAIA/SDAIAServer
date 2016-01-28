#ifndef SDAIA_SERVER_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <mysql.h>

#include "assets.h"
#include "APIv1.h"

#define SDAIA_SERVER_H_

MYSQL * connection;
MYSQL_RES * res;
MYSQL_ROW * row;

int init(void);
int get_index_page(struct http_request *);

#endif //SDAIA_SERVER_H_
