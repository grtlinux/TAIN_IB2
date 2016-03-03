/*
 *  PROGRAM ID      : bk_fep_agent.h
 *  PROGRAM NAME    : FEP(��ŷ) Agent API header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.09.12 ����, �����ۼ�
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
    char sSequence[8];  /* �Ϸù�ȣ */
    char sReceiver[10]; /* �������μ��� */
    char sDataLen[5];   /* �����ͱ��� */
    BIZ_DATA stB;       /* FEP���� Biz���� */
} FILE_DATA;

/*
 *  FEP Batch File data format
 */
typedef struct _BATCH_FILE_DATA
{
    char sSequence[8];  /* �Ϸù�ȣ */
    char sReceiver[10]; /* �������μ��� */
    char sDataLen[5];   /* �����ͱ��� */
    BATCH_INFO stI;     /* FEP���� Batch Info */
} BATCH_FILE_DATA;

/*
 *  FEP File Queue
 */
typedef struct _FQ
{
    char  sFepId[5+1];  /* FEP ID              */
    char  sApNo[2+1];   /* AP ����             */
    char  cSR;          /* �ۼ��ű���          */
    char  cRW;          /* Read/Write����      */
    char  sNow[20+1];   /* �۾��ð�            */
    int   iWriteSeq;    /* ��  �����Ϸù�ȣ    */
    int   iReadSeq;     /* ���������Ϸù�ȣ    */
    long  lPos;         /* file position       */
    FILE *pstFData;     /* DATA File           */
    FILE *pstFStat;     /* STAT file           */
    int   iLocked;      /* STAT file lock ���� */
    struct flock stLock;/* file lock           */
}   FQ;

/*
 *  FEP �ۼ��� ��������
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
 *  Description  : FEP(��ŷ) API���� ��ȯ�� �������� ���� �޽����� ��ȯ
 *                 -11 : FEP ID ����
 *                 -12 : DATA ����
 *                 -13 : ���� ����
 *                 -14 : DATA �����Ķ���� ����
 *                 -15 : ���� �Ķ���� ����
 *                 -16 : �Ϸù�ȣ(SEQ) ����
 *                 -17 : FEP Agent DATA FILE ����
 *                 -18 : ��ġ TR���� ����
 *                 -19 : ��ġ TR�� ����
 *                 -20 : ��ġ TR���� ����
 *                 -21 : ��ġ TR�������Ͽ� ������ �ֽ��ϴ�.
 *                 -99 : SYSTEM ����
 */
char *StrBKFepErr( int err_no );

/*
 *  Function     : ReadStatFile
 *  Input        : char *psFepId(FEP ID)
 *  Output       : int *piWriteSeq: ��    �Ϸù�ȣ
 *                 int *piReadSeq : �о�Ϸù�ȣ
 *  Return       : int(����:0, ���н� errno)
 *  Description  : FEP ID�� ���� ���Ϸù�ȣ, �о�Ϸù�ȣ�� ��ȯ
 */
int   GetBKFepSeq( char *psFepId, int *piWriteSeq, int *piReadSeq );

/*
 *  Function     : SendToBKFep
 *  Input        : char *psFepId(FEP ID)
 *                 char *psMsg(������ ����)
 *                 int iMsgLen(������ ���� ����)
 *  Output       : int *piSeq(����ó�� �� �Ϸù�ȣ��, NULL ����)
 *  Return       : int(����:0, ���н� errno)
 *  Description  : �־��� FEP ID�� ������ �����ϸ� piSeq�� ���� ��� �ش� �Ϸù�ȣ�� ��ȯ
 */
int   SendToBKFep( char *psFepId, char *psMsg, int iMsgLen, int *piSeq );

/*
 *  Function     : RecvFromBKFep
 *  Input        : char *psFepId(FEP ID)
 *                 int *piSeq(�����Ϸ��� �Ϸù�ȣ, NULL ����)
 *  Output       : char *psMsg(���ŵ� ����)
 *                 int *piMsgLen(0:������ �������� ���� ���, ���ŵ� ������ ����)
 *                 int *piSeq(���ŵ� ������ �Ϸù�ȣ, NULL ����)
 *  Return       : int(����:0, ���н� errno)
 *  Description  : �־��� FEP ID�� ������ �����ϸ� piSeq�� ���� ��� �ش� �Ϸù�ȣ�� ��ȯ
 *                 # piSeq�� NULL�̰ų� *piSeq�� 0 �Ǵ� �����Ϸù�ȣ���� ū ���̸�
 *                   ������ ������ ������ ���� ������ ����
 *                 # ��ȯ�Ǵ� �Ϸù�ȣ�� ������ �����
 *                 # ��ȯ�Ǵ� �Ϸù�ȣ�� ���Ӽ��� ������� ����
 *                   1�� ������ ���ŵ� �� 3�� ������ ���� �� �� ����
 *
 */
int   RecvFromBKFep( char *psFepId, char *psMsg, int *piMsgLen, int *piSeq );

/*
 *  Function     : SeekBKFep
 *  Input        : char *psFepId(FEP ID)
 *                 int iSeekSeq(�缳���� �Ϸù�ȣ)
 *  Output       :
 *  Return       : int(����:0, ���н� errno)
 *  Description  : �־��� FEP ID�� ���� �����Ϸù�ȣ�� �缳���Ͽ� ���� �Ϸù�ȣ���� �����ϵ��� �Ѵ�.
 *                 # iSeekSeq = 0           : 1������ �ٽ� ����
 *                   iSeekSeq = n           : n+1������ �ٽ� ����
 *                   iSeekSeq > �����Ϸù�ȣ: ���� ����
 */
int   SeekBKFep( char *psFepId, int iSeekSeq );

#endif
