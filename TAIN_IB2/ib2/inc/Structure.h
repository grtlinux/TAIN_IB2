/*
 * PROGRAM ID   : Structure.h
 * PROGRAM NAME : ����ü ���
 * AUTHOR       : (��)ICA
 * COMMENT      : ���������� ����ϴ� ����ü�� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 */

#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

/*============================================================================*/
/*
 * header information
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
 * Select �� ���� ����ü
 *
 *    chType
 *        I : Input  (�⺻)
 *        O : Output (�⺻)
 *        E : Error  (�⺻)
 *        S : Server Socket
 *        C : Client Socket
 *        . : no descriptor
 *
 */
typedef struct _SOCKET_TABLE_
{
	char    chType             ;   /* I O E S C . */
	int     socket             ;

	char    strInfo     [100+1];
} SOCKET_TABLE;

/*----------------------------------------------------------------------------*/
/*
 * �������/������ ���� ����ü
 */
typedef struct _FLCK_FCNTL_
{
	int    fd       ;  /* file descriptor                                           */
	int    cmd      ;  /* command : F_GETLK, F_SETLK, F_SETLKW                      */
	short  l_type   ;  /* type of lock : F_RDLCK, F_WRLCK, F_UNLCK                  */
	short  l_whence ;  /* how to intrpret l_start : SEEK_SET, SEEK_CUR, SEEK_END    */
	off_t  l_start  ;  /* starting offset for lock                                  */
	off_t  l_len    ;  /* number of bytes to lock                                   */
} FLCK_FCNTL;

/*----------------------------------------------------------------------------*/
/*
 * TCP/IP ������ �ۼ��� ����ü
 */
typedef struct _PACKET_
{
	char    sLen       [4];   /* packet ���� - �����ʵ�(4) */
	char    sData      [1];   /* ���� �����ڷ� */
} PACKET;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * File Queue �� ���� ����ü
 */
typedef struct _FILE_QUEUE_
{
	char    strFepid      [  5+1]; /* FEPID       */
	char    strApno       [  2+1]; /* AP���� ��ȣ */

	char    chSendRecv           ; /* �ۼ��ű���   */
	char    chWriteRead          ; /* �����бⱸ�� */

	int     iWriteSeq            ; /* write sequence  */
	int     iReadSeq             ; /* read sequence   */
	long    lRPos                ; /* read position   */

	FILE*   pFpSeq               ; /* seq file  */
	FILE*   pFpDat               ; /* data file */
} FQ;

/*----------------------------------------------------------------------------*/
/*
 * File Queue SEQ �� ���� ����ü
 */
typedef struct _FILE_QUEUE_SEQ_
{
	char    strWriteSeq    [ 8];   /* write sequence     */
	char    strReadSeq     [ 8];   /* read sequence      */
	long    lRPos              ;   /* long read position */
} FQSEQ;

/*----------------------------------------------------------------------------*/
/*
 * File Queue DAT �� ���� ����ü
 */
typedef struct _FILE_QUEUE_DAT_
{                                /*                                  */
	char    sSequence      [ 8];   /* ��������                        */
	char    sReceiver      [10];   /* ó�� �����̰�, ���� ���� ����� */
	char    sDataLen       [ 5];   /* sData�� �����̰� '\n'���� ����  */

	char    sData          [ 1];   /* �����ϰ����ϴ� ������ */
} FQDAT;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * ���� : ����ť2�� ��ü�� �����Ѵ�.
 */
typedef struct _FILE_QUEUE2_
{
	char    sFqNm         [  3+1]; /* FQ Name                          */
	char    sSvcid        [ 10+1]; /* Service ID                       */
	char    sFepid        [  5+1]; /* FEPID                            */
	char    sApno         [  2+1]; /* AP���� ��ȣ                      */

	char    chSR                 ; /* �ۼ��ű���                       */
	char    chWR                 ; /* �����бⱸ��                     */

	int     iWSeq                ; /* write sequence                   */
	int     iRSeq                ; /* read sequence                    */

	long    lRPos                ; /* read position                    */

	FILE*   fpSeq                ; /* seq file                         */
	FILE*   fpDat                ; /* data file                        */
} FQ2;

/*----------------------------------------------------------------------------*/
/*
 * File Queue 2 SEQ �� ���� ����ü
 */
typedef struct _FILE_QUEUE2_SEQ_
{
	char    sWSeq          [ 8];   /* write sequence                   */
	char    sRSeq          [ 8];   /* read sequence                    */
	long    lRPos              ;   /* Reserved                         */
	char    cLF                ;   /* new line, line feed              */
} FQ2SEQ;

/*----------------------------------------------------------------------------*/
/*
 * File Queue 2 DAT �� ���� ����ü
 */
typedef struct _FILE_QUEUE2_DAT_
{
	char    sSeq           [ 8];   /* ��������                         */
	char    sRecvr         [10];   /* ó�� �����̰�, ���� ���� �����  */
	char    sLen           [ 5];   /* sData�� �����̰� '\n'���� ����   */

	char    sData          [ 1];   /* �����ϰ����ϴ� ������            */
} FQ2DAT;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * ���� : ����ť2�� ��ü�� �����Ѵ�.
 */
typedef struct _FILE_QUEUE3_
{
	char    sFqNm         [  3+1]; /* FQ Name                          */
	char    sSvcid        [ 10+1]; /* Service ID                       */
	char    sFepid        [  5+1]; /* FEPID                            */
	char    sApno         [  2+1]; /* AP���� ��ȣ                      */

	char    chSR                 ; /* �ۼ��ű���                       */
	char    chWR                 ; /* �����бⱸ��                     */

	int     iWSeq                ; /* write sequence                   */
	int     iRSeq                ; /* read sequence                    */

	long    lRPos                ; /* read position                    */

	FILE*   fpSeq                ; /* seq file                         */
	FILE*   fpDat                ; /* data file                        */
} FQ3;

/*----------------------------------------------------------------------------*/
/*
 * File Queue 2 SEQ �� ���� ����ü
 */
