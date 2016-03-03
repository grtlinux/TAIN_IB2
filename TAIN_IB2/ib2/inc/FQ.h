/*
 * PROGRAM ID   : FQ.h
 * PROGRAM NAME : ����ť�� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ����ť�� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
