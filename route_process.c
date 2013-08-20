#include <stdlib.h>
#include <math.h>
#include "route_process.h"
  
float departure_time_indexer(int departure_time, int timezone, int route_type){
    //Route Type
    //0 => Regional, 1 => Long-Haul, 2 => Short-Haul 
    float result = 0;
    static const float regional_index[] = {1, 1, 1, 1, 2.166667, 8, 9.5, 10, 10, 9.166667, 8.333333, 8.5, 8.666667, 7.666667, 7.5, 7.5, 8, 8.333333, 7.666667, 7, 6.333333, 4.5, 3.5, 1.67};
    static const float shortH_index[] = {2.333333, 1.5, 1, 1, 2.166667, 7, 8.5, 10, 10, 9.166667, 8.333333, 8.5, 8.666667, 7.666667, 7.5, 7.5, 8, 8.333333, 7.666667, 7, 6.333333, 5.666667, 4, 2.666667};
    static const float longH_index[] = {2.333333, 1.5, 1, 1, 2.166667, 8.5, 9.3, 10, 10, 9.166667, 8.333333, 8.5, 8.666667, 7.666667, 7.5, 7.5, 8, 8.333333, 7.666667, 7, 6.333333, 5.666667, 4, 2.666667};
    
    departure_time += (timezone * 100);
    if(departure_time >= 2400){
        departure_time = (departure_time % 2400);
    }
    
    switch(route_type - 1){
        case 0:
            result = regional_index[(departure_time / 100)];
            return result;
        case 1:
            result = longH_index[(departure_time / 100)];
            return result;
        case 2:
            result = shortH_index[(departure_time / 100)];
            return result;
        default :
            return 0;
    }
}

float arrival_time_indexer(int arrival_time, int timezone, int route_type){
    //Route Type
    //0 => Regional, 1 => Long-Haul, 2 => Short-Haul
    float result = 0;
    static const float regional_index[] = {7.5, 6.2, 5.5, 2, 2, 6.4, 8.5, 9, 9.667, 10, 8.667, 8.167, 8, 7.5, 7.5, 7.833, 8.333, 8.5, 8.667, 9.5, 9.333, 9.333, 9, 8.5};
    static const float shortH_index[] = {5.5, 3.167, 1.333, 1, 1.167, 2.833, 5.833, 8, 9.667, 10, 8.667, 8.167, 8, 7.5, 7.5, 7.833, 8.333, 8.5, 8.667, 9.5, 9.333, 8.833, 8, 7.167};
    static const float longH_index[] = {7, 6, 3.3, 1, 1.167, 2.833, 5.833, 8, 9.667, 10, 8.667, 8.167, 8, 7.5, 7.5, 7.833, 8.333, 8.5, 8.667, 9.5, 9.333, 8.833, 8, 8};
    
    arrival_time += (timezone * 100);
    if(arrival_time >= 2400){
        arrival_time = (arrival_time % 2400);
    }
    
    switch(route_type - 1){
        case 0:
            result = regional_index[(arrival_time / 100)];
            return result;
        case 1:
            result = longH_index[(arrival_time / 100)];
            return result;
        case 2:
            result = shortH_index[(arrival_time / 100)];
            return result;
    }
}

float reputation_score_indexer(float popularity, float quality){
	return ((popularity / 100) * (quality - 50)) + 50;
}

float price_indexer(float default_price, float route_price){
    return (default_price / route_price) * 40.0;
}

float catering_indexer(int price, int catering_quality){
	if(price > 0){
		if(catering_quality < 30){
			//Zone 1
			return 5.333 + (0.5 * catering_quality) + (-0.052 * price);		
		}else{
			//Zone 2
			return 14.655 + (0.473 * catering_quality) + (-0.1 * price);		
		}
	}else{
		//Zone 3
		return 40 + (0.6 * catering_quality);		
	}
}

float seat_area_indexer(int pitch, float width){
	if(pitch <= 31){
		if(width <= 17.5){
			//Zone 1
			return 5;
		}else if(width > 20.5){
			//Zone 3
			return -62 + (4 * width);
		}else{
			//Zone 2
			return -62 + (4 * width);
		}
	}else if(pitch > 35){
		if(width <= 17.5){
			//Zone 7
			return -56.2 + (2.4 * pitch);
		}else if(width > 20.5){
			//Zone 9
			return -112 + (3 * pitch) + (4 * width);
		}else{
			//Zone 8
			return -98.2 + (2.4 * pitch) + (4 * width);		
		}
	}else{
		if(width <= 17.5){
			//Zone 4
			return -80.8 + (3.3 * pitch);
		}else if(width > 20.5){
			//Zone 6
			return -46 + (2 * pitch) + (2 * width);		
		}else{
			//Zone 5
			return -137.3 + (3.3 * pitch) + (4 * width);
		}
	}
}

float seat_comfort_corrector(int seat_score_1, float seat_score_2){
    return seat_score_1 + seat_score_2;
}

float comfort_indexer(int seat_comfort, int catering_score){
	if(catering_score <= 30){
		if(seat_comfort <= 30){
			//Zone 1
			return 0.2 +0.37 * seat_comfort + 0.3 * catering_score;
		}else if(seat_comfort > 70){
			//Zone 3
			return -2 + 0.4 * seat_comfort + 0.4 * catering_score;
		}else{
			//Zone 2
			return 7.7 + 0.3 * seat_comfort + 0.37 + catering_score;
		}	
	}else if(catering_score > 70){
		if(seat_comfort <= 30){
			//Zone 7
			return -12 + 0.4 * seat_comfort + 0.4 *catering_score;
		}else if(seat_comfort > 70){
			//Zone 9
			return 50 + 0.3 * seat_comfort + 0.2 * catering_score;
		}else{
			//Zone 8
			return 12 + 0.4 * seat_comfort + 0.4 * catering_score;		
		}
	}
}

