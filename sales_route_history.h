/* 
 * File:   sales_route_history.h
 * Author: chung
 *
 * Created on August 12, 2013, 4:42 PM
 */

#ifndef SALES_ROUTE_HISTORY_H
#define	SALES_ROUTE_HISTORY_H

#ifdef	__cplusplus
extern "C" {
#endif

void salesRouteHistoryWeeklyProcess(MYSQL* db, int week_number, int year, int month, int day);
void salesRouteHistoryMonthlyProcess(MYSQL* db, int year, int month, int day);


#ifdef	__cplusplus
}
#endif

#endif	/* SALES_ROUTE_HISTORY_H */

