/*
 * PROGRAM ID   : Passwd.h
 * PROGRAM NAME : Passwd 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : Passwd 헤더
 * HISTORY      : 2013.08.04 강석. 최초작성
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


