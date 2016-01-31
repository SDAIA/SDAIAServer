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

#define FIN_OK 0
#define ERR_NO_REQUIRED 1

int init(void);
int get_index_page(struct http_request *);

#endif //SDAIA_SERVER_H_
