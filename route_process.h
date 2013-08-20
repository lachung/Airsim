/* 
 * File:   route_process.h
 * Author: chung
 *
 * Created on August 12, 2013, 2:12 PM
 */

#ifndef ROUTE_PROCESS_H
#define	ROUTE_PROCESS_H

#ifdef	__cplusplus
extern "C" {
#endif

    //Upstream indexer
    float departure_time_indexer(int departure_time, int timezone, int route_type);
    float arrival_time_indexer(int arrival_time, int timezone, int route_type);

    float reputation_score_indexer(float popularity, float quality);
    float price_indexer(float default_price, float route_price);
    float catering_indexer(int price, int catering_quality);
    float seat_area_indexer(int pitch, float width);
    float seat_comfort_corrector(int seat_score_1, float seat_score_2);
    
    //Central Processor
    float comfort_indexer(int seat_comfort, int catering_score);
    //R1
    float dep_arr_time_processor(int departure_time_index, int arrival_time_index);
    //R2
    float price_reputation_processor(int price, int reputation);
    //R3
    float price_comfort_indexer(float comfort, int price);
    //R4
    float chief_processor(float r1, float r2, float r3);
    //R4 Corrector
    float r4_corrector(float r4, float duration_time_index);

    //Downstream Corrector
    float route_distance_indexer(int route_distance);
    float frequency_effect_processor(float route_distance_index);
    float duration_time_indexer(int average_time, int route_time);
    int route_time_calculate(int dep_time, int arr_time); 
    float minimun_separation_minutes(int route_distance);
    //FR
    float stop_penalty_corrector(int stopover, int min_stop);
    float global_correction(float r4, float penalty);
    float fr_correction(float fr, float frequency_bonus, int frequency);
    int passenger_distribution(float fr, float sum_of_fr, int route_demand);
        
    //Passenger allocation
    int passenger_calculation(float allocated_percentage, int route_demand, int aircraft_capacity);
    int remain_passenger_allocation(int remain_passenger, float fr, float sum_of_fr);


#ifdef	__cplusplus
}
#endif

#endif	/* ROUTE_PROCESS_H */

