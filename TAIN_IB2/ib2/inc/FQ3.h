/*
 * PROGRAM ID   : FQ3.h
 * PROGRAM NAME : ����ť�� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ����ť�� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.10.16 ����. �����ۼ�
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


