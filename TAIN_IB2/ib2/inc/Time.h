/*
 * PROGRAM ID   : Time.h
 * PROGRAM NAME : �ð��� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : �ð��� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 */

#ifndef _TIME_H_
#define _TIME_H_

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/

char* TimeGetDateTime()        ;
char* TimeGetDateTime2()       ;
char* TimeGetDateTime3()       ;
char* TimeGetDate()            ;
char* TimeGetDate2()           ;
char* TimeGetTime()            ;
char* TimeGetTime2()           ;
char* TimeGetLTime()           ;
char* TimeGetLTime2()          ;
char* TimeAddDay(int iDay)     ;
char* TimeAddHour(int iHour)   ;
int TimeGetWeek()              ;
char* TimeGetYear()            ;

#endif
