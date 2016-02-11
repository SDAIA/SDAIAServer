#ifndef MYSQL_LIMITS_H_

#define MYSQL_LIMITS_H_

/* Max length of user, password and database name in MySQL.
 * Before 5.7.8 -> 16
 * After 5.7.8 -> 32
 */
#define MAX_LENGTH_USER 16
#define MAX_LENGTH_PASS 16
#define MAX_LENGTH_DB_NAME 64
#define MAX_LENGTH_COL_NAME 64
/* Memory allowed for queries can be from 1MB to 1GB in
 * latest MySQL versions. Modify this as your choice.
 * (in chars). Check size of char for exact bytes.
 */
#define MAX_LENGTH_QUERY 4096

#endif //MYSQL_LIMITS_H_
