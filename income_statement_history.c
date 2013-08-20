#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mysql/mysql.h>

#include "route_data.h"
#include "route_process.h"
#include "income_statement_data.h"

/* Income statement weekly history process
 * 
 * 
 */

void incomeStatementWeeklyProcess(MYSQL* db, int year, int month, int day, int week_number){
    MYSQL_RES *is_ptr;
    MYSQL_ROW is_row;
    char* search_datetime = (char*)malloc(sizeof(char) * 100);
    char* submit_datetime = (char*)malloc(sizeof(char) * 100);
    sprintf(search_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    sprintf(submit_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    
    if(week_number == 0)
        week_number = 6;
    else
        week_number -= 1;
    
    is_ptr = getIncomeStatementDaily(db, search_datetime);
    if(is_ptr){
        printf("Income Statement Weekly Process Start - Search Time[%s] - Submit Time[%s]\n", search_datetime, submit_datetime);
        while(is_row = mysql_fetch_row(is_ptr)){
            // 0 => Transact Type ID
            // 1 => Airline ID
            // 2 => Airport ID
            // 3 => Amount
            
            submitIncomeStatementWeekly(db, atoi(is_row[1]), atoi(is_row[2]), atoi(is_row[0]), atoll(is_row[3]), submit_datetime, week_number);
        }
        mysql_free_result(is_ptr);
        if(!removeIncomeStatementDaily(db, search_datetime)){
            printf("Income Statement Weekly - Can not remove daily records\n");
        }
    }
    free(search_datetime);
    free(submit_datetime);
}

void incomeStatementMonthlyProcess(MYSQL* db, int year, int month, int day){
    MYSQL_RES* is_ptr;
    MYSQL_ROW is_row;
    char* search_datetime = (char*)malloc(sizeof(char) * 100);
    char* submit_datetime = (char*)malloc(sizeof(char) * 100);
    sprintf(search_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    sprintf(submit_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    
    if(month == 1){
        month = 12;
    }else{
        month -= 1;
    }
    
    usleep(600000);
    
    is_ptr = getIncomeStatementWeekly(db, search_datetime);
    if(is_ptr){
        printf("Income Statement Monthly Process Start - Search Time[%s] - Submit Time[%s]\n", search_datetime, submit_datetime);
        while(is_row = mysql_fetch_row(is_ptr)){
            // 0 => Transact Type ID
            // 1 => Airline ID
            // 2 => Airport ID
            // 3 => Amount
            
            submitIncomeStatementMonthly(db, atoi(is_row[1]), atoi(is_row[2]), atoi(is_row[0]), atoll(is_row[3]), submit_datetime, month);
        }
        mysql_free_result(is_ptr);
        if(!removeIncomeStatementWeekly(db, search_datetime)){
            printf("Income Statement Monthly - Can not remove weekly records\n");
        }
    }
    free(search_datetime);
    free(submit_datetime);
}

void incomeStatementQuarterlyProcess(MYSQL* db, int year, int month, int day){
    MYSQL_RES* is_ptr;
    MYSQL_ROW is_row;
    int quarter = 0;
    char* search_datetime = (char*)malloc(sizeof(char) * 100);
    char* submit_datetime = (char*)malloc(sizeof(char) * 100);
    sprintf(search_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    sprintf(submit_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    
    if((month - 1) < 1){
        month = 12;
    }
    
    switch(month){
        case 1:
        case 2:
        case 3:
            quarter = 1;
            break;
        case 4:
        case 5:
        case 6:
            quarter = 2;
            break;
        case 7:
        case 8:
        case 9:
            quarter = 3;
            break;
        case 10:
        case 11:
        case 12:
            quarter = 4;
            break;
        default :
            quarter = 0;
            break;
    }
    
    usleep(600000);
    
    is_ptr = getIncomeStatementWeekly(db, search_datetime);
    if(is_ptr){
        printf("Income Statement Quarter Process Start - Search Time[%s] - Submit Time[%s]\n", search_datetime, submit_datetime);
        while(is_row = mysql_fetch_row(is_ptr)){
            // 0 => Transact Type ID
            // 1 => Airline ID
            // 2 => Airport ID
            // 3 => Amount
            
            submitIncomeStatementQuarterly(db, atoi(is_row[1]), atoi(is_row[2]), atoi(is_row[0]), atoll(is_row[3]), submit_datetime, quarter);
        }
        mysql_free_result(is_ptr);
        if(!removeIncomeStatementMonthly(db, search_datetime)){
            printf("Income Statement Quarterly - Can not remove weekly records\n");
        }
    }
    free(search_datetime);
    free(submit_datetime);
}



void incomeStatementYearlyProcess(MYSQL* db, int year, int month, int day){
    MYSQL_RES* is_ptr;
    MYSQL_ROW is_row;
    char* search_datetime = (char*)malloc(sizeof(char) * 100);
    char* submit_datetime = (char*)malloc(sizeof(char) * 100);
    sprintf(search_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    sprintf(submit_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    
    year -= 1;
    
    usleep(600000);
    
    is_ptr = getIncomeStatementQuarterly(db, search_datetime);
    if(is_ptr){
        printf("Income Statement Yearly Process Start - Search Time[%s] - Submit Time[%s]\n", search_datetime, submit_datetime);
        while(is_row = mysql_fetch_row(is_ptr)){
            // 0 => Transact Type ID
            // 1 => Airline ID
            // 2 => Airport ID
            // 3 => Amount
            
            submitIncomeStatementYearly(db, atoi(is_row[1]), atoi(is_row[2]), atoi(is_row[0]), atoll(is_row[3]), submit_datetime, year);
        }
        mysql_free_result(is_ptr);
        if(!removeIncomeStatementQuarterly(db, search_datetime)){
            printf("Income Statement Yearly - Can not remove quarterly records\n");
        }
    }
    free(search_datetime);
    free(submit_datetime);
}