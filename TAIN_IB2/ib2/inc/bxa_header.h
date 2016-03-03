/*
 *  PROGRAM ID      : bxa_header.h
 *  PROGRAM NAME    : ��� ��ġ ���� header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 ����, �����ۼ�
 *  �ϰ����� TCP/IP ��� ���� FORMAT
 *  1) �������
 *     0320.DATA����
 *     0300.����ڷ᳻�� ����
 *     0310.���DATA ����
 *
 *  2) �Ϲݰ�������
 *     0600.��������
 *     0610.������������
 *     0620.�������Ȯ��
 *     0630.ȭ����������
 *     0640.ȭ��������������
 */

#ifndef _BXA_HEADER_H_
#define _BXA_HEADER_H_

#if defined KFT         /* ����������,ī���������� */
#  define LEN_FIELD1   1    /* Header ù��° length field ��뿩��     */
#  define LEN_FIELD2   0    /* Header �ι�° length field ��뿩��     */
#  define ORG_CODE_LEN 3    /* Header ����ڵ��� ����                  */
#  define TRX_CLS_SEND "R"  /* Header ������ ���ϼ۽��� �ŷ������ڵ� */
#  define TRX_CLS_RECV "S"  /* Header ������ ���ϼ����� �ŷ������ڵ� */
#  define SR_FLAG_SEND "B"  /* Header ������ �����۽��� �ۼ����÷��� */
#  define MSG_DTL_LEN  4058 /* DATA ������ ���� ũ��                   */
#  define WE_OPEN_ALL  1    /* ���ϼ��Žÿ��� ��簡 �����ϴ��� ����   */
#elif defined KFT1      /* ����������(�̿�����) */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "R"
#  define TRX_CLS_RECV "S"
#  define SR_FLAG_SEND "B"
#  define MSG_DTL_LEN  4058
#  define WE_OPEN_ALL  1
#elif defined SHC       /* ����ī�� */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   1
#  define ORG_CODE_LEN 2
#  define TRX_CLS_SEND "R"
#  define TRX_CLS_RECV "S"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2011
#  define WE_OPEN_ALL  1
#elif defined HDC       /* ����ī�� */
#  define LEN_FIELD1   0
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined LTC       /* �Ե�ī�� */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined SSC       /* �Ｚī�� */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined CTC       /* ��Ƽī�� */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined KSN       /* KS-NET */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 3
#  define TRX_CLS_SEND "R"
#  define TRX_CLS_RECV "S"
#  define SR_FLAG_SEND "B"
#  define MSG_DTL_LEN  4058
#  define WE_OPEN_ALL  1
#endif

#define  MSGTYPE_OPEN     10 /* �������� */
#define  MSGTYPE_FILE     20 /* �������� */
#define  MSGTYPE_DATA     30 /* DATA     */
#define  MSGTYPE_MISS     40 /* ���     */
#define  MSGTYPE_MDAT     50 /* ���DATA */
#define  MSGTYPE_CONT     60 /* ���ϰ�� */
#define  MSGTYPE_FEND     70 /* �������� */
#define  MSGTYPE_DONE     80 /* �������� */

/*
 *  ���� ���
 */
typedef struct
{
  /*char sTrCode[   9];                 ** Transaction Code                         */
#if LEN_FIELD1
    char sMsgLen[   4];                 /* TCP/IP�����۽�Byte��                     */
#endif
    char sBizKnd[   3];                 /* ���������ڵ� 'KTB'                       */
#if LEN_FIELD2
    char sMsgLen[   4];                 /* TCP/IP�����۽�Byte��                     */
#endif
#if LEN_FIELD3
    char sLength[   4];                 /* TCP/IP�����۽�Byte��                     */
#endif
    char sOrgzCd[ORG_CODE_LEN];         /* ����ڵ�                                 */
    char sMsgCls[   4];                 /* ���������ڵ�                             */
    char sTrxCls[   1];                 /* �ŷ������ڵ� 'S','R'                     */
    char sSrFlag[   1];                 /* �ۼ��� Flag  'C','B'                     */
    char sFileNm[   8];                 /* ���ϱ����ڵ�  0600/001, 0600/004 Space   */
    char sRespCd[   3];                 /* �����ڵ�                                 */
}   MSG_HEADER;

/*
 *  0600/0610 ����
 */
