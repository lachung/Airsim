#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mysql/mysql.h>

#include "route_data.h"
#include "route_process.h"
#include "income_statement_data.h"

/* Sales Route History Process
 *     Function : 
 *       - Weekly
 *       - Monthly
 *
 */

void salesRouteHistoryWeeklyProcess(MYSQL* db, int week_number, int year, int month, int day){
    MYSQL_RES *sr_ptr;
    MYSQL_ROW sr_row;
    char*search_datetime = (char*)malloc(sizeof(char) * 70);
    char*submit_datetime = (char*)malloc(sizeof(char) * 70);
    
    sprintf(search_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    sprintf(submit_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    
    if(week_number == 0)
        week_number = 6;
    else
        week_number -= 1;
    
    sr_ptr = getSalesRouteDaily(db, search_datetime);
    if(sr_ptr){
        printf("Sales Route History Weekly Start - Search Time[%s] - Submit Time[%s]\n", search_datetime, submit_datetime);
        while(sr_row = mysql_fetch_row(sr_ptr)){
            // 0 => Sales Route ID
            // 1 => Sum of Pax
            // 2 => Average of Load Factor
            //printf("Sales Route ID [%lu] - History Processing\n", strtoul(sr_row[0], NULL, 0));
            submitSalesHistoryWeekly(db, strtoul(sr_row[0], NULL, 0), atoi(sr_row[1]), atof(sr_row[2]), week_number, submit_datetime);
        }
        mysql_free_result(sr_ptr);
        if(!removeSalesRouteHistoryDaily(db, search_datetime)){
            printf("Sales Route History Weekly - Can not remove daily records\n");
        }
    }
    free(search_datetime);
    free(submit_datetime);
}

void salesRouteHistoryMonthlyProcess(MYSQL* db, int year, int month, int day){
    MYSQL_RES *sr_ptr;
    MYSQL_ROW sr_row;
    char*search_datetime = (char*)malloc(sizeof(char) * 70);
    char*submit_datetime = (char*)malloc(sizeof(char) * 70);
    
    sprintf(search_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    sprintf(submit_datetime, "%d-%d-%d %d:%d:%d", year, month, day, 0, 0, 0);
    
    if(month == 1)
        month = 12;
    else
        month -= 1;
    
    usleep(600000);
    
    sr_ptr = getSalesRouteWeekly(db, search_datetime);
    if(sr_ptr){
        printf("History Monthly Start - Search Time[%s] - Submit Time[%s]\n", search_datetime, submit_datetime);
        while(sr_row = mysql_fetch_row(sr_ptr)){
            // 0 => Sales Route ID
            // 1 => Sum of Pax
            // 2 => Average of Load Factor
            submitSalesHistoryMonthly(db, strtoul(sr_row[0], NULL, 0), atoi(sr_row[1]), atof(sr_row[2]), month, submit_datetime);
        }
        mysql_free_result(sr_ptr);
        if(!removeSalesRouteHistoryWeekly(db, search_datetime)){
            printf("Sales Route History Monthly - Can not remove weekly records\n");
        }
    }
    free(search_datetime);
    free(submit_datetime);
}