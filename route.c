#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mysql/mysql.h>

#include "route_data.h"
#include "route_process.h"
#include "income_statement_data.h"

#define FUEL_PRICE 400

//Seat Class
//0 => Y and Y+
//1 => J and F
void scoreProcessing(MYSQL* db,int seat_class){
    //For Route Process Use
    int  passenger_number = 0;
    float price_index = 0;
    float dep_index = 0;
    float arr_index = 0;
    float seat_area_index = 0;
    float r1 = 0;
    float r2 = 0;
    float r3 = 0;
    float r4 = 0;
    float fr = 0;
    float fr_v2 = 0;
    float route_distance_index = 0;
    float sum_of_fr = 0;
    float seat_comfort_score = 0;
    float comfort_index = 0;
    float reputation_index = 0;
    float catering_index = 0;
    float msm = 0;
        
    //For Route Data Use
    int sales_route_id = 0;
    int user_route_id = 0;
    int airline_id = 0;
    float default_price = 0;
    int distance = 0;
    int passenger_demand = 0;
    int price = 0;
    int stopover = 0;
    int cater_price = 0;
    float cater_quality = 0;
    float quality = 0;
    float popularity = 0; 
    int dep_time = 0;
    int arr_time = 0;
    float seat_length = 0;
    float seat_width = 0;
    int route_type_id = 0;
    int physical_route_id = 0;
    int seat_number = 0;
    int timezone_a = 0;
    int timezone_b = 0;
    float penalty = 0;
    float frequency_bouns = 0;
    int frequency = 0;
    int allocated_passenger = 0;
    int remained_passenger = 0;
    int passenger_cnt = 0;
    int allocation_check = 0;
    
    MYSQL_RES *query_ptr;
    MYSQL_RES *route_ptr;
    MYSQL_ROW query_row;
    MYSQL_ROW route_row;
    
    
    query_ptr = getRouteProcessQuery(db);
    if(query_ptr){
        printf("Route Process Query => %d rows received\n", mysql_num_rows(query_ptr));
        while(query_row = mysql_fetch_row(query_ptr)){
            //0 => Physical Route ID
            route_ptr = getRouteDataByPhysicalRouteID(db, atoi(query_row[0]), seat_class);
            //printf("MYSQL ERROR[%d]\n", mysql_errno(db));
            passenger_cnt = 0;
            allocated_passenger = 0;
            printf("ID [%s]\n", query_row[0]);
            if(route_ptr){ 
                    while(route_row = mysql_fetch_row(route_ptr)){
                        //0 => Sales Route ID, 1 => User Route ID, 2 => Airline ID, 3 => Default Price Index
                        //4 => Distance, 5 => Passenger Demand Y, 6=> Passenger Demand JF, 7 => Price, 8 => Stopover
                        //9 => CaterPrice
                        //10 => CaterQuality, 11 => Quality, 12 => Popularity, 13 => Departure Time, 14 => Arrival Time
                        //15 => Seat Length, 16 => Seat Width, 17 => Route Type ID, 18 => Physical Route ID
                        //19 => Seat Number, 20 => Seat Score, 21 => Airport A Time zone, 22 => Airport B Time zone
                        //Setup and get all data
                        sales_route_id = atoi(route_row[0]);
                        user_route_id = atoi(route_row[1]);
                        airline_id = atoi(route_row[2]);
                        default_price = atof(route_row[3]);
                        distance = atoi(route_row[4]);
                        if(seat_class == 0){
                            passenger_demand = atoi(route_row[5]);
                        }else{
                            passenger_demand = atoi (route_row[6]);
                        }
                        price = atof(route_row[7]);
                        stopover = atoi(route_row[8]);
                        cater_price = atoi(route_row[9]);
                        cater_quality = atoi(route_row[10]);
                        quality = atof(route_row[11]);
                        popularity = atof(route_row[12]);
                        dep_time = atoi(route_row[13]);
                        arr_time = atoi(route_row[14]);
                        seat_length = atoi(route_row[15]);
                        seat_width = atoi(route_row[16]);
                        route_type_id = atoi(route_row[17]);
                        physical_route_id = atoi(route_row[18]);
                        seat_number = atoi(route_row[19]);
                        seat_comfort_score = atoi(route_row[20]);
                        timezone_a = atoi(route_row[21]);
                        timezone_b = atoi(route_row[22]);
                        //printf("Entered - Point A 1\n");
                        
                        //printf("Entered - Point A 2\n");
                        frequency = getRouteFrequency(db, airline_id, physical_route_id);
                    
                    
                        //printf("Entered - Point B\n");
                    
                        //Process the index and score
                        reputation_index = reputation_score_indexer(popularity, quality);
                        //printf("Reputation index - pass\n"); 
                        price_index = price_indexer(default_price, price);
                        //printf("price index - pass\n"); 
                        dep_index = departure_time_indexer(dep_time, timezone_a, route_type_id);
                        //printf("Departure index - In [%d][%d] - Out [%f]\n",dep_time , timezone_a, dep_index); 
                        arr_index = arrival_time_indexer(arr_time, timezone_b, route_type_id);
                        //printf("Arrival index - In [%d][%d] - Out [%f]\n", arr_time, timezone_b, arr_index); 
                        seat_area_index = seat_area_indexer(seat_length, seat_width);
                        //printf("Seat area index - pass\n");  
                        comfort_index = seat_comfort_corrector(seat_area_index, seat_comfort_score);
                        //printf("Comfort index - pass\n"); 
                        catering_index = catering_indexer(cater_price, cater_quality);
                        //printf("Catering index - pass\n");
                        route_distance_index = route_distance_indexer(distance);
                        //printf("Rote distance index - IN [%d] OUT [%f]\n", distance, route_distance_index);
                        penalty = stop_penalty_corrector(stopover, getMinStopOver(db, physical_route_id));
                        //printf("Penalty of stopover - pass\n");
                        frequency_bouns = frequency_effect_processor(route_distance_index);
                        //printf("Rote distance index - IN [%f] OUT [%f]\n",  route_distance_index, frequency_bouns);
                        
                        msm = minimum_separation_minutes(distance);
                        
                        r1 = dep_arr_time_processor(dep_index, arr_index);
                        //printf("R1 - IN [%f] [%f] OUT [%f]\n", dep_index, arr_index, r1);
                        r2 = price_reputation_processor(price_index, reputation_index);
                        //printf("R2 - IN [%f] [%f] OUT [%f]\n", price_index, reputation_index, r2);
                        r3 = price_comfort_indexer(comfort_index, price_index);
                        //printf("R3 - IN [%f] [%f] OUT [%f]\n", comfort_index, price_index, r3);
                        r4 = chief_processor(r1, r2, r3);
                        //printf("R4 - IN [%f] [%f] [%f] OUT [%f]\n", r1, r2, r3, r4);
                        fr = global_correction(r4, penalty);
                        //printf("FR - IN [%f] [%f] OUT [%f]\n", r4, penalty, fr);
                        fr_v2 = fr_correction(fr, frequency_bouns, frequency);
                        //printf("FR correction - IN [%f] [%f] [%d] OUT [%f]\n", fr, frequency_bouns, frequency, fr_v2);              
                        
                        
                    
                        //Print out all score and index for debug
                        /*
                        printf("================[Salse Route - %d]===============\n", sales_route_id);
                        printf("Reputation Index\t\t[%f]\n", reputation_index);
                        printf("Price Indexer\t\t\t[%f]\n", price_index);
                        printf("Catering Indexer\t\t[%f]\n", catering_indexer(cater_price, cater_quality));
                        printf("Seat Area Indexer\t\t[%f]\n", seat_area_index);
                        printf("Seat Comfort Score\t\t[%f]\n", seat_comfort_score);
                        printf("Seat Comfort Indexer\t\t[%f]\n", comfort_index);
                        printf("Route Distance Indexer\t\t[%f]\n", route_distance_index);
                        printf("Stopover penalty\t\t[%f]\n", penalty);
                        printf("Route Frequency\t\t\t[%d]\n", frequency);
                        printf("Route Distance\t\t\t[%d]\n", distance);
                        printf("Frequency Bouns\t\t\t[%f]\n", frequency_bouns);
                        printf("R1 - DEP_ARR_TIME Processor\t[%f]\n", r1);
                        printf("R2 - Price Reputation Processor\t[%f]\n", r2);
                        printf("R3 - Price Comfort Processor\t[%f]\n", r3);
                        printf("R4 - Chief Processor\t\t[%f]\n", r4);
                        printf("FR - Global Correction\t\t[%f]\n", fr);
                        printf("FR - FR Correction\t\t[%f]\n\n", fr_v2);
                        printf("Demand Got [%f]\n", (fr_v2 / sum_of_fr) * passenger_demand);
                        */
                
                        //Update the score
                        if(setSalesScore(db, sales_route_id, r1, r2, r3, r4, fr_v2)){
                            printf("Update score data - Sales Route => Class %d [OK]\n", seat_class );
                        }else{
                            printf("Update score data - Sales Route => Class %d [Fail]\n", seat_class );
                        }
                        
                        
                    
                    }
                    
                    //Release the memory
                    mysql_free_result(route_ptr);
                    route_ptr = getSalesRouteDataByPhysicalRouteID(db, atoi(query_row[0]), seat_class);
                    
                    while(route_row = mysql_fetch_row(route_ptr)){
                        //0 => Sales Route ID, 1 => FR, 2 => Passenger, 3 => Seat Number
                        
                        sales_route_id = atoi(route_row[0]);
                        fr = atof(route_row[1]);
                        passenger_number = atoi(route_row[2]);
                        seat_number = atoi(route_row[3]);
                        if(!seat_class){
                            passenger_demand = atoi(route_row[4]);
                        }else{
                            passenger_demand = atoi(route_row[5]);
                        }
                        
                        sum_of_fr = getSumOfFR(db, atoi(query_row[0]), seat_class);
                        //printf("SUM of FR - OUT [%f]\n", sum_of_fr);
                        
                        passenger_number = passenger_distribution(fr, sum_of_fr, passenger_demand);
                        if(passenger_number <= 0){
                            printf("Passenger number - IN [%f] [%f] [%d] OUT [%d]\n", fr, sum_of_fr, passenger_demand, passenger_number);
                            passenger_number = 0;
                        }
                        allocated_passenger = passenger_calculation((fr / sum_of_fr), passenger_demand, seat_number);
                        //printf("Allocated passenger number - IN [%f] [%d] [%d] OUT [%d]\n", (fr_v2 / sum_of_fr), passenger_demand, seat_number, allocated_passenger);
                        passenger_cnt += allocated_passenger;
                        //printf("Passenger Count [%d]- pass\n", passenger_cnt);
                    
                        //Update the passenger data
                        if(setPassengerData(db, sales_route_id, allocated_passenger)){
                            printf("Update passenger data - Sales Route => Class %d [OK]\n", seat_class );
                        }else{
                            printf("Update passenger data - Sales Route => Class %d [Fail]\n", seat_class );
                        }
                    
                        //printf("Sum of FR \t\t\t[%f]\n", sum_of_fr);
                        //printf("Passenger Number => %d (%3.2f%)\n", passenger_number, (fr / sum_of_fr) * 100);
                        //printf("Allocated Passenger Number\t\t[%d]\n", allocated_passenger);
                    
                    
                        //Check is any routes didn't got the 100% load factor
                        if(allocated_passenger != seat_number){
                            allocation_check++;
                        }
                    }
                    
                    //Calculate the remained passenger
                    remained_passenger = passenger_demand - passenger_cnt;
                    //printf("Passenger Demand [%d] - Passenger Cnt [%d]\n", passenger_demand, passenger_cnt);
                
                    //Release the memory
                    mysql_free_result(route_ptr);
                
                    //Passenger allocation
                    if(allocation_check > 0){
                        //remained_passenger = rd->getRemainedPassenger(db, atoi(query_row[0]));
                        if(remained_passenger > 0){
                            //printf("Allocation Remained Passenger [%d]\n", remained_passenger);
                            route_ptr = getSalesRouteDataByPhysicalRouteID(db, atoi(query_row[0]), seat_class);
                            if(route_ptr){
                                while(route_row = mysql_fetch_row(route_ptr)){
                                    //0 => Sales Route ID, 1 => FR, 2 => Passenger, 3 => Seat Number
                                    if(atoi(route_row[2]) != atoi(route_row[3])){
                                        //Allocate the remained passenger
                                        allocated_passenger = atoi(route_row[2]) + remain_passenger_allocation(remained_passenger, atof(route_row[1]), sum_of_fr);
                                        //printf("Final allocated Passenger Number\t\t[%d]\n", allocated_passenger);
                                        if(allocated_passenger > atoi(route_row[3])){
                                            allocated_passenger = atoi(route_row[3]);
                                        }

                                        //Update the sales route allocated passenger
                                        setPassengerData(db, atoi(route_row[0]), allocated_passenger);
                                    }
                                }
                                mysql_free_result(route_ptr);
                            }
                            //printf("Remained Passenger allocated\n");
                        }
                    }
                }
            }
            //Release the memory
            mysql_free_result(query_ptr);
            
            if(removeRouteQuery(db, physical_route_id)){
                printf("Route Query Removed\n");
            }else{
                printf("MYSQL ERROR [%d]\n", mysql_errno(db));
            }
            
        }else{
            printf("No route process in query\n");
        }
    //printf("Score process Error check - MYSQL[%d]\n", mysql_errno(db));
}

