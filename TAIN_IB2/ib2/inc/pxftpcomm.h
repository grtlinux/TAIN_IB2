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

#include <LibConstDefine.h>
#include <LibExtVarDefine.h>

#ifndef _PXFTPCOMM_H_
#define _PXFTPCOMM_H_

/*
 *  Define
#define   VERSION             "2010.12.08.03"
#define   VERSION             "2010.12.09.01: 송신파일삭제시 작업에서 제거"
 */
#define   VERSION             "2011.01.19.01: shell 실행 값 받기"

#define   INFO_FILE           "cfg/ftp.cfg"

#define   SEND_RETRY_CNT          3
#define   MAX_DATA_BYTES       4096

#define   N_TO_S               while( iSndLen > strlen(sSndBuf) && (sSndBuf[strlen(sSndBuf)] = 0x20) )

#define   _ER_                 __func__, __LINE__, 'E'
#define   _WR_                 __func__, __LINE__, 'W'
#define   _IN_                 __func__, __LINE__, 'I'
#define   _DE_                 __func__, __LINE__, 'D'
#define   LOG_LEVEL           (cLogLevel == 'E' ? "ER" : \
                               cLogLevel == 'W' ? "WR" : \
                               cLogLevel == 'I' ? "IN" : "DE")

typedef   unsigned char        UCHAR;
typedef   unsigned long        ULONG;

typedef struct
{
    char   sProcName[ 10+1];   /* 프로그램ID */
    char   sInfoType[ 10+1];   /* 정보구분   */
    char   sContent [256+1];   /* 내용       */
}   CONF_INFO;
CONF_INFO conf;

typedef union
{
    char   sChr[4];
    short  
    int    sInt;
    int    iInt;
}   ST_LEN;

typedef struct {
    pthread_t thread;
    int       iIdx;
    int       iFlag;
    int       iFd;
    char     *psDirectory;
    char     *psFilename;
    time_t    tStart;
    time_t    tEnd;
    off_t     size;
}   WORK_INFO;

#if 0
typedef struct {
    int       iLen;
    char      sProcName[10];
    char      sSession [ 3];
    char      sSequence[ 6];
    char      sMsgType [ 1];    /* r:direcotyr, f:filename, d:data, e:end */
    char      sData    [ 1];
}   MSG_FMT;
#endif

/*
 *  Global Variable Declare
 */
void  EventLoop();
int   GetWork();
int   SendSocket( int iSendFd, char *sMsgBuf, int iMsgLen );
int   RecvSocket( int iSendFd, char *sMsgBuf, MSG_FMT *stP );
void *FileSendThread( void *arg );
void *FileRecvThread( void *arg );
void  ThreadExit( int iWorkIdx, int iRtv );
void  InitThread();
int   GetConfigInfo( char *sProcName );
char *GetFormattedStr( char *sFmt, char *sDir, char *sFile, const char *sFunc, int iLine, char cLogLevel );
int   MakeDir( char *sFilename, int iOffset );
void  Log(const char *sFunc, int iLine, char cLogLevel, const char *psMsg, ... );
void  LogPut( char *, int, int, int, const char *, ... );
void  SETLOGPUT(void  (*)( char *, int, int, int, const char *, ... ));
void  LibSignalHandler( int iCode );

#endif
