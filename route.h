/* 
 * File:   route.h
 * Author: chung
 *
 * Created on August 12, 2013, 4:30 PM
 */

#ifndef ROUTE_H
#define	ROUTE_H

#ifdef	__cplusplus
extern "C" {
#endif

void scoreProcessing(MYSQL* db, int seat_class);
void ticketIncomeProcessing(MYSQL* db, int weekdate, int year, int month, int day, int hour, int minute);


#ifdef	__cplusplus
}
#endif

#endif	/* ROUTE_H */