typedef struct _FILE_QUEUE3_SEQ_
{
	char    sWSeq          [ 8];   /* write sequence                   */
	char    sRSeq          [ 8];   /* read sequence                    */
	long    lRPos              ;   /* Reserved                         */
	char    cLF                ;   /* new line, line feed              */
} FQ3SEQ;

/*----------------------------------------------------------------------------*/
/*
 * File Queue 2 DAT �� ���� ����ü
 */
typedef struct _FILE_QUEUE3_DAT_
{
	char    sSeq           [ 8];   /* ��������                         */
	char    sRecvr         [10];   /* ó�� �����̰�, ���� ���� �����  */
	char    sLen           [ 5];   /* sData�� �����̰� '\n'���� ����   */

	char    sData          [ 1];   /* �����ϰ����ϴ� ������            */
} FQ3DAT;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*
 * ##### FEP ��ġ���� �۽� #####
 *
 *  1. ��ġFQ�� �д´�.
 *      BAT_INF(100) ������ �д´�.
 *
 *          typedef struct _BAT_INF_
 *          {
 *              char    sRecLen     [ 4]; // ���ڵ� ����
 *              char    sRecCnt     [ 7]; // ���ڵ� �Ǽ�
 *              char    sSR         [ 1]; // �ۼ��� ����
 *              char    sOrgCode    [ 3]; // ����ڵ�
 *              char    sTrCode     [11]; // �ϰ����� TR Code
 *              char    sTrCodeLen  [ 2]; // �ϰ����� TR Code ����
 *              char    sDate       [ 8]; // ������������ YYYYMMDD
 *              char    sTime       [ 9]; // �������۽ð� hhmmssSSS
 *              char    sTrCodeSeq  [ 2]; // ���ں� TR Code ȸ��
 *              char    sFiller     [53]; // ����
 *          } BAT_INF;
 *
 *  2. ��ġ�۽� ����
 *      �۽�:BABG : FEP_HDR(50:BABG) + BAT_INF(100)    ->
 *      ����:BABG : FEP_HDR(50:BABG)                   <-
 *
 *  3. ��ġ���� �۽�(�ݺ� ���ڵ�۽�)
 *      �۽�:DATA : FEP_HDR(50:DATA) + sDataLine(����)  ->
 *      ����:DAOK : FEP_HDR(50:DAOK)                    <-
 *
 *  4. ��ġ�۽� ����
 *      �۽�:BAED : FEP_HDR(50:BAED)    ->
 *      ����:BAED : FEP_HDR(50:BAED)    <-
 *
 * ##### FEP ��ġ���� ���� #####
 *
 *  1. ��ġ�۽� ����
 *      �۽�:BABG : FEP_HDR(50:BABG) + BAT_INF(100)    ->
 *      ����:BABG : FEP_HDR(50:BABG)                   <-
 *
 *  2. ��ġ���� �۽�(�ݺ� ���ڵ�۽�)
 *      �۽�:DATA : FEP_HDR(50:DATA) + sDataLine(����)  ->
 *      ����:DAOK : FEP_HDR(50:DAOK)                    <-
 *
 *  3. ��ġ�۽� ����
 *      �۽�:BAED : FEP_HDR(50:BAED)    ->
 *      ����:BAED : FEP_HDR(50:BAED)    <-
 *
 *  4. ��ġFQ�� �д´�.
 *      BAT_INF(100) ������ ����.
 *
 *          typedef struct _BAT_INF_
 *          {
 *              char    sRecLen     [ 4]; // ���ڵ� ����
 *              char    sRecCnt     [ 7]; // ���ڵ� �Ǽ�
 *              char    sSR         [ 1]; // �ۼ��� ����
 *              char    sOrgCode    [ 3]; // ����ڵ�
 *              char    sTrCode     [11]; // �ϰ����� TR Code
 *              char    sTrCodeLen  [ 2]; // �ϰ����� TR Code ����
 *              char    sDate       [ 8]; // ������������ YYYYMMDD
 *              char    sTime       [ 9]; // �������۽ð� hhmmssSSS
 *              char    sTrCodeSeq  [ 2]; // ���ں� TR Code ȸ��
 *              char    sFiller     [53]; // ����
 *          } BAT_INF;
 *
 */

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/
/*
 *  FEP���� Fep Header  (50 B)
 * ���Ǽۼ��Ž� �����.
 *
 */
typedef struct _FEP_HDR_50_
{
	char    sLength     [ 4];  /* ��������    : �������� Field�� ������ ������ Length                 */
	char    sBsnCode    [ 6];  /* �����ĺ��ڵ�: KBBS01                                                */
	char    sTrCode     [ 4];  /* TR-CODE     : LINK, LIOK, DATA, DAOK, POLL, POOK, RJCT ��           */
	char    sResCode    [ 2];  /* �����ڵ�    : '00'5. ��������ڵ�ǥ ����                            */
	char    sSeq        [ 8];  /* �����Ϸù�ȣ: '99999999' ? ����SEQ��ȣ                              */
	char    sDataCnt    [ 2];  /* Data�Ǽ�    : 01 ~ (2 KByte �̳�)                                   */
	char    sDateTime   [20];  /* �ð�        : YYYYMMDDHHMMSSssssss                                  */
	char    sAckYn      [ 1];  /* ACK����     : Data�� ���� ACK : ���(Y), �̻��(N) LINK�ÿ� ���� �� */
	char    sFiller     [ 3];
} FEP_HDR;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  FEP���� Biz Header  (100 B)
 * �ڷ��� ����� �����
 *
 */
typedef struct _BIZ_HDR_100_
{
	char    sDataLength [ 4];  /* �� Length FIELD�� ������ ���� DATA SIZE */
	char    sDataSeq    [ 8];  /* �ش� Data�� �Ϸù�ȣ                    */
	char    sGlobalId   [32];  /* GUID                                    */
	char    sDateTime   [20];  /* �ð�        : YYYYMMDDHHMMSSssssss */
	char    sRespCode   [ 4];
	char    sMediaTp    [ 2];
	char    sMediaSeq   [ 1];
	char    sFiller     [29];
} BIZ_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  FEP���� Biz Data
 * FQ�� ���� �дµ� ����ϴ� ����ü
 *
 * BATCH_INFO(100) + sData
 */
