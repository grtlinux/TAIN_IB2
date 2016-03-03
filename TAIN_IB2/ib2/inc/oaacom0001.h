/*
 *  PROGRAM ID      : oaacom0001.h
 *  PROGRAM NAME    : FEP �ۼ��� FEP�� header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.09.12 ����, �����ۼ�
 *                    ��ܱ�� |     FEP             | ������
 *                             | ��ܱ��     ������ |  FEP      FEPAgent
 *                             |  �ۼ���      �ۼ��� | �ۼ���      API
 *                            <->        <->        <->        <->
 *                                                   *                  : FEP_HEADER
 *                                        *          *          *       : BIZ_HEADER
 *                             *          *          *          *       : �������
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

/* KANG �߰� : 2013.04.24 ����. AP#1('01'), AP#2('02')�� ���� ������ ���� */
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
    BATCH_INFO stI;     /* FEP���� Batch Info */
}   BATCH_FILE_DATA;

/*
 *  FEP File Queue
 */
typedef struct _FQ
{
    char  sFepId[5+1];
    char  sApNo[2+1]; /* KANG �߰� : 2013.04.24 ����. AP#1('01'), AP#2('02')�� ���� ������ ���� */
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
 *  FEP �ۼ��� ��������
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
 *  FEP �ۼ��� ���� �м� ����(��û���� ���� / ������ȣ ȹ�� ����)
 */
typedef struct _MSG_INFO
{
    int    iReqCnt;                 /* ��û���� �Ǵ� ���� ���� */
    struct REQUEST_MSG {
        int   iOffset;
        int   iLength;
        char *psVal;
    }  *pstReqs;                    /* ��û���� �Ǵ� ���� */

    int    iaaKeys[3][2];           /* �������� ����Ű ȹ������(offset/length 3�ֱ���) */
    int    iNoCnt;                  /* ���������� ������ȣ ȹ������ ���� */
    struct MSG_NO {
        char *psKey;    /* �������� ����Ű */
        int   iOffset;  /* ������ȣ offset */
        int   iLength;  /* ������ȣ length */
    }  *pstNos;                     /* ���������� ������ȣ ȹ������ */
}   MSG_INFO;

#endif

