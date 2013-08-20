#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mysql/mysql.h>




bool submitIncomStatementData(MYSQL* db, int transact_type_id, int airline_id, int airport_id, int amount, int week_date, char* game_date){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "INSERT INTO income_statement_daily VALUES(NULL, %d, %d, %d, %d, %d, '%s', NULL)", transact_type_id, airline_id, airport_id, amount, week_date, game_date);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool updateAirlineCash(MYSQL* db, int airline_id, long long int cash){
    char *buf = (char*)malloc(sizeof(char) * 300);
    sprintf(buf, "UPDATE airline SET cash = %lld WHERE id = %d", cash, airline_id);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

MYSQL_RES* getIncomeStatementDaily(MYSQL* db, char* search_datetime){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 300);
    sprintf(buf, "SELECT transact_type_id_id, airline_id_id, airport_id_id, SUM(amount) FROM income_statement_daily WHERE ts_created < '%s' GROUP BY transact_type_id_id, airline_id_id, airport_id_id", search_datetime);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

MYSQL_RES* getIncomeStatementWeekly(MYSQL* db, char* search_datetime){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 300);
    sprintf(buf, "SELECT transact_type_id_id, airline_id_id, airport_id_id, SUM(amount) FROM income_statement_weekly WHERE ts_created < '%s' GROUP BY transact_type_id_id, airline_id_id, airport_id_id", search_datetime);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

MYSQL_RES* getIncomeStatementMonthly(MYSQL* db, char* search_datetime){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 300);
    sprintf(buf, "SELECT transact_type_id_id, airline_id_id, airport_id_id, SUM(amount) FROM income_statement_monthly WHERE ts_created < '%s' GROUP BY transact_type_id_id, airline_id_id, airport_id_id", search_datetime);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

MYSQL_RES* getIncomeStatementQuarterly(MYSQL* db, char* search_datetime){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 300);
    sprintf(buf, "SELECT transact_type_id_id, airline_id_id, airport_id_id, SUM(amount) FROM income_statement_quarterly WHERE ts_created < '%s' GROUP BY transact_type_id_id, airline_id_id, airport_id_id", search_datetime);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}


bool submitIncomeStatementWeekly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int week_number){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "INSERT INTO income_statement_weekly VALUES(NULL, %d, %d, %d, %lld, %d, '%s', NULL)", transact_id, airline_id, airport_id, amount, week_number, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool submitIncomeStatementMonthly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int month){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "INSERT INTO income_statement_monthly VALUES(NULL, %d, %d, %d, %lld, %d, '%s', NULL)", transact_id, airline_id, airport_id, amount, month, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool submitIncomeStatementQuarterly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int quarter){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "INSERT INTO income_statement_quarterly VALUES(NULL, %d, %d, %d, %lld, %d, '%s', NULL)", transact_id, airline_id, airport_id, amount, quarter, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool submitIncomeStatementYearly(MYSQL* db, int airline_id, int airport_id, int transact_id, long long int amount, char* submit_datetime, int year){
    char *buf = (char*)malloc(sizeof(char) * 500);
    sprintf(buf, "INSERT INTO income_statement_yearly VALUES(NULL, %d, %d, %d, %lld, %d, '%s', NULL)", transact_id, airline_id, airport_id, amount, year, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeIncomeStatementDaily(MYSQL* db, char* search_datetime){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "DELETE FROM income_statement_daily WHERE ts_created < '%s'", search_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeIncomeStatementWeekly(MYSQL* db, char* search_datetime){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "DELETE FROM income_statement_weekly WHERE  ts_created <= '%s'", search_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeIncomeStatementMonthly(MYSQL* db, char* search_datetime){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "DELETE FROM income_statement_monthly WHERE ts_created <= '%s'", search_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeIncomeStatementQuarterly(MYSQL* db, char* search_datetime){
    char *buf = (char*)malloc(sizeof(char) * 400);
    sprintf(buf, "DELETE FROM income_statement_quarterly WHERE ts_created <= '%s'", search_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