typedef struct _BIZ_DAT_
{
	BIZ_HDR         stBiz;  /* BIZ_HEADER (100) */

	char    sData    [ 1];  /* variable length... */
} BIZ_DAT;

/*----------------------------------------------------------------------------*/
/*
 *  FEP���� �ۼ��� Data
 * ������ �̿��� �ڷ��� �ۼ��Žÿ� �����.
 *
 *  FEP_HEADER(50) + BATCH_INFO(100) + sData
 */
typedef struct _FEP_BIZ_DAT_
{
	FEP_HDR         stFep;  /* FEP_HEADER (50)   */
	BIZ_HDR         stBiz;  /* BIZ_HEADER (100) */

	char    sData    [ 1];  /* variable length... */
} FEP_BIZ_DAT;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  FEP���� Batch Info  (100 B)
 * FQ�� ���� �дµ� ����ϴ� ����ü
 *
 *  BAT_INFO(100)
 *
 * ���ϸ� : sSR_sOrgCode_sTrCode_sDate_sTrCodeSeq
 *
 */
typedef struct _BAT_INF_
{
	char    sRecLen     [ 4]; /* ���ڵ� ����              */
	char    sRecCnt     [ 7]; /* ���ڵ� �Ǽ�              */
	char    sSR         [ 1]; /* �ۼ��� ����              */
	char    sOrgCode    [ 3]; /* ����ڵ�                 */
	char    sTrCode     [11]; /* �ϰ����� TR Code         */
	char    sTrCodeLen  [ 2]; /* �ϰ����� TR Code ����    */
	char    sDate       [ 8]; /* ������������ YYYYMMDD    */
	char    sTime       [ 9]; /* �������۽ð� hhmmssSSS   */
	char    sTrCodeSeq  [ 2]; /* ���ں� TR Code ȸ��      */
	char    sFiller     [53]; /* ����                     */
} BAT_INF;

/*----------------------------------------------------------------------------*/
/*
 *  FEP���� Batch Info Data (150 B)
 * ������ �̿��� �ڷ��� �ۼ��Žÿ� �����.
 *
 *  FEP_HEADER(50) + BATCH_INFO(100)
 */
typedef struct _FEP_BAT_INF_
{
	FEP_HDR         stFep;  /* FEP_HEADER (50)  */
	BAT_INF         stBat;  /* BATCH_INFO (100) */
} FEP_BAT_INF;

/*----------------------------------------------------------------------------*/
/*
 *  FEP���� Batch Data
 *
 *  FEP_HEADER(50) + BATCH_INFO(100) + sData
 */
typedef struct _FEP_DAT_
{
	FEP_HDR         stFep;  /* FEP_HEADER (50) */

	char    sData    [ 1];  /* variable length... */
} FEP_DAT;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define  MSGTYPE_OPEN 10  /* �������� */
#define  MSGTYPE_FILE 20  /* �������� */
#define  MSGTYPE_DATA 30  /* DATA     */
#define  MSGTYPE_MISS 40  /* ���     */
#define  MSGTYPE_MDAT 50  /* ���DATA */
#define  MSGTYPE_CONT 60  /* ���ϰ�� */
#define  MSGTYPE_FEND 70  /* �������� */
#define  MSGTYPE_DONE 80  /* �������� */

/*----------------------------------------------------------------------------*/
/*
 *  ������� : ����������, ī����������                  KFT1
 */
typedef struct _MSG_HDR_KFT1_  /* (23) */
{
	char    sMsgLen   [   4];  /* TCP/IP �����۽� Byte��                  */
	char    sBizKnd   [   3];  /* ���������ڵ� 'KTB'                      */
	char    sOrgzCd   [   3];  /* ��������ڵ� '269'                      */
	char    sMsgCls   [   4];  /* ���������ڵ�                            */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S', 'R'                   */
	char    sSrFlag   [   1];  /* �ۼ��� Flag C:Center, B:Bank            */
	char    sFileNm   [   8];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE */
	char    sRespCd   [   3];  /* �����ڵ�                                */
} MSG_KFT1_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_KFT1_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss      */
	char    sBizCls   [   3];  /* ������������                 */
	char    sSender   [  20];  /* �۽��ڸ�                     */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ                   */
} MSG_KFT1_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_KFT1_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_KFT1_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_KFT1_  /* header + (24)*/
{
	char    sFileId   [   8];  /* ���� NAME    */
	char    sFileSz   [  12];  /* ���� SIZE    */
	char    sMsgByte  [   4];  /* ���� BYTE �� */
} MSG_KFT1_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_KFT1_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_KFT1_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_KFT1_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* ��DATA BYTE��*/
	char    sMsgDtl   [8000];  /* ���� ����    */
} MSG_KFT1_0310;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_KFT1_  /* header + (110) */
{
	MSG_KFT1_HDR  stHdr   ;

	char         sData[1];
} MSG_KFT1_LINE;







/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
 *  ������� : ����������(�̿�����)                  KFT2
 */
