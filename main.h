/* 
 * File:   main.h
 * Author: chung
 *
 * Created on August 12, 2013, 2:43 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

    //Thread 
    void *timer_thread(void *ptr);
    void *routes_process_y(void *ptr);
    void *routes_process_jf(void *ptr);
    void *ticketIncome_process(void *ptr);
    void *salesRouteHistoryWeekly_process(void *ptr);
    void *salesRouteHistoryMonthly_process(void *ptr);
    void *incomeStatementWeekly_process(void *ptr);
    void *incomeStatementMonthly_process(void *ptr);
    void *incomeStatementQuarterly_process(void *ptr);
    void *incomeStatementYearly_process(void *ptr);


    
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

