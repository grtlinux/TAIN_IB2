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

#include <LibConstDefine.h>
#include <LibExtVarDefine.h>

/*
 *  Define
 */
#define   VERSION             "2011.01.13.01"

#define   MAX_THREAD           512
#define   MAX_THREAD_RETRY     100

#define   SOCK_ACCE_FILE      "ram/cfg/sockaccess.info"  /* Access       정보 화일 */
#define   SOCK_INFO_FILE      "ram/cfg/sock.info"  /* Socket  Connect    정보 화일 */
#define   ROUT_INFO_FILE      "ram/cfg/rout.info"  /* Message Routing    정보 화일 */
#define   CONV_INFO_FILE      "ram/cfg/conv.info"  /* Message Conversion 정보 화일 */
#define   POLL_INFO_FILE      "ram/cfg/poll.info"  /* Polling            정보 화일 */
#define   FDRW_INFO_FILE      "ram/cfg/fdrw.info"  /* FD Read Write      정보 화일 */
#define   REER_INFO_FILE      "ram/cfg/reer.info"  /* Error 대행응답     정보 화일 */

#define   SEND_RETRY_CNT          3
#define   MAX_DATA_BYTES       1024*10

#define   N_TO_S               while( iSndLen > strlen(sSndBuf) && (sSndBuf[strlen(sSndBuf)] = 0x20) )

typedef   unsigned char        UCHAR;
typedef   unsigned long        ULONG;

/*
 *  Process 가 수신 해야할 Socket 정보
 */
typedef struct
{
    char   sProcName[10+1];   /* 프로그램ID                                           */
    char   sRecvClas[ 2+1];   /* 수신구분     (X.외부,   I.내부)                      */
    char   sSendClas[ 2+1];   /* 송신구분     (X.외부,   I.내부)                      */
    char   sConnId  [20+1];   /* Ip Address                                           */
    char   sPortNo  [ 6+1];   /* Port No                                              */
    char   sConnType[ 1+1];   /* 접속형태     (S.Server, C.Client, N.없음)            */
    char   sClosType[ 1+1];   /* Close Type   (C.Close,  N.No, S.송신시, R.수신시)    */
    char   sRecvLoop[ 1+1];   /* 수신Loop처리 (L.Loop,   N.No)                        */
    char   sRlenType[ 1+1];   /* 수신길이Type (B.Binary, D.Decmal, X.MegaPac)         */
    char   sRlenOset[ 4+1];   /* 수신길이Offset                                       */
    char   sRealByte[ 4+1];   /* 수신길이실Byte                                       */
    char   sLenMinus[ 4+1];   /* 수신길이Minus                                        */
    char   sConvType[ 1+1];   /* 전문변환Type (A.ASCII,  E.EBCDIC, N.No)              */
    char   sRlenTake[ 1+1];   /* 길이유지 여부(Y.유지,   N.No)                        */
    char   sTimeout [ 4+1];   /* Idle 지속시간                                        */
    char   sConConn [ 4+1];   /* 동시연결갯수                                         */
    char   sMaxConn [ 4+1];   /* 최대연결갯수                                         */
    char   sLogSave [ 1+1];   /* 전문DB저장여부  (Y:Yes,     N:No)                    */
    char   sComment [50+1];   /* 설명                                                 */
    char  *psAccess;          /* socket access                                        */
    
    int    iPortNo;
    int    iListenFd;
    int    iTimeLimit;
    int    iConConn;
    int    iMaxConn;
    int    iCurConn;
    EVENT_LIST *pstLastEL;
}   CONN_INFO;

/*
 *  Key 및 전문 Position 정보
 *  0.기관코드, 1.전문종별, 2.거래구분, 3.Socket Fd
 */
typedef struct
{
    char   sInfoClas[ 5+1];
    int    iPos[3];       /* Position */
    int    iLen[3];       /* Length   */
}   KEYS_ROUT;

/*
 *  Message Routing 정보
 */
typedef struct
{
    char   sProcName[10+1];   /* 프로그램명                            */
    char   sInfoClas[ 5+1];   /* 정보구분 O.기관코드 위치
                                          D.전문종별 위치
                                          B.거래구분 위치
                                          F.FD Get & Set 위치
                                          X.외부에서 수신(eXternal)
                                          I.내부에서 수신(Internal)    */
    char   sRecvOrg [11+1];   /* 수신기관코드                          */
    char   sRecvDoc [11+1];   /* 수신전문종별                          */
    char   sRecvBiz [11+1];   /* 수신거래구분                          */
    char   sSendClas[ 5+1];   /* 송신소켓구분                          */
    char   sComment [50+1];   /* 설명                                  */
    int    iLastSockIdx;
    KEYS_ROUT *pstRKey;
}   ROUT_INFO;


/*
 *  Conversion 전문 Key 및 Position 정보
 *  0.기관코드, 1.전문종별, 2.거래구분
 */
typedef struct
{
    int    iPos;              /* Position */
    int    iLen;              /* Length   */
    char   sFil[1+1];         /* Filler   */
}   KEYS_CONV;

/*
 *  Data Conversion 정보
 */
