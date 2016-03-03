/*
 * PROGRAM ID   : CSock.h
 * PROGRAM NAME : Client Socket ���� ��� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Client Socket ���� ��� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 */

#ifndef _CSOCK_H_
#define _CSOCK_H_

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

BOOL CSockPrintSocketTable();
BOOL CSockFDZERO();
BOOL CSockFDSET(int socket, char ch);
BOOL CSockFDCLR(int socket);
BOOL CSockFDCLR_All();
fd_set CSockGetFdSet(char chType);
int CSockGetFdCount(char chType);
int CSockGetMaxFds(char chType);
BOOL CSockSocket(char* strHostIp, int iPort, int connCount);
BOOL CSockReadable(int (*handler) (int));
int CSockReceiver(int cSock);
void CSockWritableAlarm(int signum);
BOOL CSockWritable(int (*handler) (int));
int CSockSender(int cSock);
int CSockReadWrite(int (*handler) (int));
int CSockWriteRead(int (*handler) (int));



#endif


