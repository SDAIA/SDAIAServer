#include "users.h"


int api_users(struct http_request *req)
{
	char * str_api_notfound;
	if(req->method == HTTP_METHOD_POST)
	{
		//TODO: Check auth
		return(http_state_run(mysql_states, mysql_states_size, req));
	}
	else if(req->method == HTTP_METHOD_GET)
	{
		//TODO: Check auth
		http_populate_arguments();
		return(http_state_run(mysql_states, mysql_states_size, req));
		
	else
	{
		str_api_notfound = gen_api_err_notfound(req);
		kore_log(LOG_NOTICE, "[%s] %s -> %d", pretty_method(req->method), req->path, HTTP_STATUS_NOT_FOUND);
		http_response(req, HTTP_STATUS_NOT_FOUND, str_api_notfound, (unsigned)strlen(str_api_notfound));
		return(KORE_RESULT_OK);
	}
}
