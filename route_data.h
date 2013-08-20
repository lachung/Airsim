/* 
 * File:   route_data.h
 * Author: chung
 *
 * Created on August 12, 2013, 2:22 PM
 */
#include <mysql/mysql.h>

#ifndef ROUTE_DATA_H
#define	ROUTE_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

    MYSQL_RES *getAllRouteData(MYSQL *db);
    MYSQL_RES *getRouteDataByPhysicalRouteID(MYSQL *db, int physical_route_id, int seat_class);
    MYSQL_RES *getRouteProcessQuery(MYSQL *db);
    MYSQL_RES *getSalesRouteDataByPhysicalRouteID(MYSQL *db, int physical_route_id, int seat_class);
    MYSQL_RES *getSalesRouteDataByDepartureTime(MYSQL *db, int departure_weekdate, int departure_time);
    MYSQL_RES *getAirlineInfo(MYSQL *db, int airline_id);
    MYSQL_RES *getSalesRouteDaily(MYSQL *db, char* search_datetime);
    MYSQL_RES *getSalesRouteWeekly(MYSQL *db, char* search_datetime);
        
        
    float getSumOfFR(MYSQL *db, int sales_route_id, int seat_class);
    int getSumOfSeatScore(MYSQL *db, int user_route_id);
    int getMinStopOver(MYSQL *db, int physical_route_id);
    int getRouteFrequency(MYSQL *db, int airline_id, int physical_route_id);
    int getRemainedPassenger(MYSQL *db, int physical_route_id);
    bool setSalesScore(MYSQL *db, int sales_route_id, float r1, float r2, float r3, float r4, float fr);
    bool submitSalesHistory(MYSQL *db, unsigned long int sales_route_id, int num_of_passenger, float load_factor, int week_date, char *game_datetime);
    bool submitSalesHistoryWeekly(MYSQL *db, unsigned long int sales_route_id, int num_of_passenger, float load_factor, int week_number, char *submit_datetime);
    bool submitSalesHistoryMonthly(MYSQL *db, unsigned long int sales_route_id, int num_of_passenger, float load_factor, int month, char *submit_datetime);
    bool setPassengerData(MYSQL *db, int sales_route_id, int passenger);
    bool removeRouteQuery(MYSQL *db, int physical_route_id);
    bool removeSalesRouteHistoryDaily(MYSQL *db, char* search_datetime);
    bool removeSalesRouteHistoryWeekly(MYSQL *db, char* search_datetime);
    
    float getCorrectedFrequency(MYSQL *db, unsigned long airline_id, int physical_route_id, int MSM);


#ifdef	__cplusplus
}
#endif

#endif	/* ROUTE_DATA_H */

