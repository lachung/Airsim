/* 
 * File:   database.h
 * Author: chung
 *
 * Created on August 12, 2013, 3:55 PM
 */
#include <mysql/mysql.h>

#ifndef DATABASE_H
#define	DATABASE_H

#ifdef	__cplusplus
extern "C" {
#endif

MYSQL_RES *getDataFromDB(MYSQL* db, char* query);


#ifdef	__cplusplus
}
#endif

#endif	/* DATABASE_H */

