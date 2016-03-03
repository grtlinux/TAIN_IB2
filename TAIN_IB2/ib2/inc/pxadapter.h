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

#define   SOCK_ACCE_FILE      "ram/cfg/sockaccess.info"  /* Access       ���� ȭ�� */
#define   SOCK_INFO_FILE      "ram/cfg/sock.info"  /* Socket  Connect    ���� ȭ�� */
#define   ROUT_INFO_FILE      "ram/cfg/rout.info"  /* Message Routing    ���� ȭ�� */
#define   CONV_INFO_FILE      "ram/cfg/conv.info"  /* Message Conversion ���� ȭ�� */
#define   POLL_INFO_FILE      "ram/cfg/poll.info"  /* Polling            ���� ȭ�� */
#define   FDRW_INFO_FILE      "ram/cfg/fdrw.info"  /* FD Read Write      ���� ȭ�� */
#define   REER_INFO_FILE      "ram/cfg/reer.info"  /* Error ��������     ���� ȭ�� */

#define   SEND_RETRY_CNT          3
#define   MAX_DATA_BYTES       1024*10

#define   N_TO_S               while( iSndLen > strlen(sSndBuf) && (sSndBuf[strlen(sSndBuf)] = 0x20) )

typedef   unsigned char        UCHAR;
typedef   unsigned long        ULONG;

/*
 *  Process �� ���� �ؾ��� Socket ����
 */
typedef struct
{
    char   sProcName[10+1];   /* ���α׷�ID                                           */
    char   sRecvClas[ 2+1];   /* ���ű���     (X.�ܺ�,   I.����)                      */
    char   sSendClas[ 2+1];   /* �۽ű���     (X.�ܺ�,   I.����)                      */
    char   sConnId  [20+1];   /* Ip Address                                           */
    char   sPortNo  [ 6+1];   /* Port No                                              */
    char   sConnType[ 1+1];   /* ��������     (S.Server, C.Client, N.����)            */
    char   sClosType[ 1+1];   /* Close Type   (C.Close,  N.No, S.�۽Ž�, R.���Ž�)    */
    char   sRecvLoop[ 1+1];   /* ����Loopó�� (L.Loop,   N.No)                        */
    char   sRlenType[ 1+1];   /* ���ű���Type (B.Binary, D.Decmal, X.MegaPac)         */
    char   sRlenOset[ 4+1];   /* ���ű���Offset                                       */
    char   sRealByte[ 4+1];   /* ���ű��̽�Byte                                       */
    char   sLenMinus[ 4+1];   /* ���ű���Minus                                        */
    char   sConvType[ 1+1];   /* ������ȯType (A.ASCII,  E.EBCDIC, N.No)              */
    char   sRlenTake[ 1+1];   /* �������� ����(Y.����,   N.No)                        */
    char   sTimeout [ 4+1];   /* Idle ���ӽð�                                        */
    char   sConConn [ 4+1];   /* ���ÿ��᰹��                                         */
    char   sMaxConn [ 4+1];   /* �ִ뿬�᰹��                                         */
    char   sLogSave [ 1+1];   /* ����DB���忩��  (Y:Yes,     N:No)                    */
    char   sComment [50+1];   /* ����                                                 */
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
 *  Key �� ���� Position ����
 *  0.����ڵ�, 1.��������, 2.�ŷ�����, 3.Socket Fd
 */
typedef struct
{
    char   sInfoClas[ 5+1];
    int    iPos[3];       /* Position */
    int    iLen[3];       /* Length   */
}   KEYS_ROUT;

/*
 *  Message Routing ����
 */
typedef struct
{
    char   sProcName[10+1];   /* ���α׷���                            */
    char   sInfoClas[ 5+1];   /* �������� O.����ڵ� ��ġ
                                          D.�������� ��ġ
                                          B.�ŷ����� ��ġ
                                          F.FD Get & Set ��ġ
                                          X.�ܺο��� ����(eXternal)
                                          I.���ο��� ����(Internal)    */
    char   sRecvOrg [11+1];   /* ���ű���ڵ�                          */
    char   sRecvDoc [11+1];   /* ������������                          */
    char   sRecvBiz [11+1];   /* ���Űŷ�����                          */
    char   sSendClas[ 5+1];   /* �۽ż��ϱ���                          */
    char   sComment [50+1];   /* ����                                  */
    int    iLastSockIdx;
    KEYS_ROUT *pstRKey;
}   ROUT_INFO;


/*
 *  Conversion ���� Key �� Position ����
 *  0.����ڵ�, 1.��������, 2.�ŷ�����
 */
typedef struct
{
    int    iPos;              /* Position */
    int    iLen;              /* Length   */
    char   sFil[1+1];         /* Filler   */
}   KEYS_CONV;

/*
 *  Data Conversion ����
 */
typedef struct
{
    char   sProcName[10+1];   /* ���α׷���                           */
    char   sInfoClas[ 5+1];   /* �������� O.����ڵ� ��ġ|����
                                          D.�������� ��ġ|����
                                          B.�ŷ����� ��ġ|����
                                          X.�ܺο��� ����(eXternal)
                                          I.���ο��� ����(Internal)   */
    char   sCodeOrg [ 6+1];   /* ����ڵ�                             */
    char   sCodeDoc [ 6+1];   /* ��������                             */
    char   sCodeBiz [ 6+1];   /* �ŷ�����                             */
    char   sConvType[ 1+1];   /* ��ȯType(A.ASCII, E.EBCDIC, N.����)  */    
    char   sOffset  [ 5+1];   /* ������ġ                             */
    char   sLength  [ 5+1];   /* ��ȯ����                             */
    char   sDataAttr[ 1+1];   /* �ڷ�Ӽ�(N.����, A.����, H.�ѱ�)     */
    char   sSoSiType[ 1+1];   /* SOSI����(S.��,   N.��)               */
    char   sOtherTp [ 1+1];   /* ��Ÿ����                             */
    char   sComment [50+1];   /* ����                                 */

}   CONV_INFO;

/*
 *  Polling ����
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
 *  FD Read Write ����
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
 *  Error �������� ����
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
