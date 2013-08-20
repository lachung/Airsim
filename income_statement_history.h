/* 
 * File:   income_statement_history.h
 * Author: chung
 *
 * Created on August 12, 2013, 4:52 PM
 */

#ifndef INCOME_STATEMENT_HISTORY_H
#define	INCOME_STATEMENT_HISTORY_H

#ifdef	__cplusplus
extern "C" {
#endif

void incomeStatementWeeklyProcess(MYSQL* db, int year, int month, int day, int week_number);
    void incomeStatementMonthlyProcess(MYSQL* db, int year, int month, int day);
    void incomeStatementQuarterlyProcess(MYSQL* db, int year, int month, int day);
    void incomeStatementYearlyProcess(MYSQL* db, int year, int month, int day);


#ifdef	__cplusplus
}
#endif

#endif	/* INCOME_STATEMENT_HISTORY_H */

