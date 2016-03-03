/*
 *  PROGRAM ID      : bxa_header.h
 *  PROGRAM NAME    : ��� ��ġ ���� header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 ����, �����ۼ�
 *  �Ｚ��Ʈ��ũ �ϰ����� TCP/IP ��� ���� FORMAT
 */

#ifndef _BXA_SSNHDR_H_
#define _BXA_SSNHDR_H_

#define TRX_CLS_SEND "SD" /* Header ������ ���ϼ۽��� �ŷ������ڵ� */
#define TRX_CLS_RECV "RD" /* Header ������ ���ϼ����� �ŷ������ڵ� */
#define MSG_DTL_LEN  2048 /* DATA ������ ���� ũ��                   */

#define MSGTYPE_LOGIN      10
#define MSGTYPE_FILEINFO   20
#define MSGTYPE_FILEHEADER 30
#define MSGTYPE_RECVNOTIFY 40
#define MSGTYPE_DATA       50
#define MSGTYPE_LOGOUT     90

/*
 *  ���� ���
 */
typedef struct
{
    char sTrCode[   9]; /* Transaction Code                         */
    char sMsgCls[   3]; /* ���������ڵ�                             */
    char sRespCd[   3]; /* �����ڵ�                                 */
    char sBizFld[  20]; /* �����ʵ�                                 */
    char sFiller[   5]; /* ����                                     */
}   MSG_HEADER;

/*
 *  LOGIN(003/030)
 */
typedef struct
{
    MSG_HEADER stH;
    char sSender[  20]; /* Van ID                                   */
    char sPasswd[   8]; /* Van Password                             */
    char sSrFlag[   2]; /* Job Type(SD:�ڷ�۽�, RD:�ڷ����, RS:�۽Ÿ�ϼ���, RR:���Ÿ�ϼ���) */
    char sRcvSeq[  10]; /* Sequence(���Ž� ���Ÿ�� ��)             */
    char sSubjct[  10]; /* subject space                            */
    char sRdFlag[   1]; /* Flag(E:�̼���, R:�����, A:��/�����)    */
    char sStDtTm[  12]; /* ���۽ð�(YYYYMMDDhhmm)                   */
    char sEdDtTm[  12]; /* ���ð�  (YYYYMMDDhhmm)                   */
    char sChgPwd[   1]; /* Password���濩��(Y:����)                 */
    char sNewPwd[   8]; /* Password����� �ű� Password             */
    char sCommSz[   4]; /* commsize(�⺻:2048)                      */
    char sCnvFlg[   1]; /* �ڵ� ��ȯ����(N:��ȯ����)                */
    char sSendId[  10]; /* �ŷ�ó ID                                */
    char sFiller[  11]; /* FILLER                                   */
}   MSG_LOGIN;

/*
 *  LOGOUT(007/070)
 */
typedef struct
{
    MSG_HEADER stH;
    char sSender[  20]; /* Van ID                                   */
    char sPasswd[   8]; /* Van Password                             */
    char sSrFlag[   2]; /* Job Type(SD:�ڷ�۽�, RD:�ڷ����, RS:�۽Ÿ�ϼ���, RR:���Ÿ�ϼ���) */
    char sRcvSeq[  10]; /* Sequence(���Ž� ���Ÿ�� ��)             */
    char sSubjct[  10]; /* subject space                            */
    char sRdFlag[   1]; /* Flag(E:�̼���, R:�����, A:��/�����)    */
    char sStDtTm[  12]; /* ���۽ð�(YYYYMMDDhhmm)                   */
    char sEdDtTm[  12]; /* ���ð�  (YYYYMMDDhhmm)                   */
    char sFiller[  35]; /* FILLER                                   */
}   MSG_LOGOUT;

/*
 *  ��������(100/110)
 */
typedef struct
{
    MSG_HEADER stH;
    char sRcvSeq[  10]; /* Sequence(���Ž� ���Ÿ�� ��)             */
    char sSubjct[  10]; /* 8�ڸ� �̳� ���                          */
    char sFileSz[  10]; /* ���� SIZE                                */
    char sSendId[  20]; /* �۽��� ID                                */
    char sRecvId[  20]; /* ������ ID                                */
    char sFill01[   1]; /* FILLER                                   */
    char sFill02[  10]; /* FILLER                                   */
    char sNxtEnd[   3]; /* ����������(NXT:���ϰ��, END:����������) */
    char sFileTp[   3]; /* ��������(NEW:�Ϲ�)                       */
    char sFill03[   1]; /* FILLER                                   */
    char sFill04[  22]; /* FILLER                                   */
}   MSG_FILEINFO;

/*
 *  ����Ȯ��(130)
 */
typedef struct
{
    MSG_HEADER stH;
    char sRcvSeq[  10]; /* Sequence(���Ž� ���Ÿ�� ��)             */
    char sSubjct[  10]; /* 8�ڸ� �̳� ���                          */
    char sFileSz[  10]; /* ���� SIZE                                */
    char sRecvYn[   1]; /* ������ſ���(Y:����, N:�̻�����)         */
    char sFiller[  79]; /* FILLER                                   */
}   MSG_RECVNOTIFY;

/*
 *  �������()
 */
typedef struct
{
    char sHeadTp[  10]; /* ������� <<NEHEAD>>                      */
    char sJob_Tp[   1]; /* ��������                                 */
    char sDataTp[   1]; /* �ڷ�����(N:�ؽ�Ʈ, B:Binary, C:����)     */
    char sFileSz[  10]; /* ���� SIZE                                */
    char sCompSz[  10]; /* ���� SIZE                                */
    char sSendId[  10]; /* �۽��� ID                                */
    char sSendTp[   2]; /* ��ü����                                 */
    char sBodyTp[   2]; /* BODY�� ��������                          */
    char sFiller[   1]; /* FILLER                                   */
    char sRecvNu[   3]; /* ������ ��                                */
    char sRecvId[  10]; /* ������ ID                                */
    char sRcdDel[   2]; /* ���ڵ� ������                            */
}   MSG_FILEHEADER;

#endif