typedef struct
{
    MSG_HEADER stH;
    char sSendDt[  10];                 /* ���������Ͻ�MMDDhhmmss                   */
    char sBizCls[   3];                 /* ������������                             */
    char sSender[  20];                 /* �۽��ڸ�                                 */
    char sPasswd[  16];                 /* �۽��ھ�ȣ                               */
}   MSG_0600;
typedef MSG_0600 MSG_MANG;

/*
 *  0620 ����
 */
typedef struct
{
    MSG_HEADER stH;
    char  sBlockN[   4];                 /* BLOCK-NO                                 */
    char  sSequeN[   3];                 /* SEQUENCE-NO                              */
}   MSG_0620;
typedef MSG_0620 MSG_MISS_REQ;

/*
 *  0630/0640 ����
 */
typedef struct
{
    MSG_HEADER stH;
    char sFileId[   8];                 /* ���� NAME                                */
    char sFileSz[  12];                 /* ���� SIZE                                */
    char sMsgByt[   4];                 /* ���� BYTE��                              */
}   MSG_0630;
typedef MSG_0630 MSG_FILE;

/*
 *  0300 ����
 */
typedef struct
{
    MSG_HEADER stH;
    char sBlockN[   4];                 /* BLOCK-NO                                 */
    char sSequeN[   3];                 /* SEQUENCE-NO                              */
    char sMisCnt[   3];                 /* ��� ����                                */
    char sMisBit[ 100];                 /* ��� Data                                */
}   MSG_0300;
typedef MSG_0300 MSG_MISS_RESP;

/*
 *  0310/0320 ����
 */
typedef struct
{
    MSG_HEADER stH;
    char sBlockN[   4];                 /* BLOCK-NO                                 */
    char sSequeN[   3];                 /* SEQUENCE-NO                              */
    char sSlByte[   4];                 /* ��DATA BYTE��                            */
    char sMsgDtl[MSG_DTL_LEN];          /* ���� ����                                */
}   MSG_0320;
typedef MSG_0320 MSG_DATA;


#if 0
/*
 *  EA13MMDD Header
 */
typedef struct
{
    char   sRecdTp[ 1];  /* Record ����("H")           */
    char   sSeqNo [ 7];  /* �Ϸù�ȣ("0000000")        */
    char   sOrgzCd[10];  /* ����ڵ�(�̿����ĺ��ڵ�) */
    char   sFileNm[ 8];  /* FILE �̸�(EA13MMDD)        */
    char   sAcptDt[ 6];  /* ��û��������(YYMMDD") (FILE�̸��� ������ ��¥) */
    char   sFiller[68];  /* FILLER                     */
}   EA13_H;

/*
 *  EA13MMDD Trailer
 */
typedef struct
{
    char   sRecdTp[ 1];  /* Record ����("T")           */
    char   sSeqNo [ 7];  /* �Ϸù�ȣ(9999999")         */
    char   sOrgzCd[10];  /* ����ڵ�(�̿����ĺ��ڵ�) */
    char   sFileNm[ 8];  /* FILE �̸�                  */
    char   sTotCnt[ 7];  /* �� Data Record��           */
    char   sNewCnt[ 7];  /* �űԵ�ϰǼ�               */
    char   sUpdCnt[ 7];  /* �����ϰǼ�(ZERO)         */
    char   sCanCnt[ 7];  /* ������ϰǼ�               */
    char   sOptCnt[ 7];  /* ���������Ǽ�               */
    char   sFiller[29];  /* FILLER                     */
    char   sMacKey[10];  /* MAC ������                 */
}   EA13_T;
#endif


/*
 *  �۽��� ��ȣ �Է�
 */
typedef struct _PASSWORD_I_
{
    int   iProcTp;       /* ó������ 1: ��ȣȭ 2: ��ȣȭ */
    char  sUserId[20+1]; /* �۽��ڸ� */
    char  sUserPw[20+1]; /* �۽��ھ�ȣ */
    char  sBizYmd[20+1]; /* �ŷ��� */
    char  sOrgzCd[20+1]; /* �̿����ĺ��ڵ� */
    char  sEnCode[20+1]; /* �۽��ھ�ȣ ��ȣ�� */
}   PASSWORD_I;

/*
 *  �۽��� ��ȣ ���
 */
typedef struct _PASSWORD_O_
{
    char  sUserPw[20+1]; /* �۽��ھ�ȣ */
    char  sEnCode[20+1]; /* �۽��ھ�ȣ ��ȣ�� */
}   PASSWORD_O;

#endif
