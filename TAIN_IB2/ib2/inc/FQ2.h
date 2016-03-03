/*
 * PROGRAM ID   : FQ2.h
 * PROGRAM NAME : ����ť�� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ����ť�� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.10.02 ����. �����ۼ�
 *
 */

#ifndef _FQ2_H_
#define _FQ2_H_

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

int FQ2Init(char* sFqName, char* sServiceid)   ;
FQ2* FQ2Open(char* sFqName, char* sServiceid)  ;
int FQ2Close(FQ2* pFq)                         ;
int FQ2Print(FQ2* pFq)                         ;
int FQ2Write(FQ2* pFq, char* str)              ;
char* FQ2Read(FQ2* pFq)                        ;
int FQ2DiffSeq(char* sFqName, char* sServiceid);



#endif


