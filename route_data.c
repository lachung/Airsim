#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mysql/mysql.h>

#include "route_data.h"
#include "database.h"


//Un-used
MYSQL_RES *getAllRouteData(MYSQL *db){
    MYSQL_RES *ptr;
    char *buf;
    buf = (char*)malloc(1200);
    sprintf(buf, "SELECT `Sales Route`.`Sales Route ID`, `User Route`.`User Route ID`, `User Route`.`Airline ID`, `Physical Route`.`Default Price Index`, `Physical Route`.`Distance`, `Physical Route`.`Passenger Demand`, `Sales Route R-Score`.`Price`, `Sales Route R-Score`.`Stopover`, `OpsRoute Catering`.`CaterPrice`, `OpsRoute Catering`.`CaterQuality`, `Airline Profile`.`Quality`, `Airline Profile`.`Popularity`, `OpsRoute Time Profile`.`Departure Time`, `OpsRoute Time Profile`.`Arrival Time`, `User Seat Config Class`.`Seat Length`, `Abreast`.`Seat Width`, `Physical Route`.`Route Type ID`, `Physical Route`.`Physical Route ID` FROM `User Route`, `Sales Route`, `Physical Route`, `Sales Route R-Score`, `Airline Profile`, `OpsRoute Catering`, `OpsRoute Time Profile`, `User Seat Config Class`, `Abreast` WHERE `User Route`.`User Route ID` = `Sales Route`.`User Route ID` and `Sales Route`.`Route ID` = `Physical Route`.`Physical Route ID` and `Sales Route`.`Sales Route ID` = `Sales Route R-Score`.`Sales Route ID` and `User Route`.`Airline ID` = `Airline Profile`.`Airline ID` GROUP BY(`Sales Route ID`)");
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

//Completed, un-check
MYSQL_RES *getRouteDataByPhysicalRouteID(MYSQL* db, int physical_route_id, int seat_class){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 5000);
    
    char *buf_str = (char*)malloc(sizeof(char) * 500);
    if(!seat_class){
        sprintf(buf_str, "(user_seat_config_class.seat_class_type_id_id = 1 OR user_seat_config_class.seat_class_type_id_id = 2)");
    }else{
        sprintf(buf_str, "(user_seat_config_class.seat_class_type_id_id = 3 OR user_seat_config_class.seat_class_type_id_id = 4)");
    }
    sprintf(buf, "SELECT sales_route.id, user_route.id, user_route.airline_id_id, physical_route.default_price, physical_route.distance, physical_route.demand_y, physical_route.demand_jf, sales_route.price, sales_route.stopover, ops_route.catering_price, ops_route.catering_quality, airline.quality, airline_popularity.popularity, sales_route.departure_time, sales_route.arrival_time, user_seat_config_class.seat_length, abreast.seat_width, physical_route.route_type_id_id, sales_route.physical_route_id_id, user_seat_config_class.seat_number, user_seat_config_class.score, (select timezone from airport where id = physical_route.airport_id_a_id) AS timezone_a, (select timezone from airport where id = physical_route.airport_id_b_id) AS timezone_b FROM sales_route, user_route, physical_route, ops_route, airline, airline_popularity, airport, country, user_seat_config_class, abreast WHERE sales_route.user_route_id_id = user_route.id and user_route.airline_id_id = airline.id and sales_route.physical_route_id_id = physical_route.id and sales_route.user_route_id_id = ops_route.user_route_id_id and sales_route.physical_route_id_id = ops_route.physical_route_id_id and physical_route.airport_id_a_id = airport.id and airline_popularity.airline_id_id = user_route.airline_id_id and airline_popularity.country_id_id = airport.country_id_id and sales_route.user_seat_class_id_id = user_seat_config_class.id and user_seat_config_class.abreast_id_id = abreast.id and sales_route.physical_route_id_id = %d AND %s GROUP BY(sales_route.id)", physical_route_id, buf_str);
    ptr = getDataFromDB(db, buf);
    free(buf);
    free(buf_str);
    return ptr;
}



