/*
 *  PROGRAM ID      : oxacom0000.h
 *  PROGRAM NAME    : 대외 온라인 공통
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>
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

#ifndef _OXACOM0000_H_
#define _OXACOM0000_H_

#include <oaacom0001.h>
#include <LibConstDefine.h>
#include <LibExtVarDefine.h>

/*
 *  Define
 */
#define   MAX_THREAD           512
#define   MAX_THREAD_RETRY     100

#define   SOCK_ACCE_FILE      "ram/cfg/oxa_access.cfg"  /* Access       정보 화일 */
#define   SOCK_INFO_FILE      "ram/cfg/oxa_line.cfg"    /* Socket  Connect    정보 화일 */
#define   ROUT_INFO_FILE      "ram/cfg/oxa_rout.cfg"    /* Message Routing    정보 화일 */
#define   CONV_INFO_FILE      "ram/cfg/oxa_conv.cfg"    /* Message Conversion 정보 화일 */
#define   POLL_INFO_FILE      "ram/cfg/oxa_poll.cfg"    /* Polling            정보 화일 */
#define   FDRW_INFO_FILE      "ram/cfg/oxa_fdrw.cfg"    /* FD Read Write      정보 화일 */
#define   REER_INFO_FILE      "ram/cfg/oxa_reer.cfg"    /* Error 대행응답     정보 화일 */

#define   SEND_RETRY_CNT       3

#define   LT_SERVER           'S'
#define   LT_CLIENT           'C'
#define   LT_FQ_RD            'R'
#define   LT_FQ_WR            'W'

#define   LT_SOCKET           'S'
#define   LT_FQ               'F'

#define   IS_SOCKET(x)        ((x)==LT_SERVER||(x)==LT_CLIENT)
#define   IS_FQ(x)            ((x)==LT_FQ_RD ||(x)==LT_FQ_WR )

#define   N_TO_S               while( iSndLen > strlen(sSndBuf) && (sSndBuf[strlen(sSndBuf)] = 0x20) )

typedef   unsigned char        UCHAR;
typedef   unsigned long        ULONG;

/*
 *  Process에 연관된 자원정보
 */
typedef struct
{
    char   sProcName[10+1];   /* 프로그램ID                                           */
    char   sRecvClas[ 3+1];   /* 수신구분     (X.외부,   I.내부)                      */
    char   sSendClas[ 3+1];   /* 송신구분     (X.외부,   I.내부)                      */
    char   sLineType[ 1+1];   /* 접속형태     (S.Server, C.Client, W.Write, R.Read)   */
    char   sLineId  [15+1];   /* Ip Address                                           */
    char   sPortNo  [ 6+1];   /* Port No                                              */
    char   sClosType[ 1+1];   /* Close Type   (C.Close,  N.No, S.송신시, R.수신시)    */
    char   sRecvLoop[ 1+1];   /* 수신Loop처리 (L.Loop,   N.No)                        */
    char   sRlenType[ 1+1];   /* 수신길이Type (B.Binary, D.Decmal, X.MegaPac)         */
    char   sRlenOset[ 4+1];   /* 수신길이Offset                                       */
    char   sRealByte[ 4+1];   /* 수신길이실Byte                                       */
    char   sLenMinus[ 4+1];   /* 수신길이Minus                                        */
    char   sConvType[ 1+1];   /* 전문변환Type (A.ASCII,  E.EBCDIC, N.No)              */
    char   sTimeout [ 4+1];   /* Idle 지속시간                                        */
    char   sUseCnt  [ 4+1];   /* 동시연결갯수                                         */
    char   sMaxCnt  [ 4+1];   /* 최대연결갯수                                         */
    char   sMsgSave [ 1+1];   /* 전문저장여부  (Y:Yes,     N:No)                      */
    char   sComment [50+1];   /* 설명                                                 */
    char  *psAccess;          /* socket access                                        */
    
    int    iPortNo;
    int    iListenFd;
    int    iTimeLimit;
    int    iUseCnt;
    int    iMaxCnt;
    int    iCurCnt;

    FD    *pstLastFd;
    FQ    *pstFQ;
}   LINE_INFO;

/*
 *  Service(전문 수신/처리/송신)에 연관된 자원정보
 */
typedef struct
{
    int  id;
    char type;
    FD *fd;
    FQ *fq;
}   SERVICE_CONTEXT;

/*
 *  Key 및 전문 Position 정보
 *  0.기관코드, 1.전문종별, 2.거래구분, 3.Socket Fd
 */
typedef struct
{
    char   sInfoClas[3+1];
    int    iPos[3];       /* Position */
    int    iLen[3];       /* Length   */
}   KEYS_ROUT;

/*
 *  Message Routing 정보
 */
typedef struct
{
    char   sProcName[10+1];   /* 프로그램명                            */
    char   sInfoClas[ 3+1];   /* 정보구분 O.기관코드 위치
                                          D.전문종별 위치
                                          B.거래구분 위치
                                          F.FD Get & Set 위치
                                          X.외부에서 수신(eXternal)
                                          I.내부에서 수신(Internal)    */
    char   sRecvOrg [11+1];   /* 수신기관코드                          */
    char   sRecvDoc [11+1];   /* 수신전문종별                          */
    char   sRecvBiz [11+1];   /* 수신거래구분                          */
    char   sSendClas[ 3+1];   /* 송신소켓구분                          */
    char   sComment [50+1];   /* 설명                                  */
    int    iLastLineId;
    KEYS_ROUT *pstRKey;
}   ROUT_INFO;


/*
 *  Conversion 전문 Key 및 Position 정보
 *  0.기관코드, 1.전문종별, 2.거래구분
 */
typedef struct
{
    char   sRecvClas[3+1];
    int    iPos[3];           /* Position */
    int    iLen[3];           /* Length   */
    char   sFil[1+1];         /* Filler   */
}   KEYS_CONV;

/*
 *  Data Conversion 정보
 */
typedef struct
{
    char   sProcName[10+1];   /* 프로그램명                           */
    char   sRecvClas[ 3+1];   /* 정보구분 O.기관코드 위치|길이
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
    KEYS_CONV *pstCKey;
}   CONV_INFO;

/*
 *  Polling 정보
 */
typedef struct
{
    char   sClas[3+1];
    int    iInterval;
    int    iWaitTime;
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
    char   sClas[3+1];
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
    char   sClas[3+1];
    int    iKeys[3][2];
    char   cSendMeth;
    char   sSendClas[3+1];
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
    int  iIndex;        /* thread index */
    int  iFlag;         /* 사용여부 1:사용, 0:미사용 */
    int  iId;           /* LINE 정보 index */
    FD  *pstFD;         /* FD일 경우 pointer */
}   DEF_THREAD_PARAM;

/* Global Variable Declare */

#endif
