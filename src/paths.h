#ifndef PATHS_H_

#define PATHS_H_

#define MYSQL_CFG_PATH "conf/mysql.conf"

#define USER_TABLE 0
#define GROUP_TABLE 1
#define PERMISSIONS_TABLE 2
#define APPLICATIONS_TABLE 3

extern const char *pub_cols[4];
extern const char *priv_cols[4];
extern const char *id_str[4];
extern const char *select_ext[4];
extern const char *insert_ext[4];
extern const char *update_ext[4];
extern const char *delete_ext[4];

#endif //PATHS_H_