//Completed, un-check
MYSQL_RES *getSalesRouteDataByPhysicalRouteID(MYSQL* db, int physical_route_id, int seat_class){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 500);
     char *buf_str = (char*)malloc(sizeof(char) * 500);
    if(!seat_class){
        sprintf(buf_str, "(user_seat_config_class.seat_class_type_id_id = 1 OR user_seat_config_class.seat_class_type_id_id = 2)");
    }else{
        sprintf(buf_str, "(user_seat_config_class.seat_class_type_id_id = 3 OR user_seat_config_class.seat_class_type_id_id = 4)");
    }
    sprintf(buf, "SELECT sales_route.id, sales_route.fr, sales_route.passenger, user_seat_config_class.seat_number, physical_route.demand_y, physical_route.demand_jf FROM sales_route, user_seat_config_class, physical_route WHERE sales_route.user_seat_class_id_id = user_seat_config_class.id and sales_route.physical_route_id_id = %d AND sales_route.physical_route_id_id = physical_route.id AND %s", physical_route_id, buf_str);
    ptr = getDataFromDB(db, buf);
    free(buf);
    free(buf_str);
    return ptr;
}
  
//Completed, un-check
MYSQL_RES *getRouteProcessQuery(MYSQL* db){ 
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 100);
    sprintf(buf, "SELECT physical_route_id_id FROM route_process_query LIMIT 1");
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

//Completed, un-check
MYSQL_RES *getSalesRouteDataByDepartureTime(MYSQL* db, int departure_weekdate, int departure_time){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 1000);
    sprintf(buf, "SELECT sales_route.id, sales_route.price, sales_route.passenger, user_route.airline_id_id, airline.airport_id_id, user_seat_config_class.seat_number, ops_route.basal_fw, ops_route.additional_fw_unit, user_seat_config_class.seat_class_type_id_id, physical_route.default_price FROM sales_route, user_route, airline, user_seat_config_class, ops_route, physical_route WHERE sales_route.user_route_id_id = user_route.id AND ops_route.user_route_id_id = sales_route.user_route_id_id AND ops_route.physical_route_id_id = sales_route.physical_route_id_id AND user_route.airline_id_id = airline.id AND sales_route.user_seat_class_id_id = user_seat_config_class.id AND sales_route.departure_week_date = %d AND sales_route.departure_time = %d AND sales_route.physical_route_id_id = physical_route.id AND user_route.user_aircraft_id_id > 0 GROUP BY sales_route.id", departure_weekdate, departure_time);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

MYSQL_RES *getAirlineInfo(MYSQL* db, int airline_id){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "SELECT id, cash FROM airline WHERE id = %d", airline_id);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

MYSQL_RES *getSalesRouteDaily(MYSQL* db, char* search_datetime){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "SELECT sales_route_id_id, SUM(pax), (SUM(load_factor) / COUNT(load_factor)) FROM sales_route_history_daily WHERE ts_created < '%s' GROUP BY sales_route_id_id", search_datetime);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

MYSQL_RES *getSalesRouteWeekly(MYSQL* db, char* search_datetime){
    MYSQL_RES *ptr;
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "SELECT sales_route_id_id, SUM(pax), (SUM(load_factor) / COUNT(load_factor)) FROM sales_route_history_weekly WHERE ts_created < '%s' GROUP BY sales_route_id_id", search_datetime);
    ptr = getDataFromDB(db, buf);
    free(buf);
    return ptr;
}

//Completed, un-check
float getSumOfFR(MYSQL* db, int sales_route_id, int seat_class){
    MYSQL_RES *temp_ptr;
    MYSQL_ROW sqlrow;
    char *buf;
    int result = 0;
    int seat_type_a, seat_type_b;
    if(seat_class == 0){
        seat_type_a = 1;
        seat_type_b = 2;
    }else{
        seat_type_a = 3;
        seat_type_b = 4;
    }
    buf = (char*)malloc(1000);
    sprintf(buf, "SELECT sales_route.physical_route_id_id, SUM(sales_route.fr) as sum_of_fr FROM sales_route, user_seat_config_class WHERE sales_route.physical_route_id_id = %d AND sales_route.user_seat_class_id_id = user_seat_config_class.id AND (user_seat_config_class.seat_class_type_id_id = %d OR user_seat_config_class.seat_class_type_id_id = %d)", sales_route_id, seat_type_a, seat_type_b);
    if(!mysql_query(db, buf)){
        temp_ptr = mysql_store_result(db);
        if(temp_ptr){
            sqlrow = mysql_fetch_row(temp_ptr);
            result = atoi(sqlrow[1]);
            mysql_free_result(temp_ptr);
        }
    }
    free(buf);
    return result;
}

