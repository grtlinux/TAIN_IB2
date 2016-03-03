/*
 * PROGRAM ID   : SSock.h
 * PROGRAM NAME : Server Socket ���� ��� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Server Socket ���� ��� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 */

#ifndef _SSOCK_H_
#define _SSOCK_H_

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

BOOL SSockPrintSocketTable();
BOOL SSockFDZERO();
BOOL SSockFDSET(int socket, char ch);
BOOL SSockFDCLR(int socket);
fd_set SSockGetFdSet(char chType);
int SSockGetFdCount(char chType);
int SSockGetMaxFds(char chType);
BOOL SSockSocket(int iPort);
BOOL SSockAccept();
BOOL SSockReadable(int (*handler) (int));
int SSockReceiver(int cSock);
void SSockWritableAlarm(int signum);
BOOL SSockWritable(int (*handler) (int));
int SSockSender(int cSock);
int SSockReadWrite(int (*handler) (int));
int SSockWriteRead(int (*handler) (int));



#endif


