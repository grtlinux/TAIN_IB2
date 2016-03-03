/*
 * PROGRAM ID   : MSock.h
 * PROGRAM NAME : Multi Socket 관련 모듈 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : Multi Socket 관련 모듈 헤더
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#ifndef _MSOCK_H_
#define _MSOCK_H_

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

extern   int    iLLen;  /* LENLENGTH : LEN필드 길이 */
extern   int    iLPls;  /* LENMINUS  : LEN필드에 포함된 크기(LEN필드) */

extern   int    iPollSendSec ;  /* recv 없으면 Poll send 시간(초) */
extern   int    iTimeOutSec  ;  /* recv 없으면 세션close 시간(초) */


/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/

int MSockSend(int sock, char* msg, int len)        ;
int MSockRecv(int sock, char* msg, int len)        ;
int MSockMsgSend(int sock, char* msg, int len)     ;
int MSockMsgRecv(int sock, char* msg)              ;
int MSockMsgRecv_DecPass(int sock, char* msg)      ;
char* MSockGetTypeStr(int iType)                   ;
char* MSockGetConnTypeStr(int iType)               ;
int MSockPrintSockItem(char* sHdr, SOCK_INFO* si)  ;
int MSockPrintSocketInfo()                         ;
int MSockGetRandomIndex()                          ;
int MSockGetSendSocket()                           ;
int MSockParsingSockInfo(char* strInfo)            ;
int MSockMakeServerSocket()                        ;
int MSockAcceptSocket()                            ;
int MSockConnectSocket()                           ;
int MSockMakeSockInfo()                            ;
int MSockCloseSockInfo(SOCK_INFO* si)              ;
int MSockCheckHandler(int (*handler) (SOCK_INFO*)) ;
int MSockSendHandler(int (*handler) (SOCK_INFO*))  ;
int MSockRecvHandler(int (*handler) (SOCK_INFO*))  ;



#endif


