/*
 *  PROGRAM ID      : baacom0000.h
 *  PROGRAM NAME    : 배치 공통
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/signal.h>
#include <netdb.h>
#include <math.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>

#include <LibConstDefine.h>
#include <LibExtVarDefine.h>

#ifndef _BAACOM0000_H_
#define _BAACOM0000_H_

/*
 *  Define
 */
#define   SEND_RETRY_CNT          3
#define   MAX_DATA_BYTES       4096

/*
 *  Config Format
 */
typedef struct
{
    char   sProcName[ 10+1];   /* 프로그램ID */
    char   sInfoType[ 10+1];   /* 정보구분   */
    char   sContent [256+1];   /* 내용       */
}   CONF_INFO;

/*
 *  배치전문 Format
 */
typedef struct {
    char sLen     [ 4];
    char sFepId   [ 5];
    char sMsgType [ 1];    /* f:filename, s:file seq, d:data, e:end */
    char sResCode [ 2];    /* f:filename, s:file seq, d:data, e:end */
    char sSequence[ 8];
    char sData    [MAX_DATA_BYTES];
}   MSG_FMT;

int   SendSocket( int iSendFd, MSG_FMT *pstF, int iMsgLen );
int   RecvSocket( int iRecvFd, MSG_FMT *pstF, MSG_FMT *pstP );
void  Log(const char *sFunc, int iLine, char cLogLevel, const char *psMsg, ... );
void  LogPut( char *, int, int, int, const char *, ... );
void  SETLOGPUT(void  (*)( char *, int, int, int, const char *, ... ));
void  LibSignalHandler( int iCode );

#endif
