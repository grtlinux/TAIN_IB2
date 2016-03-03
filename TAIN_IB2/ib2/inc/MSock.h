/*
 * PROGRAM ID   : MSock.h
 * PROGRAM NAME : Multi Socket ���� ��� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Multi Socket ���� ��� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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

extern   int    iLLen;  /* LENLENGTH : LEN�ʵ� ���� */
extern   int    iLPls;  /* LENMINUS  : LEN�ʵ忡 ���Ե� ũ��(LEN�ʵ�) */

extern   int    iPollSendSec ;  /* recv ������ Poll send �ð�(��) */
extern   int    iTimeOutSec  ;  /* recv ������ ����close �ð�(��) */


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