typedef struct _MSG_HDR_KFT2_  /* (32) */
{
	char    sMsgLen   [   4];  /* TCP/IP �����۽� Byte��                  */
	char    sBizKnd   [   3];  /* ���������ڵ� 'FTE'                      */
	char    sOrgzCd   [   8];  /* ����ڵ�                                */
	char    sMsgCls   [   4];  /* ���������ڵ�                            */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S', 'R'                   */
	char    sSrFlag   [   1];  /* �ۼ��� Flag 'C', 'B' -> B               */
	char    sFileNm   [   8];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE */
	char    sRespCd   [   3];  /* �����ڵ�                                */
} MSG_KFT2_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_KFT2_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss      */
	char    sBizCls   [   3];  /* ������������                 */
	char    sSender   [  20];  /* �۽��ڸ�                     */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ                   */
} MSG_KFT2_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_KFT2_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_KFT2_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_KFT2_  /* header + (24)*/
{
	char    sFileId   [   8];  /* ���� NAME    */
	char    sFileSz   [  12];  /* ���� SIZE    */
	char    sMsgByte  [   4];  /* ���� BYTE �� */
} MSG_KFT2_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_KFT2_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_KFT2_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_KFT2_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* ��DATA BYTE��*/
	char    sMsgDtl   [8000];  /* ���� ����    */
} MSG_KFT2_0310;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_KFT2_  /* header + (110) */
{
	MSG_KFT2_HDR  stHdr   ;

	char         sData[1];
} MSG_KFT2_LINE;


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  �۽��� ��ȣ �Է�
 */
typedef struct _PASSWORD_I_
{
	int     iProcTp         ;  /* ó������ 1:��ȣȭ 2:��ȣȭ   */
	char    sUserId   [20+1];  /* �ۻ�����                     */
	char    sUserPw   [20+1];  /* �۽��ھ�ȣ                   */
	char    sBizYmd   [20+1];  /* �ŷ���                       */
	char    sOrgzCd   [20+1];  /* �̿����ĺ��ڵ�             */
	char    sEnCode   [20+1];  /* �۽��ھ�ȣ ��ȣ��            */
} PASSWORD_I;

/*----------------------------------------------------------------------------*/
/*
 *  �۽��� ��ȣ ���
 */
typedef struct _PASSWORD_O_
{
	char    sUserPw   [20+1];  /* �۽��ھ�ȣ        */
	char    sEnCode   [20+1];  /* �۽��ھ�ȣ ��ȣ�� */
} PASSWORD_O;



/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  ������� : KS-NET                  KSN
 */
typedef struct _MSG_HDR_KSN_  /* (23) */
{
	char    sMsgLen   [   4];  /* TCP/IP �����۽� Byte��                  */
	char    sBizKnd   [   3];  /* ���������ڵ� 'KTB'                      */
	char    sOrgzCd   [   3];  /* ��������ڵ� '269'                      */
	char    sMsgCls   [   4];  /* ���������ڵ�                            */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S', 'R'                   */
	char    sSrFlag   [   1];  /* �ۼ��� Flag C:Center, B:Bank            */
	char    sFileNm   [   8];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE */
	char    sRespCd   [   3];  /* �����ڵ�                                */
} MSG_KSN_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_KSN_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss      */
	char    sBizCls   [   3];  /* ������������                 */
	char    sSender   [  20];  /* �۽��ڸ�                     */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ                   */
} MSG_KSN_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_KSN_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_KSN_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_KSN_  /* header + (24)*/
{
	char    sFileId   [   8];  /* ���� NAME    */
	char    sFileSz   [  12];  /* ���� SIZE    */
	char    sMsgByte  [   4];  /* ���� BYTE �� */
} MSG_KSN_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_KSN_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_KSN_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_KSN_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* ��DATA BYTE��*/
	char    sMsgDtl   [8000];  /* ���� ����    */
} MSG_KSN_0310;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_KSN_  /* header + (110) */
{
	MSG_KSN_HDR  stHdr   ;

	char         sData[1];
} MSG_KSN_LINE;






/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  ������� : ��Ƽī��                  CTC
 */
typedef struct _MSG_HDR_CTC_  /* (23) */
{
	char    sMsgLen   [   4];  /* TCP/IP �����۽� Byte��                  */
	char    sBizKnd   [   3];  /* ���������ڵ� 'FTE'                      */
	char    sOrgzCd   [   8];  /* ��������ڵ� 'LOTTE   '                 */
	char    sMsgCls   [   4];  /* ���������ڵ�                            */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S', 'R'                   */
	char    sSrFlag   [   1];  /* �ۼ��� Flag C:Center, B:Bank            */
	char    sFileNm   [   4];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE */
	char    sCdDate   [   4];  /* �ڷ����� MMDD                           */
	char    sRespCd   [   3];  /* �����ڵ�                                */
} MSG_CTC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_CTC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss      */
	char    sBizCls   [   3];  /* ������������                 */
	char    sSender   [  20];  /* �۽��ڸ�                     */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ                   */
} MSG_CTC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_CTC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_CTC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_CTC_  /* header + (24)*/
{
	char    sFileId   [   8];  /* ���� NAME = sFileNm + sCdDate */
	char    sFileSz   [  12];  /* ���� SIZE    */
	char    sMsgByte  [   4];  /* ���� BYTE �� */
} MSG_CTC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_CTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_CTC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_CTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* ��DATA BYTE��*/
	char    sMsgDtl   [8000];  /* ���� ����    */
} MSG_CTC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_CTC_  /* header + (110) */
{
	MSG_CTC_HDR  stHdr   ;

	char         sData[1];
} MSG_CTC_LINE;






/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 *  ������� : �Ե�ī��
 */
typedef struct _MSG_HDR_LTC_  /* (32) */
{
	char    sMsgLen   [   4];  /* TCP/IP �����۽� Byte��                      */
	char    sBizKnd   [   3];  /* ���������ڵ� 'FTE'                          */
	char    sOrgzCd   [   8];  /* ����ڵ�     'LOTTE   '                     */
	char    sMsgCls   [   4];  /* ���������ڵ�                                */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S'����۽�, 'R'ī���۽�     */
	char    sSrFlag   [   1];  /* �ۼ��� Flag 'C'����۽�, 'B'ī���۽�      */
	char    sFileNm   [   4];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE     */
	char    sCdDate   [   4];  /* �ڷ����� MMDD                               */
	char    sRespCd   [   3];  /* �����ڵ�                                    */
} MSG_LTC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_LTC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss                                                  */
	char    sBizCls   [   3];  /* ������������ 001(��������),002(�̾����),003(��������),004(��������)   */
	char    sSender   [  20];  /* �۽��ڸ�   SPACE                                                         */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ SPACE                                                         */
} MSG_LTC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_LTC_  /* header + (24) */
{
	char    sFileId   [   4];  /* ���� NAME                                                 */
	char    sCdDate   [   4];  /* �ڷ����� MMDD                                             */
	char    sFileSz   [  12];  /* ���� SIZE    0630�� '000000000000', 0640����� ���� ����  */
	char    sMsgByte  [   4];  /* ���� BYTE ��                                              */
} MSG_LTC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_LTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO      */
	char    sSeqNo    [   3];  /* SEQUENCE NO   */
	char    sSlByte   [   4];  /* ��DATA BYTE�� */
	char    sMsgDtl   [8000];  /* ���� ����     */
} MSG_LTC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MREQ
 */
