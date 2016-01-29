#include "mysqlconnector.h"

int read_mysql_cfg(char * cfg_file_name, DB_CONN_CFG db_conn_cfg)
{
    int fd = open(cfg_file_name, O_RDONLY);
    struct stat fs;
    char *buffer, *buffer_end;
    char *begin, *end, c;

    if(fd == -1)
    {
        kore_log(LOG_ERR, "In read_mysql_cfg: Can't open config file.");
        return (MYSQL_CFG_READ_ERROR);
    }

    if(fstat(fd, &fs) == -1)
    {
        kore_log(LOG_ERR, "In read_mysql_cfg: Negative file stat.");
        return (MYSQL_CFG_READ_ERROR);
    }

    buffer = mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(buffer == (void*) -1)
    {
        kore_log(LOG_ERR, "In read_mysql_cfg: Mmap unexpected error.");
        close(fd);
        return (MYSQL_CFG_READ_ERROR);
    }

    buffer_end = buffer + fs.st_size;
    begin = end = buffer;

    for(;;)
    {
        if(!(*end == '\r' || *end == '\n'))
        {
            if(++end < buffer_end)
            {
                continue;
            }
        }
        else if(1 + end < buffer_end)
        {
            c = *(1 + end);
            if((c == '\r' || c == '\n') && c != *end)
            {
                ++end;
            }
        }


    }
}

int connect_mysql(MYSQL * conn, DB_CONN_CFG db_conn_cfg)
{
    return 0;
}

int close_mysql(MYSQL * conn)
{
    return 0;
}