void ticketIncomeProcessing(MYSQL* db, int weekdate, int year, int month, int day, int hour, int minute){
    MYSQL_RES *ptr;
    MYSQL_RES *airline_ptr;
    MYSQL_ROW airline_row;
    MYSQL_ROW route_row;
    
    char *buf = (char*)malloc(sizeof(char) * 60);
    int income = 0;
    int fuel_cost = 0;
    int gametime = 0;
    float random_number = 0;
    long long int cash = 0;
    float load_factor = 0;
    
    unsigned long int sales_route_id = 0;
    int price = 0;
    int passenger = 0;
    int airline_id = 0;
    int airport_id = 0;
    int seat_number = 0;
    int basal_fw = 0;
    float additional_fw_unit = 0;
    int seat_class_type_id = 0;
    int default_price = 0;
    int transact_id = 0;
    
    //Time corrector
    //time = minute % 5;
    //time = minute - time;
    gametime = minute;
    gametime += (hour * 100);
    
    //printf("Ticket Income process - A\n");
    
    ptr = getSalesRouteDataByDepartureTime(db, weekdate, gametime);
    //printf("Ticket Income process - [%d] [%d] [%d]\n", db, weekdate, gametime);
    
    sprintf(buf, "%d-%d-%d %d:%d:0", year, month, day, hour, minute);
    
    //printf("MYSQL ERROR [%d]\n", mysql_errno(db));
    
    if(ptr){
        //printf("Ticket Income process - Start [%lld]\n", ptr);
        //route_row = mysql_fetch_row(ptr);
        //printf("Ticket Income process - B [%d]\n", mysql_num_rows(ptr));
        while(route_row = mysql_fetch_row(ptr)){
            //Sales Route Data : 
            //0 => Sales Route ID, 1 => Price, 2 => Passenger
            //3 => Airline ID, 4 => Airport ID, 5 => Seat Number
            //6 => Basal FW, 7 => Additional FW Unit, 8 => Seat Class Type ID
            //9 => Default Price
            
            //Seat Class Type ID :
            //0 => Y class
            //1 => Y+ class
            //2 => J class
            //3 => F class 
            
            sales_route_id = strtoul(route_row[0], NULL, 0);
            price = atoi(route_row[1]);
            passenger = atoi(route_row[2]);
            airline_id = atoi(route_row[3]);
            airport_id = atoi(route_row[4]);
            seat_number = atoi(route_row[5]);
            basal_fw = atoi(route_row[6]);
            additional_fw_unit = atof(route_row[7]);
            seat_class_type_id = atoi(route_row[8]);
            default_price = atoi(route_row[9]);
            
            //Random the passenger number by +-5%
            srand(time(NULL));
            random_number = ((rand() % 100) - 50) / 1000.0;
                                    
            fuel_cost = FUEL_PRICE * ((basal_fw + (((passenger * 80) / 1000) * additional_fw_unit)) / 1000);
            printf("Fuel Cost => [%d]\n", fuel_cost);
            
            passenger = passenger + (passenger * random_number);
            printf("Passenger Number Random - [%d] % - [%f]\n", passenger, random_number);
            
            // Passenger Number Correction
            if(passenger > seat_number){
                passenger = seat_number;
            }
            printf("Passenger Number After correction - [%d]\n", passenger);
            
            //
            if(price >= (default_price * 1.5)){
                passenger = passenger * ((default_price * 1.5) / price);
            }
            
            
            load_factor = ((float)passenger / (float)seat_number) * 100;
            
            //Calculate the ticket income
            income = price * passenger;
                
            
                
            //Update the ticket income of the Sales Route
            if(submitSalesHistory(db, sales_route_id, passenger, load_factor, weekdate, buf)){
                printf("SalesHistory Update - [OK]\n");
            }else{
                printf("SalesHistory Update - [Fail]\n");
            }
            //printf("MYSQL ERROR - [%d]\n", mysql_errno(db));
                
            
            airline_ptr = getAirlineInfo(db, airline_id);
            if(airline_ptr){
                airline_row = mysql_fetch_row(airline_ptr);
                cash = atoll(airline_row[1]);
                cash += (income - fuel_cost);
                
                //Update Airline Cash
                if(updateAirlineCash(db, atoi(airline_row[0]), cash)){
                    printf("Updated Airline Cash\n");
                }else{
                    printf("MYSQL ERROR [%d]\n", mysql_errno(db));
                }
                
                //Update the income statement of the user
                //Transact ID
                //1 => Ticket Income - Y Class
                //2 => Ticket Income - J Class
                //3 => Ticket Income - F Class
                //4 => Fuel Cost
                
                switch(seat_class_type_id){
                    case 1:
                        transact_id = 1;
                        break;
                    case 2:
                        transact_id = 0;
                        break;
                    case 3:
                        transact_id = 2;
                        break;
                    case 4:
                        transact_id = 3;
                        break;
                    default:
                        transact_id = 0;
                }
                
                
                if(submitIncomStatementData(db, transact_id, airline_id, airport_id, income, weekdate, buf)){
                    printf("Updated Income Statement\n");
                }else{
                    printf("MYSQL ERROR [%d]\n", mysql_errno(db));
                }
                
                if(submitIncomStatementData(db, 4, airline_id, airport_id, (fuel_cost * -1), weekdate, buf)){
                    printf("Updated Income Statement\n");
                }else{
                    printf("MYSQL ERROR [%d]\n", mysql_errno(db));
                }
                mysql_free_result(airline_ptr);
            }
             
        }
        mysql_free_result(ptr);
    }
    free(buf);
    
}