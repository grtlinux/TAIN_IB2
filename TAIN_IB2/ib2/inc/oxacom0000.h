/*
 *  PROGRAM ID      : oxacom0000.h
 *  PROGRAM NAME    : ��� �¶��� ����
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 ����, �����ۼ�
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

#define   SOCK_ACCE_FILE      "ram/cfg/oxa_access.cfg"  /* Access       ���� ȭ�� */
#define   SOCK_INFO_FILE      "ram/cfg/oxa_line.cfg"    /* Socket  Connect    ���� ȭ�� */
#define   ROUT_INFO_FILE      "ram/cfg/oxa_rout.cfg"    /* Message Routing    ���� ȭ�� */
#define   CONV_INFO_FILE      "ram/cfg/oxa_conv.cfg"    /* Message Conversion ���� ȭ�� */
#define   POLL_INFO_FILE      "ram/cfg/oxa_poll.cfg"    /* Polling            ���� ȭ�� */
#define   FDRW_INFO_FILE      "ram/cfg/oxa_fdrw.cfg"    /* FD Read Write      ���� ȭ�� */
#define   REER_INFO_FILE      "ram/cfg/oxa_reer.cfg"    /* Error ��������     ���� ȭ�� */

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
 *  Process�� ������ �ڿ�����
 */
typedef struct
{
    char   sProcName[10+1];   /* ���α׷�ID                                           */
    char   sRecvClas[ 3+1];   /* ���ű���     (X.�ܺ�,   I.����)                      */
    char   sSendClas[ 3+1];   /* �۽ű���     (X.�ܺ�,   I.����)                      */
    char   sLineType[ 1+1];   /* ��������     (S.Server, C.Client, W.Write, R.Read)   */
    char   sLineId  [15+1];   /* Ip Address                                           */
    char   sPortNo  [ 6+1];   /* Port No                                              */
    char   sClosType[ 1+1];   /* Close Type   (C.Close,  N.No, S.�۽Ž�, R.���Ž�)    */
    char   sRecvLoop[ 1+1];   /* ����Loopó�� (L.Loop,   N.No)                        */
    char   sRlenType[ 1+1];   /* ���ű���Type (B.Binary, D.Decmal, X.MegaPac)         */
    char   sRlenOset[ 4+1];   /* ���ű���Offset                                       */
    char   sRealByte[ 4+1];   /* ���ű��̽�Byte                                       */
    char   sLenMinus[ 4+1];   /* ���ű���Minus                                        */
    char   sConvType[ 1+1];   /* ������ȯType (A.ASCII,  E.EBCDIC, N.No)              */
    char   sTimeout [ 4+1];   /* Idle ���ӽð�                                        */
    char   sUseCnt  [ 4+1];   /* ���ÿ��᰹��                                         */
    char   sMaxCnt  [ 4+1];   /* �ִ뿬�᰹��                                         */
    char   sMsgSave [ 1+1];   /* �������忩��  (Y:Yes,     N:No)                      */
    char   sComment [50+1];   /* ����                                                 */
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
 *  Service(���� ����/ó��/�۽�)�� ������ �ڿ�����
 */
typedef struct
{
    int  id;
    char type;
    FD *fd;
    FQ *fq;
}   SERVICE_CONTEXT;

/*
 *  Key �� ���� Position ����
 *  0.����ڵ�, 1.��������, 2.�ŷ�����, 3.Socket Fd
 */
typedef struct
{
    char   sInfoClas[3+1];
    int    iPos[3];       /* Position */
    int    iLen[3];       /* Length   */
}   KEYS_ROUT;

/*
 *  Message Routing ����
 */
typedef struct
{
    char   sProcName[10+1];   /* ���α׷���                            */
    char   sInfoClas[ 3+1];   /* �������� O.����ڵ� ��ġ
                                          D.�������� ��ġ
                                          B.�ŷ����� ��ġ
                                          F.FD Get & Set ��ġ
                                          X.�ܺο��� ����(eXternal)
                                          I.���ο��� ����(Internal)    */
    char   sRecvOrg [11+1];   /* ���ű���ڵ�                          */
    char   sRecvDoc [11+1];   /* ������������                          */
    char   sRecvBiz [11+1];   /* ���Űŷ�����                          */
    char   sSendClas[ 3+1];   /* �۽ż��ϱ���                          */
    char   sComment [50+1];   /* ����                                  */
    int    iLastLineId;
    KEYS_ROUT *pstRKey;
}   ROUT_INFO;


/*
 *  Conversion ���� Key �� Position ����
 *  0.����ڵ�, 1.��������, 2.�ŷ�����
 */
typedef struct
{
    char   sRecvClas[3+1];
    int    iPos[3];           /* Position */
    int    iLen[3];           /* Length   */
    char   sFil[1+1];         /* Filler   */
}   KEYS_CONV;

/*
 *  Data Conversion ����
 */
typedef struct
{
    char   sProcName[10+1];   /* ���α׷���                           */
    char   sRecvClas[ 3+1];   /* �������� O.����ڵ� ��ġ|����
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
    KEYS_CONV *pstCKey;
}   CONV_INFO;

/*
 *  Polling ����
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
 *  FD Read Write ����
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
 *  Error �������� ����
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
    int  iFlag;         /* ��뿩�� 1:���, 0:�̻�� */
    int  iId;           /* LINE ���� index */
    FD  *pstFD;         /* FD�� ��� pointer */
}   DEF_THREAD_PARAM;

/* Global Variable Declare */

#endif