int getSumOfSeatScore(MYSQL* db, int user_route_id){
    MYSQL_RES *temp_ptr;
    MYSQL_ROW sqlrow;
    char *buf;
    buf = (char*)malloc(1200);
    int result = 0;
    //Get Sum of Seat Score
    //0 => User Route ID, 1 => User Seat Class ID, 2 => User Seat ID
    //3 => sum_of_positive, 4 => sum_of_negative
    sprintf(buf, "SELECT user_route.id, user_seat_config_class.id, user_seat_config_class.user_seat_id_id, SUM(cabin_equipment_type.positive_effect), SUM(cabin_equipment_type.negative_effect) FROM user_cabin_equipment, user_seat_config_class, cabin_equipment_type, user_route, user_aircraft WHERE user_cabin_equipment.user_seat_class_id_id = user_seat_config_class.id and user_cabin_equipment.type_id_id = cabin_equipment_type.id and user_aircraft.user_seat_id_id = user_seat_config_class.user_seat_id_id and user_route.user_aircraft_id_id = user_aircraft.id and user_route.user_route_id = %d", user_route_id);
    if(!mysql_query(db, buf)){
        temp_ptr = mysql_store_result(db);
        if(temp_ptr){
            sqlrow = mysql_fetch_row(temp_ptr);
            result = atoi(sqlrow[3]);
            mysql_free_result(temp_ptr);
        }
    }

    free(buf);
    return result;
}

int getMinStopOver(MYSQL* db, int physical_route_id){
    MYSQL_RES *temp_ptr;
    MYSQL_ROW sqlrow;
    char *buf;
    int result = 0;
    //Get the minimum stopover of an route using physical route id
    //0 => Physical Route ID, 1 => Minimum Stopover
    buf = (char*)malloc(350);
    sprintf(buf, "SELECT physical_route.id, MIN(sales_route.stopover) AS min_stop FROM sales_route, physical_route WHERE sales_route.physical_route_id_id = physical_route.id and sales_route.physical_route_id_id = %d", physical_route_id);
    if(!mysql_query(db, buf)){
        temp_ptr = mysql_store_result(db);
        if(temp_ptr){
            sqlrow = mysql_fetch_row(temp_ptr);
            result = atoi(sqlrow[1]);
            mysql_free_result(temp_ptr);
        }
    }
    free(buf);
    return result;
}

int getRouteFrequency(MYSQL* db, int airline_id, int physical_route_id){
    MYSQL_RES *temp_ptr;
    MYSQL_ROW sqlrow;
    char *buf;
    int result = 0;
    //Get the route frequency using Airline ID & Physical Route ID
    //0 => Airline ID, 1 => Frequency
    buf = (char*)malloc(300);
    sprintf(buf, "SELECT sales_route.airline_id_id, COUNT(sales_route.airline_id_id) as frequency FROM sales_route, physical_route WHERE sales_route.physical_route_id_id = physical_route.id and physical_route.id = %d and sales_route.airline_id_id = %d", physical_route_id, airline_id);
    if(!mysql_query(db, buf)){
        temp_ptr = mysql_store_result(db);
        if(temp_ptr){
            sqlrow = mysql_fetch_row(temp_ptr);
            result = atoi(sqlrow[1]);
            mysql_free_result(temp_ptr);
        }
    }
    free(buf);
    return result;
}

