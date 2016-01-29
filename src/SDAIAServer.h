#ifndef SDAIA_SERVER_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>

#include "assets.h"
#include "APIv1.h"
#include "connectors/mysqlconnector.h"

#define SDAIA_SERVER_H_

int init(void);
int get_index_page(struct http_request *);

#endif //SDAIA_SERVER_H_