typedef struct
{
    char   sProcName[10+1];   /* 프로그램명                           */
    char   sInfoClas[ 5+1];   /* 정보구분 O.기관코드 위치|길이
                                          D.전문종별 위치|길이
                                          B.거래구분 위치|길이
                                          X.외부에서 수신(eXternal)
                                          I.내부에서 수신(Internal)   */
    char   sCodeOrg [ 6+1];   /* 기관코드                             */
    char   sCodeDoc [ 6+1];   /* 전문종별                             */
    char   sCodeBiz [ 6+1];   /* 거래구분                             */
    char   sConvType[ 1+1];   /* 변환Type(A.ASCII, E.EBCDIC, N.없음)  */    
    char   sOffset  [ 5+1];   /* 시작위치                             */
    char   sLength  [ 5+1];   /* 변환길이                             */
    char   sDataAttr[ 1+1];   /* 자료속성(N.숫자, A.영문, H.한글)     */
    char   sSoSiType[ 1+1];   /* SOSI구분(S.유,   N.무)               */
    char   sOtherTp [ 1+1];   /* 기타구분                             */
    char   sComment [50+1];   /* 설명                                 */

}   CONV_INFO;

/*
 *  Polling 정보
 */
typedef struct
{
    char   sClas[5+1];
    int    iInterval;
    char  *psMsg;
    int    iKeys[3][2];
    int    iActionCnt;
    struct POLL_ACTION {
        char  cAction;
        char *psKey;
        int   iOffset;
        char *psValue;
    }  *pstActs;
}   POLL_INFO;

/*
 *  FD Read Write 정보
 */
typedef struct
{
    char   sClas[5+1];
    char   cRW;
    int    iKeys[3][2];
    int    iOrgCnt;
    struct ORG_MSG_NO {
        char *psKey;
        int   iOffset;
        int   iLength;
    }  *pstOrgs;
}   FDRW_INFO;

/*
 *  Error 대행응답 정보
 */
typedef struct
{
    char   sClas[5+1];
    int    iKeys[3][2];
    char   cSendMeth;
    char   sSendClas[5+1];
    int    iActionCnt;
    struct REER_ACTION {
        char  cAction;
        char *psKey;
        int   iOffset;
        char *psValue;
    }  *pstActs;
}   REER_INFO;

typedef union
{
    char   sChr[4];
    short  
    int    sInt;
    int    iInt;
}   ST_LEN;

typedef struct 
{
    int iLastWrNo;
    int iLastRdNo;

    int iWrFds[1024];

    struct FD_TBL
    {
        int    iFd;
        char   sNo[30+1];
    }   stRd[1024];
}   RETURN_FD;

typedef struct {
    pthread_t thread;
    int       iIndex;
    int       iFlag;
    void     *pEL;
}   DEF_THREAD_PARAM;

/* Global Variable Declare */
pthread_mutex_t   gstTrdMtx;
pthread_mutex_t   gstSokMtx;
DEF_THREAD_PARAM  gstTrdPrm[MAX_THREAD];
typedef void *Connections;
Connections    gstEvtGrp;

char  gsProcName[20+1];

int   giSockCnt    = 0;
int   giRoutCnt    = 0;
int   giConvCnt    = 0;
int   giPollCnt    = 0;
int   giFdrwCnt    = 0;
int   giReerCnt    = 0;
int   giLinkNxt    = 0;

KEYS_CONV   gstCkey[10];
CONN_INFO   gstConn[20];
ROUT_INFO   gstRout[99];
KEYS_ROUT   gstRKey[10];
CONV_INFO   gstConv[99];
POLL_INFO   gstPoll[10];
FDRW_INFO   gstFdrw[10];
REER_INFO   gstReer[10];
RETURN_FD   gstRtfd;

int   InitialSocket();
int   UseSocketOpen( int, int, int );
void  RecvSocketEvt( int, int );
void *RecvSocketTrd( void * );
int   RecvSocketCom( int, int, int, char ** );
void  SendSocketMsg( int, char *, int );
int   SendSocketCom( int, int, int, char *, int );

void  InitialThread();
void  ApplEventLoop();
void  SocketCloseId( char *, int );

void  UseThreadExit( int, int, int );
void *UsedSockClose();
int   GetReceivInfo( char * );
int   GetRutingInfo( int, char * );
int   MsgConversion( char *, char *, char * );

void  ApplStatusTrd();
void *UpdateTkcsI04();
void  rTrim( char * );

void Service();
int  SetReceivedFd( int, int, char * );
void CloseSocket( const char *, int, int, int );
int  GetSockFdIdx( int, int *, int * );
int  rvpoll( int iSockIdx, int iFdIdx, int iRecvFd, char *pBuffer, int iMsgLen );
void ProcessSendError( int iSockIdx, int iFdIdx, char *pMsgBuf, int iMsgLen );
int  CLOSE( const char *func, int line, int fd );
int  CheckPeerIP( int iIdx, struct sockaddr_in *peeraddr_in );
int  InitSendRecvSharedMemory();
int  SetSendRecvCnt( char *sSockClas, char *sSendRecv );

int  FgetsSockInfo( char * );
int  FgetsRoutInfo( char * );
int  FgetsConvInfo( char * );
int  FgetsPollInfo( char * );
int  FgetsFdrwInfo( char * );
int  FgetsReerInfo( char * );