typedef struct _MSG_0620_LTC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_LTC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MRES
 */
typedef struct _MSG_0300_LTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_LTC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_LTC_  /* header + (110) */
{
	MSG_LTC_HDR  stHdr   ;

	char         sData[1];
} MSG_LTC_LINE;




/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 *  ������� : ����ī��
 */
typedef struct _MSG_HDR_SHC_  /* (27) */
{
	char    sMsgLen   [   4];  /* TCP/IP �����۽� Byte��                      */
	char    sBizKnd   [   3];  /* ���������ڵ� 'EFT'                          */
	char    sLength   [   4];  /* ����SIZE Byte�� = sMsgLen                   */
	char    sOrgzCd   [   2];  /* ȸ�����ڵ�. 02(�����ڵ�)                    */
	char    sMsgCls   [   4];  /* ���������ڵ�                                */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S'����۽�, 'R'ī���۽�     */
	char    sSrFlag   [   1];  /* �����߻����.�ۼ���Flag 'C'��� 'B'ī���   */
	char    sFileNm   [   8];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE     */
	char    sRespCd   [   3];  /* �����ڵ�                                    */
} MSG_SHC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_SHC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss                                                  */
	char    sBizCls   [   3];  /* ������������ 001(��������),002(�̾����),003(��������),004(��������)   */
	char    sSender   [  20];  /* �۽��ڸ�   SPACE                                                         */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ SPACE                                                         */
} MSG_SHC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_SHC_  /* header + (24) */
{
	char    sFileId   [   4];  /* ���� NAME                                                 */
	char    sCdDate   [   4];  /* �ڷ����� MMDD                                             */
	char    sFileSz   [  12];  /* ���� SIZE    0630�� '000000000000', 0640����� ���� ����  */
	char    sMsgByte  [   4];  /* ���� BYTE ��                                              */
} MSG_SHC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_SHC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO      */
	char    sSeqNo    [   3];  /* SEQUENCE NO   */
	char    sSlByte   [   4];  /* ��DATA BYTE�� */
	char    sMsgDtl   [8000];  /* ���� ����     */
} MSG_SHC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MREQ
 */
typedef struct _MSG_0620_SHC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_SHC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MRES
 */
typedef struct _MSG_0300_SHC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_SHC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_SHC_  /* header + (110) */
{
	MSG_SHC_HDR  stHdr   ;

	char         sData[1];
} MSG_SHC_LINE;




/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  ������� : ����ī��          HDC51
 */
typedef struct _MSG_HDR_HDC_  /* (23) */
{
	/* char    sMsgLen   [   4];*/  /* TCP/IP �����۽� Byte��                  */
	char    sBizKnd   [   3];  /* ���������ڵ� 'KTB'                      */
	char    sOrgzCd   [   8];  /* ��������ڵ� 'HYUNDAI'                  */
	char    sMsgCls   [   4];  /* ���������ڵ�                            */
	char    sTrxCls   [   1];  /* �ŷ������ڵ� 'S', 'R'                   */
	char    sSrFlag   [   1];  /* �ۼ��� Flag C:Center, B:Bank            */
	char    sFileId   [   4];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE */
	char    sCdDate   [   4];  /* �ڷ����� MMDD                           */
	char    sRespCd   [   3];  /* �����ڵ�                                */
} MSG_HDC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 ����  : MSG_MANG
 */
typedef struct _MSG_0600_HDC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* ���������Ͻ� MMDDhhmmss      */
	char    sBizCls   [   3];  /* ������������                 */
	char    sSender   [  20];  /* �۽��ڸ�   SPACE             */
	char    sPasswd   [  16];  /* �۽��ھ�ȣ SPACE             */
} MSG_HDC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 ����  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_HDC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_HDC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 ����  : MSG_FILE
 */
typedef struct _MSG_0630_HDC_  /* header + (24)*/
{
	char    sFileId   [   4];  /* ���ϱ����ڵ�  0600/001, 0600/004, SPACE */
	char    sCdDate   [   4];  /* �ڷ����� MMDD                           */
	char    sFileSz   [  12];  /* ���� SIZE    */
	char    sMsgByte  [   4];  /* ���� BYTE �� */
} MSG_HDC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 ����  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_HDC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* ��� ����    */
	char    sMisBit   [ 100];  /* ��� DATA    */
} MSG_HDC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 ����  : MSG_DATA
 */
typedef struct _MSG_0310_HDC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* ��DATA BYTE��*/
	char    sMsgDtl   [8000];  /* ���� ����    */
} MSG_HDC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_LINE_HDC_  /* header + (110) */
{
	MSG_HDC_HDR  stHdr   ;

	char         sData[1];
} MSG_HDC_LINE;






