#include "paths.h"

const char *pub_cols[] = {
    "userid, username, create_time, GROUP_CONCAT(groupname SEPARATOR \',\')",
    "groupid, groupname, description",
    "*",
    "applicationid, applicationname, description, create_time, last_edit_time, framework, username, groupname"
};

const char *priv_cols[] = {
    "userid, username, email, create_time, GROUP_CONCAT(groupname SEPARATOR \',\')",
    "*",
    "",
    "applicationid, applicationname, description, create_time, last_edit_time, framework, exec_path, data_path, username, groupname"
};

const char *id_str[] = {
    " WHERE userid = ",
    " WHERE groupid = ",
    " WHERE idpermissions = ",
    " WHERE applicationid = "
};

const char *select_ext[] = {
    " INNER JOIN users_has_groups ug ON users.userid = ug.users_userid INNER JOIN groups ON groups.groupid = ug.groups_groupid",
    "",
    "",
    ""
};

const char *insert_ext[] = {
    "",
    "",
    "",
    ""
};

const char *update_ext[] = {
    "",
    "",
    "",
    ""
};

const char *delete_ext[] = {
    "",
    "",
    "",
    ""
};
