/*
 * PROGRAM ID   : FQ.h
 * PROGRAM NAME : 파일큐에 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 파일큐에 사용하는 함수들을 선언한다.
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#ifndef _FQ_H_
#define _FQ_H_

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

FQ*     FQOpen    ( char* strFqName, char* strFepid );
BOOL    FQClose   ( FQ* pFq );
BOOL    FQWrite   ( FQ* pFq, char* str );
char*   FQRead    ( FQ* pFq );
BOOL    FQGetSeq  ( FQ* pFq, int* iWSeq, int* iRSeq, long* lRPos );
BOOL    FQSetSeq  ( FQ* pFq, int iWSeq, int iRSeq, long lRPos );



#endif
