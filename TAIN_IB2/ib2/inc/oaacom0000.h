/*
 *  PROGRAM ID      : oaacom0000.h
 *  PROGRAM NAME    : FEP �ۼ��� ���� header
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

#include <LibConstDefine.h>
/*#include "/hw01/ibridge/com/lib/inc/LibConstDefine.h"*/

#ifndef _OAACOM0000_H_
#define _OAACOM0000_H_

#define FQ_READ  'R'
#define FQ_WRITE 'W'
#define SR_SEND  'S'
#define SR_RECV  'R'

#define FQ_PEEK   1
#define FQ_BACKUP 1

#define SR_TIMEOUT 10

#define TC_LINK "LINK"
#define TC_LIOK "LIOK"
#define TC_POLL "POLL"
#define TC_POOK "POOK"
#define TC_DATA "DATA"
#define TC_DAOK "DAOK"
#define TC_RJCT "RJCT"
#define TC_BABG "BABG" /* ��ġ���μ����� �̿� */
#define TC_BAED "BAED" /* ��ġ���μ����� �̿� */
#define TC_RSEQ "RSEQ" /* ������μ����� �̿� */

#define SEARCH_TYPE_ASC  0
#define SEARCH_TYPE_DESC 1
#define SEARCH_TYPE_NEXT 2
#define SEARCH_TYPE_MAX  3

#define IS_BATCH(fepid) ((fepid)[3] > '4')

/*
 *  FEP���� Fep Header
 */
typedef struct _FEP_HEADER 
{
    char sLength    [ 4];  /* ��������    : �������� Field�� ������ ������ Length */
    char sBsnCode   [ 6];  /* �����ĺ��ڵ�: KBBS01 */
    char sTrCode    [ 4];  /* TR-CODE     : LINK, LIOK, DATA, DAOK, POLL, POOK, RJCT �� */
    char sResCode   [ 2];  /* �����ڵ�    : ��00��5. ��������ڵ�ǥ ���� */
    char sSeq       [ 8];  /* �����Ϸù�ȣ: ��99999999��? ����SEQ��ȣ */
    char sDataCnt   [ 2];  /* Data�Ǽ�    : 01 ~ (2 KByte �̳�) */
    char sDateTime  [20];  /* �ð�        : YYYYMMDDHHMMSSssssss */
    char sAckYn     [ 1];  /* ACK����     : Data�� ���� ACK : ���(Y), �̻��(N) LINK�ÿ� ���� �� */
    char sFiller    [ 3];
}   FEP_HEADER;

/*
 *  FEP���� Biz Header
 */
typedef struct _BIZ_HEADER
{
    char sDataLength[ 4];  /* �� Length FIELD�� ������ ���� DATA SIZE */
    char sDataSeq   [ 8];  /* �ش� Data�� �Ϸù�ȣ */
    char sGlobalId  [32];
    char sDateTime  [20];
    char sRespCode  [ 4];
    char sMediaTp   [ 2];
    char sMediaSeq  [ 1];
    char sFiller    [29];
}   BIZ_HEADER;

/*
 *  FEP���� Biz ����
 */
typedef struct _BIZ_DATA
{
    BIZ_HEADER       stH;
    char sData      [DATA_MAX_SIZE];
}   BIZ_DATA;

/*
 *  FEP����
 */
typedef struct _FEP_DATA
{
    FEP_HEADER       stH;
    BIZ_DATA         stB;
}   FEP_DATA;

/*
 *  FEP���� Batch Info
 */
typedef struct _BATCH_INFO
{
    char sRecLen    [ 4]; /* ���ڵ� ���� */
    char sRecCnt    [ 7]; /* ���ڵ� �Ǽ� */
    char sSR        [ 1]; /* �ۼ��� ���� */
    char sOrgCode   [ 3]; /* ����ڵ� */
    char sTrCode    [11]; /* �ϰ����� TR Code */
    char sTrCodeLen [ 2]; /* �ϰ����� TR Code ���� */
    char sDate      [ 8]; /* ������������ */
    char sTime      [ 9]; /* �������۽ð� */
    char sTrCodeSeq [ 2]; /* ���ں� TR Code ȸ�� */
    char sFiller    [53]; /* ���� */
}   BATCH_INFO;

/*
 *  FEP ��ġ����
 */
typedef struct _BATCH_FEP_DATA
{
    FEP_HEADER       stH;
    char sData      [DATA_MAX_SIZE];
}   BATCH_FEP_DATA;

/*
 *  TR LIST ����(bfa_trlist.cfg)
 */
typedef struct
{
    char sFepId [ 5+1];
    char sSR    [ 1+1];
    char sTrCode[11+1];
    char sRecLen[ 4+1];
    char sDesc  [50+1];
}   TRCODE_INFO;

int   SendData( int iFd, FEP_DATA *pstD, char *psBsnCode, char *psTrCode, char *psResCode, int iSeq, char cACKFlag, int iMsgLen );
int   RecvData( int iFd, FEP_DATA *pstD, char *psBsnCode, char *psTrCode, char cACKFlag );

#endif

