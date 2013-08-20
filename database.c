#include <stdlib.h>
#include <mysql/mysql.h>



MYSQL_RES *getDataFromDB(MYSQL* db, char* query){
    mysql_query(db, query);
    return mysql_store_result(db);
}