/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 *  ������� : �Ｚ��Ʈ��
 *
 *    ������ȣ(sTranNum)
 *        003 : LOGIN ��û    (�����  -> VAN)
 *        030 : LOGIN ����    (�����  -> VAN)
 *        007 : LOGOUT ��û   (�����  -> VAN)
 *        070 : LOGOUT ����   (�����  -> VAN)
 *        100 : �۽����� �뺸 (����� <-> VAN)
 *        110 : �۽����� ���� (����� <-> VAN)
 *        130 : ���� Ȯ��     (����� <-> VAN)
 *
 *    �����ڵ�(sResCode)
 *        000 : ����
 *        001 : �ý��� ���
 *        002 : USERID ����
 *        003 : PASSWORD ����
 *        004 : JOB TYPE ����
 *        005 : ���ǿ� �´� �ڷ� ����
 *        006 : ���� ���� ����
 *        007 : ���� byte ����
 *        008 : ���� ���� ����
 *        009 : PASSWORD Change ����
 *        010 : ���� ���� ����
 *        099 : ��Ÿ ����
 *
 */
typedef struct _MSG_HDR_SSN_   /* SIZE(40) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* ������ȣ (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* �����ڵ�                                    */
	char    sBizFld   [  20];  /* �����ʵ� (VAN���� Ȱ��)                     */
	char    sReserved [   5];  /* ����                                        */
} MSG_SSN_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  003, 030 ����  : MSG_LOGIN
 *
 *    Code��ȯ����(sChgCode)
 *        A : EBCDIC -> ASCII (SOSI ������ŭ size ����)
 *        E : ASCII -> EBCDIC (SOSI ������ŭ size ����)
 *        S : EBCDIC -> ASCII (SOSI ���� �� size ����ȭ)
 *        T : ASCII -> EBCDIC (SOSI �߰� �� size ����ȭ)
 *        N : �ڵ� ����ȭ
 *
 */
typedef struct _MSG_SSN_LOGIN_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* ������ȣ (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* �����ڵ�                                    */
	char    sBizFld   [  20];  /* �����ʵ� (VAN���� Ȱ��)                     */
	char    sReserved [   5];  /* ����                                        */

	char    sUserid   [  20];   /* VAN �����ID                                   */
	char    sPasswd   [   8];   /* VAN Password                                   */
	char    sJobType  [   2];   /* SD(�۽�), RD(����), RS(�۽�List), RR(����List) */
	char    sSequence [  10];   /* ���Ž� Ư�����ϸ� �����ϰ����� ��              */
	char    sSubject  [  10];   /* ���Ž� Subject (�����̸� ��� ����)            */
	char    sFlag     [   1];   /* ���Ž� E(�̼���),R(�����),A(�̼���+�����)    */
	char    sStTime   [  12];   /* ���Ž� YYYYMMDDhhmm                            */
	char    sEdTime   [  12];   /* ���Ž� YYYYMMDDhhmm                            */
	char    sYnChgPw  [   1];   /* Y : change password                            */
	char    sNewPass  [   8];   /* new password                                   */
	char    sCommSize [   4];   /* default : 2048                                 */
	char    sChgCode  [   1];   /* Code��ȯ����                                   */
	char    sTransId  [  10];   /* �ŷ�óID                                       */
	char    sFiller   [  11];   /* ����                                           */
} MSG_SSN_LOGIN;

/*----------------------------------------------------------------------------*/
/*
 *  007, 070 ����  : MSG_LOGOUT
 *
 *    Code��ȯ����(sChgCode)
 *        A : EBCDIC -> ASCII (SOSI ������ŭ size ����)
 *        E : ASCII -> EBCDIC (SOSI ������ŭ size ����)
 *        S : EBCDIC -> ASCII (SOSI ���� �� size ����ȭ)
 *        T : ASCII -> EBCDIC (SOSI �߰� �� size ����ȭ)
 *        N : �ڵ� ����ȭ
 *
 */
typedef struct _MSG_SSN_LOGOUT_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* ������ȣ (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* �����ڵ�                                    */
	char    sBizFld   [  20];  /* �����ʵ� (VAN���� Ȱ��)                     */
	char    sReserved [   5];  /* ����                                        */

	char    sUserid   [  20];   /* VAN �����ID                                   */
	char    sPasswd   [   8];   /* VAN Password                                   */
	char    sJobType  [   2];   /* SD(�۽�), RD(����), RS(�۽�List), RR(����List) */
	char    sSequence [  10];   /* ���Ž� Ư�����ϸ� �����ϰ����� ��              */
	char    sSubject  [  10];   /* ���Ž� Subject (�����̸� ��� ����)            */
	char    sFlag     [   1];   /* ���Ž� E(�̼���),R(�����),A(�̼���+�����)    */
	char    sStTime   [  12];   /* ���Ž� YYYYMMDDhhmm                            */
	char    sEdTime   [  12];   /* ���Ž� YYYYMMDDhhmm                            */
	char    sFiller   [  35];   /* ����                                           */
} MSG_SSN_LOGOUT;

/*----------------------------------------------------------------------------*/
/*
 *  100, 110 ����  : MSG_FILE
 *
 */
typedef struct _MSG_SSN_FILE_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* ������ȣ (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* �����ڵ�                                    */
	char    sBizFld   [  20];  /* �����ʵ� (VAN���� Ȱ��)                     */
	char    sReserved [   5];  /* ����                                        */

	char    sSequence [  10];   /* ���Ž� Ư�����ϸ� �����ϰ����� ��              */
	char    sSubject  [  10];   /* ���Ž� Subject (�����̸� ��� ����)            */
	char    sFileSize [  10];   /* �۽��� ������ size                             */
	char    sSendId   [  20];   /* �۽��� ID                                      */
	char    sRecvId   [  20];   /* ������ ID                                      */
	char    sFiller1  [   1];   /* ����-1                                         */
	char    sFiller2  [  10];   /* ����-2                                         */
	char    sYnLast   [   3];   /* ���������� ���� NXT:�������� END:��������      */
	char    sTranType [   3];   /* �������� : 'NEW'                               */
	char    sFiller3  [   1];   /* ����-3                                         */
	char    sFiller4  [  22];   /* ����-4                                         */
} MSG_SSN_FILE;

/*----------------------------------------------------------------------------*/
/*
 *  130 ����  : MSG_CHECK
 *
 */
