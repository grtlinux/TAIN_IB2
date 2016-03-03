/*
 *  PROGRAM ID      : bk_fep_agent.h
 *  PROGRAM NAME    : FEP(뱅킹) Agent API header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.09.12 이현, 최초작성
 */

#include <oaacom0000.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/time.h>

#ifndef _BK_FEP_AGENT_H_
#define _BK_FEP_AGENT_H_

#define BKFEP_MAXDATASIZE  sizeof(BIZ_DATA)+1

#define APNO1              "01"
#define APNO2              "02"

#define SEND_DATA_FILE     "/hw03/ibridge/DAT/%.8s/OFS%.5s%.2s"
#define SEND_STAT_FILE     "/hw03/ibridge/SEQ/%.8s/OFS%.5s%.2s"
#define SEND_PIPE_NAME     "/hw03/ibridge/FIFO/OFS%.5s%.2s"
#define RECV_DATA_FILE     "/hw03/ibridge/DAT/%.8s/OFR%.5s%.2s"
#define RECV_STAT_FILE     "/hw03/ibridge/SEQ/%.8s/OFR%.5s%.2s"
#define RECV_PIPE_NAME     "/hw03/ibridge/FIFO/OFR%.5s%.2s"
#define BATCH_SEND_TR_FILE "/hw03/hwbiz/BK/SEND/%.8s/%.1s_%.3s_%.11s_%.8s_%.2s"
#define BATCH_RECV_TR_FILE "/hw03/hwbiz/BK/RECV/%.8s/%.1s_%.3s_%.11s_%.8s_%.2s"
#define BATCH_TR_FILE_LIST "/hw01/ibridge/ram/cfg/baa_trlist.cfg"

/*
 *  FEP File data format
 */
typedef struct _FILE_DATA
{
    char sSequence[8];  /* 일련번호 */
    char sReceiver[10]; /* 수신프로세스 */
    char sDataLen[5];   /* 데이터길이 */
    BIZ_DATA stB;       /* FEP전문 Biz전문 */
} FILE_DATA;

/*
 *  FEP Batch File data format
 */
typedef struct _BATCH_FILE_DATA
{
    char sSequence[8];  /* 일련번호 */
    char sReceiver[10]; /* 수신프로세스 */
    char sDataLen[5];   /* 데이터길이 */
    BATCH_INFO stI;     /* FEP전문 Batch Info */
} BATCH_FILE_DATA;

/*
 *  FEP File Queue
 */
typedef struct _FQ
{
    char  sFepId[5+1];  /* FEP ID              */
    char  sApNo[2+1];   /* AP 구분             */
    char  cSR;          /* 송수신구분          */
    char  cRW;          /* Read/Write구분      */
    char  sNow[20+1];   /* 작업시각            */
    int   iWriteSeq;    /* 총  수신일련번호    */
    int   iReadSeq;     /* 최종수신일련번호    */
    long  lPos;         /* file position       */
    FILE *pstFData;     /* DATA File           */
    FILE *pstFStat;     /* STAT file           */
    int   iLocked;      /* STAT file lock 여부 */
    struct flock stLock;/* file lock           */
}   FQ;

/*
 *  FEP 송수신 관리정보
 */
typedef struct
{
    char sBizCode[6+1];
    char sIP[15+1];
    int  iPort;
    char cACK;
    int  iSleepInt;
    int  iPollTime;
    int  iFd;
    int  iWaitCnt;
}   OFA_FEP_INFO;


/*
 *  Function     : StrBKFepErr
 *  Input        : int err_no
 *  Output       :
 *  Return       : char *
 *  Description  : FEP(뱅킹) API에서 반환한 에러값에 대한 메시지를 반환
 *                 -11 : FEP ID 오류
 *                 -12 : DATA 오류
 *                 -13 : 길이 오류
 *                 -14 : DATA 버퍼파라미터 오류
 *                 -15 : 길이 파라미터 오류
 *                 -16 : 일련번호(SEQ) 오류
 *                 -17 : FEP Agent DATA FILE 오류
 *                 -18 : 배치 TR정보 없음
 *                 -19 : 배치 TR명 오류
 *                 -20 : 배치 TR파일 없음
 *                 -21 : 배치 TR정보파일에 오류가 있습니다.
 *                 -99 : SYSTEM 오류
 */
char *StrBKFepErr( int err_no );

/*
 *  Function     : ReadStatFile
 *  Input        : char *psFepId(FEP ID)
 *  Output       : int *piWriteSeq: 쓴    일련번호
 *                 int *piReadSeq : 읽어간일련번호
 *  Return       : int(정상:0, 실패시 errno)
 *  Description  : FEP ID에 대해 쓴일련번호, 읽어간일련번호를 반환
 */
int   GetBKFepSeq( char *psFepId, int *piWriteSeq, int *piReadSeq );

/*
 *  Function     : SendToBKFep
 *  Input        : char *psFepId(FEP ID)
 *                 char *psMsg(전송할 전문)
 *                 int iMsgLen(전송할 전문 길이)
 *  Output       : int *piSeq(전송처리 후 일련번호값, NULL 가능)
 *  Return       : int(정상:0, 실패시 errno)
 *  Description  : 주어진 FEP ID로 전문을 전송하며 piSeq가 있을 경우 해당 일련번호를 반환
 */
int   SendToBKFep( char *psFepId, char *psMsg, int iMsgLen, int *piSeq );

/*
 *  Function     : RecvFromBKFep
 *  Input        : char *psFepId(FEP ID)
 *                 int *piSeq(수신하려는 일련번호, NULL 가능)
 *  Output       : char *psMsg(수신된 전문)
 *                 int *piMsgLen(0:전문이 존재하지 않을 경우, 수신된 전문의 길이)
 *                 int *piSeq(수신된 전문의 일련번호, NULL 가능)
 *  Return       : int(정상:0, 실패시 errno)
 *  Description  : 주어진 FEP ID로 전문을 수신하며 piSeq가 있을 경우 해당 일련번호를 반환
 *                 # piSeq가 NULL이거나 *piSeq가 0 또는 최종일련번호보다 큰 값이면
 *                   이전에 수신한 전문의 다음 전문을 수신
 *                 # 반환되는 일련번호의 순서는 보장됨
 *                 # 반환되는 일련번호의 연속성은 보장되지 않음
 *                   1번 전문이 수신된 후 3번 전문이 수신 될 수 있음
 *
 */
int   RecvFromBKFep( char *psFepId, char *psMsg, int *piMsgLen, int *piSeq );

/*
 *  Function     : SeekBKFep
 *  Input        : char *psFepId(FEP ID)
 *                 int iSeekSeq(재설정할 일련번호)
 *  Output       :
 *  Return       : int(정상:0, 실패시 errno)
 *  Description  : 주어진 FEP ID에 대해 최종일련번호를 재설정하여 다음 일련번호부터 수신하도록 한다.
 *                 # iSeekSeq = 0           : 1번부터 다시 수신
 *                   iSeekSeq = n           : n+1번부터 다시 수신
 *                   iSeekSeq > 최종일련번호: 변동 없음
 */
int   SeekBKFep( char *psFepId, int iSeekSeq );

#endif
