/*
 * PROGRAM ID   : Util.h
 * PROGRAM NAME : ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ���������� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 */

#ifndef _UTIL_H_
#define _UTIL_H_

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
char* UtilTrim( char* str )                                   ;
char* UtilDelBracket( char* str )                             ;
char* UtilDelComment( char* str )                             ;
char** UtilStrSplit( char* str, char ch, char** arrItem )     ;
int UtilGetCmdLine( char* str, char** arr )                   ;
char** UtilGetArgs(char* str, char** args)                    ;
char* UtilToUpper( char* str )                                ;
char* UtilToLower( char* str )                                ;
int UtilToInt( char* str )                                    ;
int UtilToIntN( char* str, int n )                            ;
long UtilToLong( char* str )                                  ;
long UtilToLongN( char* str, int n )                          ;
char* UtilFillCopy(char* tgt, char* src, int size, char ch )  ;
char* UtilSetYYYYMMDD(char* str)                              ;
char* UtilSetFEPID(char* str, char* sFepId)                   ;
char* UtilSetMSGID(char* str, char* sMsgId)                   ;


#endif