typedef struct _MSG_SSN_CHECK_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* ������ȣ (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* �����ڵ�                                    */
	char    sBizFld   [  20];  /* �����ʵ� (VAN���� Ȱ��)                     */
	char    sReserved [   5];  /* ����                                        */

	char    sSequence [  10];   /* ���Ž� Ư�����ϸ� �����ϰ����� ��              */
	char    sSubject  [  10];   /* ���Ž� Subject (�����̸� ��� ����)            */
	char    sFileSize [  10];   /* �۽��� ������ size                             */
	char    sYnTrans  [   1];   /* ó������ : Y.�̻���� N.�̻�����               */
	char    sFiller   [  79];   /* ����                                           */
} MSG_SSN_CHECK;


/*----------------------------------------------------------------------------*/
/*
 *  ����  : MSG_LINE
 */
typedef struct _MSG_SSN_LINE_
{
	char    sHdrType  [  10];   /* �������                                       */
	char    sBizType  [   1];   /* ��������                                       */
	char    sDatType  [   1];   /* �ڷ�����                                       */
	char    sFileSize [  10];   /* ����ũ��                                       */
	char    sCompSize [  10];   /* ����������ũ��                                 */
	char    sSendId   [  10];   /* �۽���ID                                       */
	char    sUpcheGb  [   2];   /* ��ü����                                       */
	char    sInfoGb   [   3];   /* ��������                                       */
	char    sFiller   [   1];   /* ����                                           */
	char    sRecvCnt  [   3];   /* �����ڼ�                                       */
	char    sRecvId   [  10];   /* ������ID                                       */
	char    sRecSep   [   2];   /* ���ڵ屸����                                   */

	char    sData     [   1];   /* ������ �ڷ� MAX = 1985                         */
} MSG_SSN_LINE;



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * MAP �� ���� ����ü : �̻��
 */
typedef struct _MAP_
{                                  /*                                 */
	char    sSequence      [ 8];   /* ��������                        */
	char    sReceiver      [10];   /* ó�� �����̰�, ���� ���� ����� */
	char    sDataLen       [ 5];   /* sData�� �����̰� '\n'���� ����  */

	char    sData          [ 1];   /* �����ϰ����ϴ� ������           */
} MAP;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * Mapper�� �����ڷᱸ��
 */
typedef struct _IBRIDGE_RPC_REQ_
{                                  /*                                 */
	char    sLength        [ 4];   /* ��������                        */
	char    sVersion       [ 4];   /* ������ ���� M1.0                */
	char    sClosure       [32];   /* session ����                    */
	char    sHint          [10];   /* MSGID  ��) X_KBB01, I_KBB01     */

	char    sData          [ 1];   /* �����ϰ����ϴ� ������           */
} IBRIDGE_RPC_REQ;

/*----------------------------------------------------------------------------*/
/*
 * Mapper�� �۽��ڷᱸ��
 */
typedef struct _IBRIDGE_RPC_RES_
{                                  /*                                 */
	char    sLength        [ 4];   /* ��������                        */
	char    sVersion       [ 4];   /* ������ ���� M1.0                */
	char    sClosure       [32];   /* session ����                    */
	char    sStatus        [ 4];   /* �����ڵ�                        */

	char    sData          [ 1];   /* �����ϰ����ϴ� ������           */
} IBRIDGE_RPC_RES;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*
 * SC���� (504 bytes) - len (4) + header (57) + data (443)
 */
typedef struct _SCB_DATA_
{
	char sMsgLen       [  4];  /* ��������                          */
	char sTrCode       [  9];  /* transaction code                  */
	char sSystemType   [  3];  /* "STK"                             */
	char sServiceType  [  3];  /* "DDD":�������, "GGG":��������    */
	char sCompanyCode  [ 10];  /* ��ü��ȣ:���ǻ��ȣ(7)+SPACE(3)   */
	char sSeqNo        [  7];  /* �����Ϸù�ȣ                      */
	char sMsgType      [  4];  /* ���������ڵ�                      */
	char sJobCode      [  3];  /* ���������ڵ�                      */
	char sSrFlag       [  1];  /* �ۼ��ű��� (����:B ����:E)        */
	char sReplyCode    [  3];  /* �����ڵ�                          */
	char sTransDateTime[ 14];  /* �ۼ����Ͻ� (YYYYMMDDhhmmss)       */
	char sData         [443];  /* data                              */
} SCB_DATA;



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

typedef struct _RUN_TIME_
{
	char sBeg          [ 10];  /* ���۽ð� (HHMM)                   */
	char sFin          [ 10];  /* ����ð� (HHMM)                   */
} RUN_TIME;

/*
 * biz.properties ���� ����ü
 */
typedef struct _BIZ_PROP_
{
	char sSvcId        [ 20];  /* ���� ID                         */
	char sDesc         [128];  /* ���� Description                  */
	char sObsFlag      [  5];  /* ���� �÷���                       */
	char sRunScpt      [128];  /* ���� ��ũ��Ʈ                     */
	char sRunSvr       [ 20];  /* ���� ���� (AP/FEP)                */
	char sRunTime      [128];  /* ���� �ð� (0~HH:MM~HH:MM)         */

	RUN_TIME  rt       [  7];  /* ���Ϻ�RunTime 0(�Ͽ���) 6(�����) */
} BIZ_PROP;

/*
 * proc ���� ����ü
 */
typedef struct _PROC_INFO_
{
	char sSvcId        [ 20];  /* ���� ID                         */
	char sProcess      [128];  /* �������μ���                      */
	char sPid          [ 20];  /* PID ���ڿ�                        */
	int  iPid               ;  /* PID                               */
} PROC_INFO;


/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

#define     SOCK_TYPE_NOTCONNECT     0
#define     SOCK_TYPE_SERVER         1
#define     SOCK_TYPE_CLIENT         2

#define     CONN_TYPE_NOTYPE         0
#define     CONN_TYPE_ACCEPT         1
#define     CONN_TYPE_CONNECT        2

/*----------------------------------------------------------------------------*/
/*
 *  Socket Structure
 */