float dep_arr_time_processor(int departure_time_index, int arrival_time_index){
	if(departure_time_index < 3){
		if(arrival_time_index < 3){
			//Zone 1
			return 5;
		}else if(arrival_time_index >= 6){
			//Zone 3
			return 5.325 + 5.914 * arrival_time_index;
		}else{
			//Zone 2
			return 5 * arrival_time_index;
		}	
	}else if(departure_time_index >= 6){
		if(arrival_time_index < 3){
			//Zone 7
			return 3.349 + 3.655 * departure_time_index;
		}else if(arrival_time_index >= 6){
			//Zone 9
			return 17.716 + 4.562 * departure_time_index + 3.432 * arrival_time_index;
		}else{
			//Zone 8
			return 6.959 + 5.929 * departure_time_index + 2.47 * arrival_time_index;		
		}	
	}else{
		if(arrival_time_index < 3){
			//Zone 4
			return 16.41 + 2.053 * departure_time_index;	
		}else if(arrival_time_index >= 6){
			//Zone 6
			return 20.254 + 1.559 * departure_time_index * 6.143 * arrival_time_index;
		}else{
			//Zone 5
			return 34.064 + 1.8 * arrival_time_index + 2.566 * departure_time_index;		
		}	
	}
}

float price_reputation_processor(int price, int reputation){
	if(reputation <= 25){
		if(price >= 85){
			//Zone 1
			return -36.667 + 0.667 * price;
		}else if(price < 30){
			//Zone 3
			return 5;
		}else{
			//Zone 2
			return 2.196 + 0.25 * price;
		}
	}else if(reputation > 80){
		if(price >= 85){
			//Zone 7
			return 43.694 + 0.275 * reputation + 0.289 * price;		
		}else if(price < 30){
			//Zone 9
			return 20 + 0.5 * reputation;
		}else{
			//Zone 8
			return 33.881 + 0.35 * reputation + 0.297 * price;		
		}	
	}else{
		if(price >= 85){
			//Zone 4
			return 22.889 + 0.357 * reputation + 0.333 * price;
		}else if(price < 30){
			//Zone 6
			return 16.222 + 0.357 * reputation;
		}else{
			//Zone 5
			return 10.83 + 0.548 * price + 0.295 * reputation;
		}
	}
}

float price_comfort_indexer(float comfort, int price){
	if(comfort <= 30){
		if(price <= 30){
			//Zone 1
			return 3.7 + 0.66 * price;		
		}else if(price > 70){
			//Zone 3
			return 5;
		}else{
			//Zone 2
			return -12.324 + 0.744 * price;
		}
	}else if(comfort > 70){
		if(price <= 30){
			//Zone 7
			return 100;
		}else if(price > 70){
			//Zone 9
			return 55 + 1 * price;
		}else{
			//Zone 8
			return 54.926 + 0.494 * price;
		}
	}else{
		if(price <= 30){
			//Zone 4
			return 0.8276 + 0.407 * comfort + 0.668 * price;
		}else if(price > 70){
			//Zone 6
			return 2 + 0.6 * comfort;
		}else{
			//Zone 5
			return -3.093 + 0.6 * comfort + 0.489 * price;
		}	
	}
}

float chief_processor(float r1, float r2, float r3){
	return -320.413 + 1.967 * r1 + 2.703 * r2 + 2.64 * r3;
}

float r4_corrector(float r4, float duration_time_index){
    return (duration_time_index * 100) + r4;
}

float route_distance_indexer(int route_distance){
    return log(route_distance);
}

float frequency_effect_processor(float route_distance_index){
    float result = 0;
    float max_result = 0;
    result = -0.1766 * route_distance_index + 1.997;
    max_result = -14.8 * route_distance_index + 124.7;
    if(max_result < 0){
        max_result = 0.3;
    }
    if(result > max_result){
        return max_result;
    }else{
        return result;
    }
}

float duration_time_indexer(int average_time, int route_time){
    return (average_time - route_time) / average_time;
}

int route_time_calculate(int dep_time, int arr_time){
    if(dep_time > arr_time){
        return 0;
    }
}

float minimum_separation_minutes(int route_distance){
    int result = route_distance / 1000 * 60;
    if(result >= 240){
        return 240;
    }else{
        return result;
    }
}

float stop_penalty_corrector(int stopover, int min_stop){
    //stopover
    //0 => direct-exist, 1 => direct-not-exist
    if(min_stop == 0){
        if(stopover == 0){
            return 0;
        }else{
            return 40 * stopover;
        }
    }else{
        return (float)(30.0 / min_stop * (stopover - min_stop));
    }
}

float global_correction(float r4, float penalty){
    return r4 - penalty;
}

float fr_correction(float fr, float frequency_bonus, int frequency){
    return (fr * (frequency_bonus * (frequency - 1) + 1)) / frequency;
}

int passenger_distribution(float fr, float sum_of_fr, int route_demand){
    return ceil((fr / sum_of_fr) * route_demand);
}

//Passenger allocation
int passenger_calculation(float allocated_percentage, int route_demand, int aircraft_capacity){
    int result = route_demand * allocated_percentage;
    if(result > aircraft_capacity){
        return aircraft_capacity;
    }else{
        return result;
    }
}

int remain_passenger_allocation(int remain_passenger, float fr, float sum_of_fr){
    int result = remain_passenger * (fr / sum_of_fr);
    return result;
}