/*
 * PROGRAM ID   : Time.h
 * PROGRAM NAME : 시간에 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 시간에 사용하는 함수들을 선언한다.
 * HISTORY      : 2013.07.24 강석. 최초작성
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