typedef struct _SOCK_INFO_
{
	int                   type        ;     /* socket type                    */
	int                   conntype    ;     /* connect type                   */

	int                   sock        ;     /* socket                         */
	int                   port        ;     /* port                           */
	char                  ipaddr[20]  ;     /* ip address                     */
	struct sockaddr_in    addr        ;     /* socket address                 */

	int                   ssiNo       ;     /* client���� ���. ���� ������ȣ */

	time_t                lastRecved  ;     /* poll sec �����                */
	int                   iPollFlag   ;     /* poll ����                      */

} SOCK_INFO;

/*----------------------------------------------------------------------------*/
/*
 *  Ip Port Structure
 */
typedef struct _IPPORT_INFO_
{
	char                  sIpAddr [20];     /* ip address                     */
	char                  sPort   [10];     /* string port                    */
	int                   iPort       ;     /* integer port                   */
} IPPORT_INFO;

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 *  IBnet Conf Structure
 */

typedef struct
{
	char                  sSvcId    [ 20+1];   /* Service ID                  */

	char                  sUseFlag  [  1+1];   /* Use Flag                    */

	char                  sNetInfo  [200+1];   /* Net Socket Info             */
	char                  sIpAddr   [ 20+1];   /* ip address                  */
	char                  sPort     [ 10+1];   /* string port                 */
	int                   iPort            ;   /* integer port                */

	char                  sProcType [  1+1];   /* Process Type                */

	char                  sTimeOut  [ 10+1];   /* TimeOut sec                 */
	char                  sPollTime [ 10+1];   /* PollTime sec                */

	int                   iTimeOut         ;   /* TimeOut sec                 */
	int                   iPollTime        ;   /* PollTime sec                */

	char                  sExeInfo  [200+1];   /* Execute Info                */

} IBNET_CONF;

/*----------------------------------------------------------------------------*/
/*
 *  IBnet Socket Structure
 */

#define     IBNET_TYPE_NOTCONNECT          0
#define     IBNET_TYPE_SERVER              1
#define     IBNET_TYPE_ACCEPT              2
#define     IBNET_TYPE_CONNECT             3

typedef struct
{
	int                   type             ;   /* socket type                 */

	int                   sock             ;   /* socket                      */
	int                   lport            ;   /* listen port                 */
	int                   port             ;   /* port                        */
	char                  ipaddr    [ 20+1];   /* ip address                  */
	struct sockaddr_in    addr             ;   /* socket address              */

	int                   iTimeOut         ;   /* TimeOut sec                 */
	int                   iPollTime        ;   /* PollTime sec                */

	int                   lastTime         ;   /* last action time            */

	char                  sExeInfo  [200+1];   /* Execute Info                */
	int                   pid              ;   /* PID                         */

} IBNET_SOCK_INFO;


/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 *  Ums Config Structure
 */

typedef struct
{
	char                  sUseYn    [  1+1];   /* ��뿩�� : Y / N          */
	char                  sEmpNo    [ 10+1];   /* �����ȣ : 999999999      */
	char                  sTmCode   [ 10+1];   /* �����ڵ� : 21085          */
	char                  sName     [ 20+1];   /* �̸�     : �ֿ���         */
	char                  sPhone    [ 20+1];   /* ��ȭ��ȣ : 0237727804     */
} UMS_CFG;

/*----------------------------------------------------------------------------*/
/*
 *  Ums File Structure (109)
 */

typedef struct
{
	char                  sWriteTm  [   14];   /* ����Ͻ�(YYYYMMDDhhmmss)  */
	char                  sSendTm   [   14];   /* �߼��Ͻ�(YYYYMMDDhhmmss)  */
	char                  sSendStat [    6];   /* �߼ۻ���                  */
	char                  sMessage  [  160];   /* �޽���:�ѱ�40��(UTF-8)    */
	char                  sLF       [    1];   /* LF(0x0A)                  */
} UMS_FILE;

/*----------------------------------------------------------------------------*/
/*
 *  Ums Send Structure (266) : �������ڵ� : FEP(��ŷ) -> SA00010
 */

typedef struct
{
	char            sWorkDay        [    8];   /* �߼�����(YYYYMMDD)        */
	char            sUmsSendType    [    3];   /* UMS�߼۱����ڵ� : SYS     */
	char            sChannelCode    [    3];   /* UMS���������ڵ� : 012     */
	char            sWorkCode       [    7];   /* UMS�������ڵ� : SA00010 */
	char            sToId           [    9];   /* �����ȣ : 999999999      */
	char            sToName         [   20];   /* �����θ�                  */
	char            sToPhone        [   11];   /* ��������ȭ��ȣ            */
	char            sFromId         [    9];   /* �߼ۻ����ȣ : 999999999  */
	char            sFromDept       [    5];   /* �߽��������ڵ� : 21085    */
	char            sFromName       [   20];   /* �߽��ڸ� : �ֿ���         */
	char            sFromPhone      [   11];   /* �߽�����ȭ��ȣ:0237727804 */
	char            sContent        [  160];   /* SMS ���� : �ѱ�40/����80  */
} UMS_SEND;

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 *  Log Del Config Structure
 */

typedef struct
{
	char            sObjPath        [128+1];   /* �������                    */
	int             iExpNo                 ;   /* Expire Number               */
	char            sPattern        [128+1];   /* ���ϰ˻� ����               */
} LOG_DEL;


/*----------------------------------------------------------------------------*/
/*
 *  Log Zip Config Structure
 */

typedef struct
{
	char            sObjPath        [128+1];   /* �������                    */
	int             iExpNo                 ;   /* Expire Number               */
	char            sPattern        [128+1];   /* ���ϰ˻� ����               */
} LOG_ZIP;

/*----------------------------------------------------------------------------*/
/*
 *  Log Sec Config Structure
 */

typedef struct
{
	char            sObjPath        [128+1];   /* �������                    */
	int             iExpNo                 ;   /* Expire Number               */
	char            sPattern        [128+1];   /* ���ϰ˻� ����               */
} LOG_SEC;


/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

#endif