bool setSalesScore(MYSQL *db, int sales_route_id, float r1, float r2, float r3, float r4, float fr){
    char *buf;
    buf = (char*)malloc(500);
    sprintf(buf, "UPDATE sales_route SET r1 = '%f', r2 = '%f', r3 = '%f', r4 = '%f', fr = '%f' WHERE id = %d", r1, r2, r3, r4, fr, sales_route_id);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool submitSalesHistory(MYSQL* db, unsigned long int sales_route_id, int num_of_passenger, float load_factor, int week_date, char* submit_datetime){
    char *buf;
    buf = (char*)malloc(300);
    sprintf(buf, "INSERT INTO sales_route_history_daily VALUES (NULL, %lu, %d, %f, %d, '%s', NULL)", sales_route_id, num_of_passenger, load_factor, week_date, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool submitSalesHistoryWeekly(MYSQL* db, unsigned long int sales_route_id, int num_of_passenger, float load_factor, int week_number, char* submit_datetime){
    char *buf;
    buf = (char*)malloc(300);
    sprintf(buf, "INSERT INTO sales_route_history_weekly VALUES (NULL, %lu, %d, %f, %d, '%s', NULL)", sales_route_id, num_of_passenger, load_factor, week_number, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool submitSalesHistoryMonthly(MYSQL* db, unsigned long int sales_route_id, int num_of_passenger, float load_factor, int month, char* submit_datetime){
    char *buf;
    buf = (char*)malloc(300);
    sprintf(buf, "INSERT INTO sales_route_history_monthly VALUES (NULL, %lu, %d, %f, %d, '%s', NULL)", sales_route_id, num_of_passenger, load_factor, month, submit_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool setPassengerData(MYSQL* db, int sales_route_id, int passenger){
    char *buf;
    buf = (char*)malloc(400);
    sprintf(buf, "UPDATE sales_route SET passenger = %d WHERE id = %d", passenger, sales_route_id);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeRouteQuery(MYSQL* db, int physical_route_id){
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "DELETE FROM route_process_query WHERE physical_route_id_id = %d", physical_route_id);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeSalesRouteHistoryDaily(MYSQL* db, char* search_datetime){
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "DELETE FROM sales_route_history_daily WHERE ts_created < '%s'", search_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

bool removeSalesRouteHistoryWeekly(MYSQL* db, char* search_datetime){
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "DELETE FROM sales_route_history_weekly WHERE ts_created <= '%s'", search_datetime);
    if(!mysql_query(db, buf)){
        free(buf);
        return true;
    }else{
        free(buf);
        return false;
    }
}

float getCorrectedFrequency(MYSQL* db, unsigned long airline_id, int physical_route_id, int MSM){
    MYSQL_RES *ptr;
    MYSQL_ROW  sqlrow;
    bool isFirst = true;
    int first_departure_time = 0;
    int first_departure_date = 0;
    int departure_time = 0;
    int departure_date = 0;
    int separation_time = 0;
    float frequency = 0;
    
    char *buf = (char*)malloc(sizeof(char) * 200);
    sprintf(buf, "SELECT * FROM sales_route where airline_id_id = %ld AND physical_route_id_id = %ld GROUP BY departure_time , departure_week_date ORDER BY departure_week_date, departure_time DESC", airline_id, physical_route_id);
    
    ptr = getDataFromDB(db, buf);
    
    if(ptr){
        while(sqlrow = mysql_fetch_row(ptr)){
            if(isFirst){
                first_departure_time = atoi(sqlrow[0]);
                first_departure_date = atoi(sqlrow[1]);
                departure_time = first_departure_time;
                departure_date = first_departure_date;
            }else{
                if(departure_date == atoi(sqlrow[1])){
                    separation_time = departure_time - atoi(sqlrow[0]);
                    if((separation_time / MSM) > 1){
                        frequency += 1;
                    }else{
                        frequency += separation_time / MSM;
                    }
                            
                }else{
                    if((departure_time - atoi(sqlrow[0])) > 0){
                        separation_time = departure_time - atoi(sqlrow[0]);
                        if((separation_time / MSM) > 1){
                            frequency += 1;
                        }else{
                            frequency += separation_time / MSM;
                        }
                        
                    }else{
                        frequency += 1;
                    }
                }
            }
        }
        
        
        
        mysql_free_result(ptr);
    }
    return frequency;
}