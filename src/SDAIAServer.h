#ifndef SDAIA_SERVER_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "assets.h"
#include "APIv1.h"
#include "connectors/mysqlconnector.h"

#define SDAIA_SERVER_H_

//Global vars.
DB_CONN_CFG *db_cfg;
MYSQL *connection;

int load_prereq(void);
int get_index_page(struct http_request *);

#endif //SDAIA_SERVER_H_
