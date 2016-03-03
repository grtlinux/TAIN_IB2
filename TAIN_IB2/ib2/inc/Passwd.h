/*
 * PROGRAM ID   : Passwd.h
 * PROGRAM NAME : Passwd ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Passwd ���
 * HISTORY      : 2013.08.04 ����. �����ۼ�
 *
 */

#ifndef _PASSWD_H_
#define _PASSWD_H_

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

char *GetDtTm();
char *GetPassword( char *sFepId, int iProcTp, char *sUserId, char *sPasswd, char *sOrgzCd );
char *GetKftcPasswrd( int iBizTp, int iProcTp, char *sUserId, char *sPasswd, char *sOrgzCd );
int KftcValidation( int iBizTp, PASSWORD_I *stPwi );
int KftcEncryption( PASSWORD_I *stPwi, PASSWORD_O *stPwo );
int KftcDecryption( PASSWORD_I *stPwi, PASSWORD_O *stPwo );


#endif


