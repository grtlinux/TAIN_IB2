/*
 * PROGRAM ID   : FQ3.h
 * PROGRAM NAME : 파일큐에 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 파일큐에 사용하는 함수들을 선언한다.
 * HISTORY      : 2013.10.16 강석. 최초작성
 *
 */

#ifndef _FQ3_H_
#define _FQ3_H_

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

int FQ3Init(char* sFqName, char* sServiceid)     ;
char* FQ3fgets(char* str, int maxlen, FILE* fp)  ;
FQ3* FQ3Open(char* sFqName, char* sServiceid)    ;
int FQ3Close(FQ3* pFq)                           ;
int FQ3Print(FQ3* pFq)                           ;
int FQ3Write(FQ3* pFq, char* str)                ;
char* FQ3Read(FQ3* pFq)                          ;
int FQ3DiffSeq(char* sFqName, char* sServiceid)  ;


#endif


