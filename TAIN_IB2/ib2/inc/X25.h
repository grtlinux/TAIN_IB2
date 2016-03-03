/*
 * PROGRAM ID   : X25.h
 * PROGRAM NAME : X25 ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : X25 ���� ���
 * HISTORY      : 2013.10.10 ����. �����ۼ�
 *
 */

#ifndef _X25_H_
#define _X25_H_

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

int X25Send(int sock, char* msg, int len)           ;
int X25Recv(int sock, char* msg, int len)           ;
int X25RecvFFEF(int sock, char* msg, int maxLen)    ;
int X25MsgSend(int sock, char* msg, int len)        ;
int X25MsgRecv(int sock, char* msg)                 ;
int X25MsgSendFFEF(int sock, char* msg, int len)    ;
int X25MsgRecvFFEF(int sock, char* msg)             ;
int X25Init(int sock)                               ;


#endif


