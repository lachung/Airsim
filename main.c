/* 
 * File:   main.c
 * Author: chung
 *
 * Created on August 12, 2013, 2:10 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "main.h"

//database
#include "route_data.h"
#include "route_process.h"
#include "income_statement_data.h"

//process
#include "route.h"





#define GAME_ID 1
#define DB_IP "183.178.237.194" 
#define DB_PORT 3308
#define DB_NAME "airsim"
#define DB_USERNAME "Chung"
#define DB_PASSWORD "235689"

#define FUEL_PRICE 400

int res;
pthread_t rpyt,rpjft, tt, tipt, srhwpt, srhmpt, iswpt, ismpt, isqpt, isypt;
MYSQL *db_main, *db_rpy, *db_rpjf, *db_tip, *db_srhwp, *db_srhmp, *db_iswp, *db_ismp, *db_isqp, *db_isyp;

struct game_time{
	int minute;
	int hour;
	int day;
	int month;
	int year;
    int weekdate;
    int week_number;
}gt;

/* 
 * Program entry point
 */

int main(int argc, char** argv) {
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;

	int i = 0;
	int num_fields;
	
	//Default game time value
	gt.minute = 0;
	gt.hour = 0;
	gt.day = 1;
	gt.month = 1;
	gt.year = 1900;
    gt.weekdate = 1;
    gt.week_number = 1;


	//Init database
	db_main = mysql_init(NULL);
    db_rpy = mysql_init(NULL);
    db_rpjf = mysql_init(NULL);
    db_tip = mysql_init(NULL);
    
	if(mysql_real_connect(db_main, DB_IP, "server_main", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0)){
        mysql_real_connect(db_tip, DB_IP, "server_tip", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
        mysql_real_connect(db_rpjf, DB_IP, "server_rp_jf", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
        mysql_real_connect(db_rpy, DB_IP, "server_rp_y", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
        printf("[MySQL]Connection Success\n");
		res = mysql_query(db_main, "SELECT * from game_time WHERE id = 1");
		
		if(res){
			printf("[MySQL select error]\n");		
		}else{
			res_ptr = mysql_store_result(db_main);
			if(res_ptr){
				printf("Retrieved %lu Rows\n", (unsigned long)mysql_num_rows(res_ptr));	
				num_fields = mysql_num_fields(res_ptr);
				while((sqlrow = mysql_fetch_row(res_ptr))){
                                    gt.weekdate = atoi(sqlrow[1]);
                                    gt.year = atoi(sqlrow[2]);
                                    gt.month = atoi(sqlrow[3]);
                                    gt.day = atoi(sqlrow[4]);
                                    gt.hour = atoi(sqlrow[5]);
                                    gt.minute = atoi(sqlrow[6]);
                                    gt.week_number = atoi(sqlrow[7]);
				}
			}		
		}
        mysql_free_result(res_ptr);

		printf("Game time received : %2d-%2d-%2d %2d:%2d:0 [Week - %d]\n", gt.year, gt.month, gt.day, gt.hour, gt.minute, gt.week_number);
                
		i = pthread_create(&tt, NULL, timer_thread, NULL);
		pthread_join(tt, NULL);
	}else{
        printf("Could not connect to DB server\n");
        printf("MYSQL ERROR Number => %d", mysql_errno(db_main));
        return (1);
    }   
    return (EXIT_SUCCESS);
}

/**
 * Thread
 * 
 */

void *timer_thread(void *ptr){
	char *buf;
	//game_time *gt = (game_time*)ptr;
    while(1){
        //printf("%2d-%2d-%2d %2d:%2d:0 [Weekdata - %d] [Week - %d]\n", gt->year, gt->month, gt->day, gt->hour , gt->minute, gt->weekdate, gt->week_number);
        
        gt.minute++;       
        if(gt.minute > 59){
            gt.minute = 0;
            gt.hour++;	
            if(gt.hour == 24){
                gt.hour = 0;
                gt.day++;
                gt.weekdate++;
                if(gt.weekdate == 7){
                    gt.weekdate = 0;
                    gt.week_number++;
                    pthread_create(&srhwpt, NULL, salesRouteHistoryWeekly_process, NULL);
                    pthread_create(&iswpt, NULL, incomeStatementWeekly_process, NULL);
                }
                if(gt.day == 31){
                    gt.day = 1;
                    gt.month++;
                    pthread_create(&srhmpt, NULL, salesRouteHistoryMonthly_process, NULL);
                    pthread_create(&ismpt, NULL, incomeStatementMonthly_process, NULL);
                        
                    if((gt.month % 3) == 1){
                        pthread_create(&isqpt, NULL, incomeStatementQuarterly_process, NULL);
                    }
                        
                    if(gt.month == 13){
                        gt.month = 1;
                        gt.week_number = 1;
                        gt.year++;
                        pthread_create(&isypt, NULL, incomeStatementYearly_process, NULL);
                    }					
                }				
            }
        }		
          
        buf = (char*)malloc(sizeof(char) * 200);
        //Update the game datetime every game time 5 minutes
                       
        sprintf(buf, "UPDATE game_time SET year = %d, month = %d, day = %d, hour = %d, minute = %d , week_date = %d, week_number = %d WHERE id = 1", gt.year, gt.month, gt.day, gt.hour, gt.minute, gt.weekdate, gt.week_number);
                        
        //printf("\n[%d]\n", strlen(buf));
        res = mysql_query(db_main, buf);
                        
        if(!res){
            //If update success
            printf("Game time updated => %d-%d-%d %d:%d [Weekdate - %d] [Week Number - %d] \n", gt.year, gt.month, gt.day, gt.hour, gt.minute, gt.weekdate, gt.week_number);
        }else{
            printf("Game time can not update\n");
            printf("MYSQL ERROR Number => %d", mysql_errno(db_main));
        }
        free(buf);
        
        if((gt.minute % 5) == 0){
            //Create a thread to process the routes data
            pthread_create(&rpyt, NULL, routes_process_y, NULL);
            pthread_create(&rpjft, NULL, routes_process_jf, NULL);
            pthread_create(&tipt, NULL, ticketIncome_process, NULL);
        }
            
        usleep(300000);
        //usleep(300000);
    }
}

void *routes_process_y(void *ptr){
    
    printf("Route Process Y Thread Entered\n");
    scoreProcessing(db_rpy, 0);
    pthread_detach(rpyt);
}

void *routes_process_jf(void *ptr){
    
    printf("Route Process JF Thread Entered\n");
    //usleep(300000);
    scoreProcessing(db_rpjf, 1); 
    pthread_detach(rpjft);
}

void *ticketIncome_process(void *ptr){
    
    printf("Ticket Income Process Thread Entered\n");
    int minute = 0;
    //Time correction
    minute = gt.minute;
    if((minute % 5) != 0){
        minute -= (minute % 5);
    }
    ticketIncomeProcessing(db_tip, gt.weekdate, gt.year, gt.month, gt.day, gt.hour, minute);
    pthread_detach(tipt);
}

void *salesRouteHistoryWeekly_process(void *ptr){
    db_srhwp = mysql_init(NULL);
    mysql_real_connect(db_srhwp, DB_IP, "server_srhwp", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
    salesRouteHistoryWeeklyProcess(db_srhwp, gt.week_number, gt.year, gt.month, gt.day);
    mysql_close(db_srhwp);
    mysql_thread_end();
    pthread_detach(srhwpt);
}

void *salesRouteHistoryMonthly_process(void *ptr){
    db_srhmp = mysql_init(NULL);
    mysql_real_connect(db_srhmp, DB_IP, "server_srhmp", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
    salesRouteHistoryMonthlyProcess(db_srhmp, gt.year, gt.month, gt.day);
    mysql_close(db_srhmp);
    mysql_thread_end();
    pthread_detach(srhmpt);
}

void *incomeStatementWeekly_process(void *ptr){
    db_iswp = mysql_init(NULL);
    mysql_real_connect(db_iswp, DB_IP, "server_iswp", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
    incomeStatementWeeklyProcess(db_iswp, gt.year, gt.month, gt.day, gt.week_number);
    mysql_close(db_iswp);
    mysql_thread_end();
    pthread_detach(iswpt);
}

void *incomeStatementMonthly_process(void *ptr){
    db_ismp = mysql_init(NULL);
    mysql_real_connect(db_ismp, DB_IP, "server_ismp", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
    incomeStatementMonthlyProcess(db_ismp, gt.year, gt.month, gt.day);
    mysql_close(db_ismp);
    mysql_thread_end();
    pthread_detach(ismpt);
}

void *incomeStatementQuarterly_process(void *ptr){
    db_isqp = mysql_init(NULL);
    mysql_real_connect(db_isqp, DB_IP, "server_isqp", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
    incomeStatementQuarterlyProcess(db_isqp, gt.year, gt.month, gt.day);
    mysql_close(db_isqp);
    mysql_thread_end();
    pthread_detach(isqpt);
}

void *incomeStatementYearly_process(void *ptr){
    db_isyp = mysql_init(NULL);
    mysql_real_connect(db_isyp, DB_IP, "server_isyp", DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0);
    incomeStatementYearlyProcess(db_isyp, gt.year, gt.month, gt.day);
    mysql_close(db_isyp);
    mysql_thread_end();
    pthread_detach(isypt);
}






