/*
 *  PROGRAM ID      : oaacom0001.h
 *  PROGRAM NAME    : FEP 송수신 FEP측 header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.09.12 이현, 최초작성
 *                    대외기관 |     FEP             | 업무계
 *                             | 대외기관     업무계 |  FEP      FEPAgent
 *                             |  송수신      송수신 | 송수신      API
 *                            <->        <->        <->        <->
 *                                                   *                  : FEP_HEADER
 *                                        *          *          *       : BIZ_HEADER
 *                             *          *          *          *       : 대외전문
 */

#include <oaacom0000.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/time.h>

#ifndef _OAACOM0001_H_
#define _OAACOM0001_H_

#define HOST_CNT 3

/*****
#define SEND_DATA_FILE     "/hw03/ibridge/DAT/%.8s/OAS%.5s01"
#define SEND_STAT_FILE     "/hw03/ibridge/SEQ/%.8s/OAS%.5s01"
#define RECV_DATA_FILE     "/hw03/ibridge/DAT/%.8s/OAR%.5s01"
#define RECV_STAT_FILE     "/hw03/ibridge/SEQ/%.8s/OAR%.5s01"

#define MSG_INFO_FILE      "/hw01/ibridge/ram/cfg/oia_msg.cfg"
#define HOST_INFO_FILE     "/hw01/ibridge/ram/cfg/aia_host.cfg"
#define REQHOST_FILE       "/hw03/ibridge/SEQ/%.8s/OIA%.5s01.RESHOST"
*****/

/* KANG 추가 : 2013.04.24 강석. AP#1('01'), AP#2('02')에 대한 적용을 위해 */
#define SEND_DATA_FILE     "/hw03/ibridge/DAT/%.8s/OAS%.5s%.2s"
#define SEND_STAT_FILE     "/hw03/ibridge/SEQ/%.8s/OAS%.5s%.2s"
#define RECV_DATA_FILE     "/hw03/ibridge/DAT/%.8s/OAR%.5s%.2s"
#define RECV_STAT_FILE     "/hw03/ibridge/SEQ/%.8s/OAR%.5s%.2s"

#define MSG_INFO_FILE      "/hw01/ibridge/ram/cfg/oia_msg.cfg"
#define HOST_INFO_FILE     "/hw01/ibridge/ram/cfg/aia_host.cfg"
#define REQHOST_FILE       "/hw03/ibridge/SEQ/%.8s/OIA%.5s%.2s.RESHOST"


#define BATCH_SEND_TR_FILE "/hw03/ibridge/DAT/SEND/%.8s/%.1s_%.3s_%.11s_%.8s_%.2s"
#define BATCH_RECV_TR_FILE "/hw03/ibridge/DAT/RECV/%.8s/%.1s_%.3s_%.11s_%.8s_%.2s"

/*
 *  FEP File data format
 */
typedef struct _FILE_DATA
{
    char sSequence[8];
    char sReceiver[10];
    char asSeq[HOST_CNT*2][8];
    char sDataLen[5];
    BIZ_DATA stB;
}   FILE_DATA;

/*
 *  FEP Batch File data format
 */
typedef struct _BATCH_FILE_DATA
{
    char sSequence[8];
    char sReceiver[10];
    char asSeq[HOST_CNT*2][8];
    char sDataLen[5];
    BATCH_INFO stI;     /* FEP전문 Batch Info */
}   BATCH_FILE_DATA;

/*
 *  FEP File Queue
 */
typedef struct _FQ
{
    char  sFepId[5+1];
    char  sApNo[2+1]; /* KANG 추가 : 2013.04.24 강석. AP#1('01'), AP#2('02')에 대한 적용을 위해 */
    char  cSR;
    char  cRW;
    char  sNow[20+1];
    int   iWriteSeq;
    int   iReadSeq;
    int   aiSeq[HOST_CNT*2];
    long  alPos[HOST_CNT*2];
    int   use;
    FILE *pstFData;
    FILE *pstFStat;
    int   iLocked;
    struct flock stLock;
}   FQ;

/*
 *  FEP 송수신 관리정보
 */
typedef struct
{
    char sBizCode[6+1];
    int  iPort;
    char cACK;
    int  iSleepInt;
    int  iPollTime;
    char asIP [HOST_CNT*2][15];
    int  aiFd [HOST_CNT*2];
    int  aiWaitCnt[HOST_CNT*2];
    FILE *pstFReqHost;
}   OAA_FEP_INFO;

/*
 *  FEP 송수신 전문 분석 정보(요청전문 여부 / 전문번호 획득 정보)
 */
typedef struct _MSG_INFO
{
    int    iReqCnt;                 /* 요청전문 판단 정보 갯수 */
    struct REQUEST_MSG {
        int   iOffset;
        int   iLength;
        char *psVal;
    }  *pstReqs;                    /* 요청전문 판단 정보 */

    int    iaaKeys[3][2];           /* 전문종류 구분키 획득정보(offset/length 3쌍까지) */
    int    iNoCnt;                  /* 전문종류별 전문번호 획득정보 갯수 */
    struct MSG_NO {
        char *psKey;    /* 전문종류 구분키 */
        int   iOffset;  /* 전문번호 offset */
        int   iLength;  /* 전문번호 length */
    }  *pstNos;                     /* 전문종류별 전문번호 획득정보 */
}   MSG_INFO;

#endif

