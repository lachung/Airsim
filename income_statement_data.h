/* 
 * File:   income_statement_data.h
 * Author: chung
 *
 * Created on August 12, 2013, 2:28 PM
 */

#include <mysql/mysql.h>

#ifndef INCOME_STATEMENT_DATA_H
#define	INCOME_STATEMENT_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    bool submitIncomStatementData(MYSQL *db, int transact_type_id, int airline_id, int airport_id, int amount, int week_date, char* game_date);
    bool updateAirlineCash(MYSQL *db, int airline_id, long long int cash);
        
    //For income statement history process
    MYSQL_RES* getIncomeStatementDaily(MYSQL* db, char* search_datetime);
    MYSQL_RES* getIncomeStatementWeekly(MYSQL* db, char* search_datetime);
    MYSQL_RES* getIncomeStatementMonthly(MYSQL* db, char* search_datetime);
    MYSQL_RES* getIncomeStatementQuarterly(MYSQL* db, char* search_datetime);
    MYSQL_RES* getIncomeStatementYearly(MYSQL* db, char* search_datetime);
    bool submitIncomeStatementWeekly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int week_number);
    bool submitIncomeStatementMonthly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int month);
    bool submitIncomeStatementQuarterly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int quarter);
    bool submitIncomeStatementYearly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int year);
    bool removeIncomeStatementDaily(MYSQL *db, char* search_datetime);
    bool removeIncomeStatementWeekly(MYSQL *db, char* search_datetime);
    bool removeIncomeStatementMonthly(MYSQL *db,  char* search_datetime);
    bool removeIncomeStatementQuarterly(MYSQL* db, char* search_datetime);


#ifdef	__cplusplus
}
#endif

#endif	/* INCOME_STATEMENT_DATA_H */

