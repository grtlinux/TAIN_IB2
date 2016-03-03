/*
 * PROGRAM ID   : Structure.h
 * PROGRAM NAME : 구조체 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : 공통적으로 사용하는 구조체를 선언한다.
 * HISTORY      : 2013.07.24 강석. 최초작성
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
 * Select 를 위한 구조체
 *
 *    chType
 *        I : Input  (기본)
 *        O : Output (기본)
 *        E : Error  (기본)
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
 * 파일잠금/해지를 위한 구조체
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
 * TCP/IP 간단한 송수신 구조체
 */
typedef struct _PACKET_
{
	char    sLen       [4];   /* packet 길이 - 길이필드(4) */
	char    sData      [1];   /* 실제 전송자료 */
} PACKET;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * File Queue 를 위한 구조체
 */
typedef struct _FILE_QUEUE_
{
	char    strFepid      [  5+1]; /* FEPID       */
	char    strApno       [  2+1]; /* AP서버 번호 */

	char    chSendRecv           ; /* 송수신구분   */
	char    chWriteRead          ; /* 쓰기읽기구분 */

	int     iWriteSeq            ; /* write sequence  */
	int     iReadSeq             ; /* read sequence   */
	long    lRPos                ; /* read position   */

	FILE*   pFpSeq               ; /* seq file  */
	FILE*   pFpDat               ; /* data file */
} FQ;

/*----------------------------------------------------------------------------*/
/*
 * File Queue SEQ 를 위한 구조체
 */
typedef struct _FILE_QUEUE_SEQ_
{
	char    strWriteSeq    [ 8];   /* write sequence     */
	char    strReadSeq     [ 8];   /* read sequence      */
	long    lRPos              ;   /* long read position */
} FQSEQ;

/*----------------------------------------------------------------------------*/
/*
 * File Queue DAT 를 위한 구조체
 */
typedef struct _FILE_QUEUE_DAT_
{                                /*                                  */
	char    sSequence      [ 8];   /* 전문순번                        */
	char    sReceiver      [10];   /* 처음 공백이고, 읽은 놈이 기록함 */
	char    sDataLen       [ 5];   /* sData의 길이이고 '\n'까지 포함  */

	char    sData          [ 1];   /* 전송하고자하는 데이터 */
} FQDAT;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * 내용 : 파일큐2의 객체를 오픈한다.
 */
typedef struct _FILE_QUEUE2_
{
	char    sFqNm         [  3+1]; /* FQ Name                          */
	char    sSvcid        [ 10+1]; /* Service ID                       */
	char    sFepid        [  5+1]; /* FEPID                            */
	char    sApno         [  2+1]; /* AP서버 번호                      */

	char    chSR                 ; /* 송수신구분                       */
	char    chWR                 ; /* 쓰기읽기구분                     */

	int     iWSeq                ; /* write sequence                   */
	int     iRSeq                ; /* read sequence                    */

	long    lRPos                ; /* read position                    */

	FILE*   fpSeq                ; /* seq file                         */
	FILE*   fpDat                ; /* data file                        */
} FQ2;

/*----------------------------------------------------------------------------*/
/*
 * File Queue 2 SEQ 를 위한 구조체
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
 * File Queue 2 DAT 를 위한 구조체
 */
typedef struct _FILE_QUEUE2_DAT_
{
	char    sSeq           [ 8];   /* 전문순번                         */
	char    sRecvr         [10];   /* 처음 공백이고, 읽은 놈이 기록함  */
	char    sLen           [ 5];   /* sData의 길이이고 '\n'까지 포함   */

	char    sData          [ 1];   /* 전송하고자하는 데이터            */
} FQ2DAT;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * 내용 : 파일큐2의 객체를 오픈한다.
 */
typedef struct _FILE_QUEUE3_
{
	char    sFqNm         [  3+1]; /* FQ Name                          */
	char    sSvcid        [ 10+1]; /* Service ID                       */
	char    sFepid        [  5+1]; /* FEPID                            */
	char    sApno         [  2+1]; /* AP서버 번호                      */

	char    chSR                 ; /* 송수신구분                       */
	char    chWR                 ; /* 쓰기읽기구분                     */

	int     iWSeq                ; /* write sequence                   */
	int     iRSeq                ; /* read sequence                    */

	long    lRPos                ; /* read position                    */

	FILE*   fpSeq                ; /* seq file                         */
	FILE*   fpDat                ; /* data file                        */
} FQ3;

/*----------------------------------------------------------------------------*/
/*
 * File Queue 2 SEQ 를 위한 구조체
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
 * File Queue 2 DAT 를 위한 구조체
 */
typedef struct _FILE_QUEUE3_DAT_
{
	char    sSeq           [ 8];   /* 전문순번                         */
	char    sRecvr         [10];   /* 처음 공백이고, 읽은 놈이 기록함  */
	char    sLen           [ 5];   /* sData의 길이이고 '\n'까지 포함   */

	char    sData          [ 1];   /* 전송하고자하는 데이터            */
} FQ3DAT;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*
 * ##### FEP 배치파일 송신 #####
 *
 *  1. 배치FQ를 읽는다.
 *      BAT_INF(100) 정보를 읽는다.
 *
 *          typedef struct _BAT_INF_
 *          {
 *              char    sRecLen     [ 4]; // 레코드 길이
 *              char    sRecCnt     [ 7]; // 레코드 건수
 *              char    sSR         [ 1]; // 송수신 구분
 *              char    sOrgCode    [ 3]; // 기관코드
 *              char    sTrCode     [11]; // 일괄파일 TR Code
 *              char    sTrCodeLen  [ 2]; // 일괄파일 TR Code 길이
 *              char    sDate       [ 8]; // 전문전송일자 YYYYMMDD
 *              char    sTime       [ 9]; // 전문전송시각 hhmmssSSS
 *              char    sTrCodeSeq  [ 2]; // 일자별 TR Code 회차
 *              char    sFiller     [53]; // 예비
 *          } BAT_INF;
 *
 *  2. 배치송신 개시
 *      송신:BABG : FEP_HDR(50:BABG) + BAT_INF(100)    ->
 *      수신:BABG : FEP_HDR(50:BABG)                   <-
 *
 *  3. 배치파일 송신(반복 레코드송신)
 *      송신:DATA : FEP_HDR(50:DATA) + sDataLine(가변)  ->
 *      수신:DAOK : FEP_HDR(50:DAOK)                    <-
 *
 *  4. 배치송신 마감
 *      송신:BAED : FEP_HDR(50:BAED)    ->
 *      수신:BAED : FEP_HDR(50:BAED)    <-
 *
 * ##### FEP 배치파일 수신 #####
 *
 *  1. 배치송신 개시
 *      송신:BABG : FEP_HDR(50:BABG) + BAT_INF(100)    ->
 *      수신:BABG : FEP_HDR(50:BABG)                   <-
 *
 *  2. 배치파일 송신(반복 레코드송신)
 *      송신:DATA : FEP_HDR(50:DATA) + sDataLine(가변)  ->
 *      수신:DAOK : FEP_HDR(50:DAOK)                    <-
 *
 *  3. 배치송신 마감
 *      송신:BAED : FEP_HDR(50:BAED)    ->
 *      수신:BAED : FEP_HDR(50:BAED)    <-
 *
 *  4. 배치FQ를 읽는다.
 *      BAT_INF(100) 정보를 쓴다.
 *
 *          typedef struct _BAT_INF_
 *          {
 *              char    sRecLen     [ 4]; // 레코드 길이
 *              char    sRecCnt     [ 7]; // 레코드 건수
 *              char    sSR         [ 1]; // 송수신 구분
 *              char    sOrgCode    [ 3]; // 기관코드
 *              char    sTrCode     [11]; // 일괄파일 TR Code
 *              char    sTrCodeLen  [ 2]; // 일괄파일 TR Code 길이
 *              char    sDate       [ 8]; // 전문전송일자 YYYYMMDD
 *              char    sTime       [ 9]; // 전문전송시각 hhmmssSSS
 *              char    sTrCodeSeq  [ 2]; // 일자별 TR Code 회차
 *              char    sFiller     [53]; // 예비
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

#define TC_BABG "BABG" /* 배치프로세스만 이용 */
#define TC_BAED "BAED" /* 배치프로세스만 이용 */
#define TC_RSEQ "RSEQ" /* 백업프로세스만 이용 */

/*----------------------------------------------------------------------------*/
/*
 *  FEP전문 Fep Header  (50 B)
 * 세션송수신시 사용함.
 *
 */
typedef struct _FEP_HDR_50_
{
	char    sLength     [ 4];  /* 전문길이    : 전문길이 Field를 제외한 나머지 Length                 */
	char    sBsnCode    [ 6];  /* 업무식별코드: KBBS01                                                */
	char    sTrCode     [ 4];  /* TR-CODE     : LINK, LIOK, DATA, DAOK, POLL, POOK, RJCT 등           */
	char    sResCode    [ 2];  /* 응답코드    : '00'5. 장애응답코드표 참조                            */
	char    sSeq        [ 8];  /* 전문일련번호: '99999999' ? 전문SEQ번호                              */
	char    sDataCnt    [ 2];  /* Data건수    : 01 ~ (2 KByte 이내)                                   */
	char    sDateTime   [20];  /* 시각        : YYYYMMDDHHMMSSssssss                                  */
	char    sAckYn      [ 1];  /* ACK구분     : Data에 대한 ACK : 사용(Y), 미사용(N) LINK시에 결정 됨 */
	char    sFiller     [ 3];
} FEP_HDR;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  FEP전문 Biz Header  (100 B)
 * 자료의 헤더로 사용함
 *
 */
typedef struct _BIZ_HDR_100_
{
	char    sDataLength [ 4];  /* 본 Length FIELD를 제외한 개별 DATA SIZE */
	char    sDataSeq    [ 8];  /* 해당 Data의 일련번호                    */
	char    sGlobalId   [32];  /* GUID                                    */
	char    sDateTime   [20];  /* 시각        : YYYYMMDDHHMMSSssssss */
	char    sRespCode   [ 4];
	char    sMediaTp    [ 2];
	char    sMediaSeq   [ 1];
	char    sFiller     [29];
} BIZ_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  FEP전문 Biz Data
 * FQ에 쓰고 읽는데 사용하는 구조체
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
 *  FEP전문 송수신 Data
 * 세션을 이용한 자료의 송수신시에 사용함.
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
 *  FEP전문 Batch Info  (100 B)
 * FQ에 쓰고 읽는데 사용하는 구조체
 *
 *  BAT_INFO(100)
 *
 * 파일명 : sSR_sOrgCode_sTrCode_sDate_sTrCodeSeq
 *
 */
typedef struct _BAT_INF_
{
	char    sRecLen     [ 4]; /* 레코드 길이              */
	char    sRecCnt     [ 7]; /* 레코드 건수              */
	char    sSR         [ 1]; /* 송수신 구분              */
	char    sOrgCode    [ 3]; /* 기관코드                 */
	char    sTrCode     [11]; /* 일괄파일 TR Code         */
	char    sTrCodeLen  [ 2]; /* 일괄파일 TR Code 길이    */
	char    sDate       [ 8]; /* 전문전송일자 YYYYMMDD    */
	char    sTime       [ 9]; /* 전문전송시각 hhmmssSSS   */
	char    sTrCodeSeq  [ 2]; /* 일자별 TR Code 회차      */
	char    sFiller     [53]; /* 예비                     */
} BAT_INF;

/*----------------------------------------------------------------------------*/
/*
 *  FEP전문 Batch Info Data (150 B)
 * 세션을 이용한 자료의 송수신시에 사용함.
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
 *  FEP전문 Batch Data
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

#define  MSGTYPE_OPEN 10  /* 업무개시 */
#define  MSGTYPE_FILE 20  /* 파일정보 */
#define  MSGTYPE_DATA 30  /* DATA     */
#define  MSGTYPE_MISS 40  /* 결번     */
#define  MSGTYPE_MDAT 50  /* 결번DATA */
#define  MSGTYPE_CONT 60  /* 파일계속 */
#define  MSGTYPE_FEND 70  /* 파일종료 */
#define  MSGTYPE_DONE 80  /* 업무종료 */

/*----------------------------------------------------------------------------*/
/*
 *  전문헤더 : 금융결제원, 카드사매입정보                  KFT1
 */
typedef struct _MSG_HDR_KFT1_  /* (23) */
{
	char    sMsgLen   [   4];  /* TCP/IP 전문송신 Byte수                  */
	char    sBizKnd   [   3];  /* 업무구분코드 'KTB'                      */
	char    sOrgzCd   [   3];  /* 금융기관코드 '269'                      */
	char    sMsgCls   [   4];  /* 전문종별코드                            */
	char    sTrxCls   [   1];  /* 거래구분코드 'S', 'R'                   */
	char    sSrFlag   [   1];  /* 송수신 Flag C:Center, B:Bank            */
	char    sFileNm   [   8];  /* 파일구분코드  0600/001, 0600/004, SPACE */
	char    sRespCd   [   3];  /* 응답코드                                */
} MSG_KFT1_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_KFT1_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss      */
	char    sBizCls   [   3];  /* 업무관리정보                 */
	char    sSender   [  20];  /* 송신자명                     */
	char    sPasswd   [  16];  /* 송신자암호                   */
} MSG_KFT1_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_KFT1_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_KFT1_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_KFT1_  /* header + (24)*/
{
	char    sFileId   [   8];  /* 파일 NAME    */
	char    sFileSz   [  12];  /* 파일 SIZE    */
	char    sMsgByte  [   4];  /* 전문 BYTE 수 */
} MSG_KFT1_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_KFT1_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_KFT1_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_KFT1_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* 실DATA BYTE수*/
	char    sMsgDtl   [8000];  /* 파일 내역    */
} MSG_KFT1_0310;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
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
 *  전문헤더 : 금융결제원(이용기관용)                  KFT2
 */
typedef struct _MSG_HDR_KFT2_  /* (32) */
{
	char    sMsgLen   [   4];  /* TCP/IP 전문송신 Byte수                  */
	char    sBizKnd   [   3];  /* 업무구분코드 'FTE'                      */
	char    sOrgzCd   [   8];  /* 기관코드                                */
	char    sMsgCls   [   4];  /* 전문구분코드                            */
	char    sTrxCls   [   1];  /* 거래구분코드 'S', 'R'                   */
	char    sSrFlag   [   1];  /* 송수신 Flag 'C', 'B' -> B               */
	char    sFileNm   [   8];  /* 파일구분코드  0600/001, 0600/004, SPACE */
	char    sRespCd   [   3];  /* 응답코드                                */
} MSG_KFT2_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_KFT2_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss      */
	char    sBizCls   [   3];  /* 업무관리정보                 */
	char    sSender   [  20];  /* 송신자명                     */
	char    sPasswd   [  16];  /* 송신자암호                   */
} MSG_KFT2_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_KFT2_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_KFT2_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_KFT2_  /* header + (24)*/
{
	char    sFileId   [   8];  /* 파일 NAME    */
	char    sFileSz   [  12];  /* 파일 SIZE    */
	char    sMsgByte  [   4];  /* 전문 BYTE 수 */
} MSG_KFT2_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_KFT2_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_KFT2_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_KFT2_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* 실DATA BYTE수*/
	char    sMsgDtl   [8000];  /* 파일 내역    */
} MSG_KFT2_0310;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
 */
typedef struct _MSG_LINE_KFT2_  /* header + (110) */
{
	MSG_KFT2_HDR  stHdr   ;

	char         sData[1];
} MSG_KFT2_LINE;


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  송신자 암호 입력
 */
typedef struct _PASSWORD_I_
{
	int     iProcTp         ;  /* 처리구분 1:암호화 2:복호화   */
	char    sUserId   [20+1];  /* 송사지명                     */
	char    sUserPw   [20+1];  /* 송신자암호                   */
	char    sBizYmd   [20+1];  /* 거래일                       */
	char    sOrgzCd   [20+1];  /* 이용기관식별코드             */
	char    sEnCode   [20+1];  /* 송신자암호 암호문            */
} PASSWORD_I;

/*----------------------------------------------------------------------------*/
/*
 *  송신자 암호 출력
 */
typedef struct _PASSWORD_O_
{
	char    sUserPw   [20+1];  /* 송신자암호        */
	char    sEnCode   [20+1];  /* 송신자암호 암호문 */
} PASSWORD_O;



/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 *  전문헤더 : KS-NET                  KSN
 */
typedef struct _MSG_HDR_KSN_  /* (23) */
{
	char    sMsgLen   [   4];  /* TCP/IP 전문송신 Byte수                  */
	char    sBizKnd   [   3];  /* 업무구분코드 'KTB'                      */
	char    sOrgzCd   [   3];  /* 금융기관코드 '269'                      */
	char    sMsgCls   [   4];  /* 전문종별코드                            */
	char    sTrxCls   [   1];  /* 거래구분코드 'S', 'R'                   */
	char    sSrFlag   [   1];  /* 송수신 Flag C:Center, B:Bank            */
	char    sFileNm   [   8];  /* 파일구분코드  0600/001, 0600/004, SPACE */
	char    sRespCd   [   3];  /* 응답코드                                */
} MSG_KSN_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_KSN_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss      */
	char    sBizCls   [   3];  /* 업무관리정보                 */
	char    sSender   [  20];  /* 송신자명                     */
	char    sPasswd   [  16];  /* 송신자암호                   */
} MSG_KSN_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_KSN_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_KSN_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_KSN_  /* header + (24)*/
{
	char    sFileId   [   8];  /* 파일 NAME    */
	char    sFileSz   [  12];  /* 파일 SIZE    */
	char    sMsgByte  [   4];  /* 전문 BYTE 수 */
} MSG_KSN_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_KSN_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_KSN_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_KSN_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* 실DATA BYTE수*/
	char    sMsgDtl   [8000];  /* 파일 내역    */
} MSG_KSN_0310;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
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
 *  전문헤더 : 시티카드                  CTC
 */
typedef struct _MSG_HDR_CTC_  /* (23) */
{
	char    sMsgLen   [   4];  /* TCP/IP 전문송신 Byte수                  */
	char    sBizKnd   [   3];  /* 업무구분코드 'FTE'                      */
	char    sOrgzCd   [   8];  /* 금융기관코드 'LOTTE   '                 */
	char    sMsgCls   [   4];  /* 전문종별코드                            */
	char    sTrxCls   [   1];  /* 거래구분코드 'S', 'R'                   */
	char    sSrFlag   [   1];  /* 송수신 Flag C:Center, B:Bank            */
	char    sFileNm   [   4];  /* 파일구분코드  0600/001, 0600/004, SPACE */
	char    sCdDate   [   4];  /* 자료일자 MMDD                           */
	char    sRespCd   [   3];  /* 응답코드                                */
} MSG_CTC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_CTC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss      */
	char    sBizCls   [   3];  /* 업무관리정보                 */
	char    sSender   [  20];  /* 송신자명                     */
	char    sPasswd   [  16];  /* 송신자암호                   */
} MSG_CTC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_CTC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_CTC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_CTC_  /* header + (24)*/
{
	char    sFileId   [   8];  /* 파일 NAME = sFileNm + sCdDate */
	char    sFileSz   [  12];  /* 파일 SIZE    */
	char    sMsgByte  [   4];  /* 전문 BYTE 수 */
} MSG_CTC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_CTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_CTC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_CTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* 실DATA BYTE수*/
	char    sMsgDtl   [8000];  /* 파일 내역    */
} MSG_CTC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
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
 *  전문헤더 : 롯데카드
 */
typedef struct _MSG_HDR_LTC_  /* (32) */
{
	char    sMsgLen   [   4];  /* TCP/IP 전문송신 Byte수                      */
	char    sBizKnd   [   3];  /* 업무구분코드 'FTE'                          */
	char    sOrgzCd   [   8];  /* 기관코드     'LOTTE   '                     */
	char    sMsgCls   [   4];  /* 전문구분코드                                */
	char    sTrxCls   [   1];  /* 거래구분코드 'S'기관송신, 'R'카드사송신     */
	char    sSrFlag   [   1];  /* 송수신 Flag 'C'기관송신, 'B'카드사송신      */
	char    sFileNm   [   4];  /* 파일구분코드  0600/001, 0600/004, SPACE     */
	char    sCdDate   [   4];  /* 자료일자 MMDD                               */
	char    sRespCd   [   3];  /* 응답코드                                    */
} MSG_LTC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_LTC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss                                                  */
	char    sBizCls   [   3];  /* 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)   */
	char    sSender   [  20];  /* 송신자명   SPACE                                                         */
	char    sPasswd   [  16];  /* 송신자암호 SPACE                                                         */
} MSG_LTC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_LTC_  /* header + (24) */
{
	char    sFileId   [   4];  /* 파일 NAME                                                 */
	char    sCdDate   [   4];  /* 자료일자 MMDD                                             */
	char    sFileSz   [  12];  /* 파일 SIZE    0630은 '000000000000', 0640응답시 받은 내용  */
	char    sMsgByte  [   4];  /* 전문 BYTE 수                                              */
} MSG_LTC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_LTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO      */
	char    sSeqNo    [   3];  /* SEQUENCE NO   */
	char    sSlByte   [   4];  /* 실DATA BYTE수 */
	char    sMsgDtl   [8000];  /* 파일 내용     */
} MSG_LTC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MREQ
 */
typedef struct _MSG_0620_LTC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_LTC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MRES
 */
typedef struct _MSG_0300_LTC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_LTC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
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
 *  전문헤더 : 신한카드
 */
typedef struct _MSG_HDR_SHC_  /* (27) */
{
	char    sMsgLen   [   4];  /* TCP/IP 전문송신 Byte수                      */
	char    sBizKnd   [   3];  /* 업무구분코드 'EFT'                          */
	char    sLength   [   4];  /* 전문SIZE Byte수 = sMsgLen                   */
	char    sOrgzCd   [   2];  /* 회원사코드. 02(은행코드)                    */
	char    sMsgCls   [   4];  /* 전문구분코드                                */
	char    sTrxCls   [   1];  /* 거래구분코드 'S'기관송신, 'R'카드사송신     */
	char    sSrFlag   [   1];  /* 전문발생기관.송수신Flag 'C'기관 'B'카드사   */
	char    sFileNm   [   8];  /* 파일구분코드  0600/001, 0600/004, SPACE     */
	char    sRespCd   [   3];  /* 응답코드                                    */
} MSG_SHC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_SHC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss                                                  */
	char    sBizCls   [   3];  /* 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)   */
	char    sSender   [  20];  /* 송신자명   SPACE                                                         */
	char    sPasswd   [  16];  /* 송신자암호 SPACE                                                         */
} MSG_SHC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_SHC_  /* header + (24) */
{
	char    sFileId   [   4];  /* 파일 NAME                                                 */
	char    sCdDate   [   4];  /* 자료일자 MMDD                                             */
	char    sFileSz   [  12];  /* 파일 SIZE    0630은 '000000000000', 0640응답시 받은 내용  */
	char    sMsgByte  [   4];  /* 전문 BYTE 수                                              */
} MSG_SHC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_SHC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO      */
	char    sSeqNo    [   3];  /* SEQUENCE NO   */
	char    sSlByte   [   4];  /* 실DATA BYTE수 */
	char    sMsgDtl   [8000];  /* 파일 내용     */
} MSG_SHC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MREQ
 */
typedef struct _MSG_0620_SHC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_SHC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MRES
 */
typedef struct _MSG_0300_SHC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_SHC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
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
 *  전문헤더 : 현대카드          HDC51
 */
typedef struct _MSG_HDR_HDC_  /* (23) */
{
	/* char    sMsgLen   [   4];*/  /* TCP/IP 전문송신 Byte수                  */
	char    sBizKnd   [   3];  /* 업무구분코드 'KTB'                      */
	char    sOrgzCd   [   8];  /* 금융기관코드 'HYUNDAI'                  */
	char    sMsgCls   [   4];  /* 전문종별코드                            */
	char    sTrxCls   [   1];  /* 거래구분코드 'S', 'R'                   */
	char    sSrFlag   [   1];  /* 송수신 Flag C:Center, B:Bank            */
	char    sFileId   [   4];  /* 파일구분코드  0600/001, 0600/004, SPACE */
	char    sCdDate   [   4];  /* 자료일자 MMDD                           */
	char    sRespCd   [   3];  /* 응답코드                                */
} MSG_HDC_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  0600, 0610 전문  : MSG_MANG
 */
typedef struct _MSG_0600_HDC_  /* header + (49) */
{
	char    sSendDt   [  10];  /* 전문전송일시 MMDDhhmmss      */
	char    sBizCls   [   3];  /* 업무관리정보                 */
	char    sSender   [  20];  /* 송신자명   SPACE             */
	char    sPasswd   [  16];  /* 송신자암호 SPACE             */
} MSG_HDC_0600;

/*----------------------------------------------------------------------------*/
/*
 *  0620 전문  : MSG_MISS_REQ
 */
typedef struct _MSG_0620_HDC_  /* header + (7) */
{
	char    sBlockNo  [   4];  /* BLOCK NO    */
	char    sSeqNo    [   3];  /* SEQUENCE NO */
} MSG_HDC_0620;

/*----------------------------------------------------------------------------*/
/*
 *  0630, 0640 전문  : MSG_FILE
 */
typedef struct _MSG_0630_HDC_  /* header + (24)*/
{
	char    sFileId   [   4];  /* 파일구분코드  0600/001, 0600/004, SPACE */
	char    sCdDate   [   4];  /* 자료일자 MMDD                           */
	char    sFileSz   [  12];  /* 파일 SIZE    */
	char    sMsgByte  [   4];  /* 전문 BYTE 수 */
} MSG_HDC_0630;

/*----------------------------------------------------------------------------*/
/*
 *  0300 전문  : MSG_MISS_RESP
 */
typedef struct _MSG_0300_HDC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sMisCnt   [   3];  /* 결번 갯수    */
	char    sMisBit   [ 100];  /* 결번 DATA    */
} MSG_HDC_0300;

/*----------------------------------------------------------------------------*/
/*
 *  0310, 0320 전문  : MSG_DATA
 */
typedef struct _MSG_0310_HDC_  /* header + (110) */
{
	char    sBlockNo  [   4];  /* BLOCK NO     */
	char    sSeqNo    [   3];  /* SEQUENCE NO  */
	char    sSlByte   [   4];  /* 실DATA BYTE수*/
	char    sMsgDtl   [8000];  /* 파일 내역    */
} MSG_HDC_0310;

/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
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
 *  전문헤더 : 삼성네트웍
 *
 *    전문번호(sTranNum)
 *        003 : LOGIN 요청    (사용자  -> VAN)
 *        030 : LOGIN 응답    (사용자  -> VAN)
 *        007 : LOGOUT 요청   (사용자  -> VAN)
 *        070 : LOGOUT 응답   (사용자  -> VAN)
 *        100 : 송신파일 통보 (사용자 <-> VAN)
 *        110 : 송신파일 응답 (사용자 <-> VAN)
 *        130 : 수신 확인     (사용자 <-> VAN)
 *
 *    응답코드(sResCode)
 *        000 : 정상
 *        001 : 시스템 장애
 *        002 : USERID 오류
 *        003 : PASSWORD 오류
 *        004 : JOB TYPE 오류
 *        005 : 조건에 맞는 자료 없음
 *        006 : 전문 종류 오류
 *        007 : 전송 byte 오류
 *        008 : 전문 형식 오류
 *        009 : PASSWORD Change 오류
 *        010 : 전문 순서 오류
 *        099 : 기타 오류
 *
 */
typedef struct _MSG_HDR_SSN_   /* SIZE(40) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* 전문번호 (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* 응답코드                                    */
	char    sBizFld   [  20];  /* 업무필드 (VAN에서 활용)                     */
	char    sReserved [   5];  /* 예비                                        */
} MSG_SSN_HDR;

/*----------------------------------------------------------------------------*/
/*
 *  003, 030 전문  : MSG_LOGIN
 *
 *    Code변환여부(sChgCode)
 *        A : EBCDIC -> ASCII (SOSI 갯수만큼 size 감소)
 *        E : ASCII -> EBCDIC (SOSI 갯수만큼 size 증가)
 *        S : EBCDIC -> ASCII (SOSI 제거 및 size 무변화)
 *        T : ASCII -> EBCDIC (SOSI 추가 및 size 무변화)
 *        N : 코드 무변화
 *
 */
typedef struct _MSG_SSN_LOGIN_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* 전문번호 (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* 응답코드                                    */
	char    sBizFld   [  20];  /* 업무필드 (VAN에서 활용)                     */
	char    sReserved [   5];  /* 예비                                        */

	char    sUserid   [  20];   /* VAN 사용자ID                                   */
	char    sPasswd   [   8];   /* VAN Password                                   */
	char    sJobType  [   2];   /* SD(송신), RD(수신), RS(송신List), RR(수신List) */
	char    sSequence [  10];   /* 수신시 특정파일만 수신하고자할 때              */
	char    sSubject  [  10];   /* 수신시 Subject (공백이면 모두 수신)            */
	char    sFlag     [   1];   /* 수신시 E(미수신),R(기수신),A(미수신+기수신)    */
	char    sStTime   [  12];   /* 수신시 YYYYMMDDhhmm                            */
	char    sEdTime   [  12];   /* 수신시 YYYYMMDDhhmm                            */
	char    sYnChgPw  [   1];   /* Y : change password                            */
	char    sNewPass  [   8];   /* new password                                   */
	char    sCommSize [   4];   /* default : 2048                                 */
	char    sChgCode  [   1];   /* Code변환여부                                   */
	char    sTransId  [  10];   /* 거래처ID                                       */
	char    sFiller   [  11];   /* 예비                                           */
} MSG_SSN_LOGIN;

/*----------------------------------------------------------------------------*/
/*
 *  007, 070 전문  : MSG_LOGOUT
 *
 *    Code변환여부(sChgCode)
 *        A : EBCDIC -> ASCII (SOSI 갯수만큼 size 감소)
 *        E : ASCII -> EBCDIC (SOSI 갯수만큼 size 증가)
 *        S : EBCDIC -> ASCII (SOSI 제거 및 size 무변화)
 *        T : ASCII -> EBCDIC (SOSI 추가 및 size 무변화)
 *        N : 코드 무변화
 *
 */
typedef struct _MSG_SSN_LOGOUT_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* 전문번호 (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* 응답코드                                    */
	char    sBizFld   [  20];  /* 업무필드 (VAN에서 활용)                     */
	char    sReserved [   5];  /* 예비                                        */

	char    sUserid   [  20];   /* VAN 사용자ID                                   */
	char    sPasswd   [   8];   /* VAN Password                                   */
	char    sJobType  [   2];   /* SD(송신), RD(수신), RS(송신List), RR(수신List) */
	char    sSequence [  10];   /* 수신시 특정파일만 수신하고자할 때              */
	char    sSubject  [  10];   /* 수신시 Subject (공백이면 모두 수신)            */
	char    sFlag     [   1];   /* 수신시 E(미수신),R(기수신),A(미수신+기수신)    */
	char    sStTime   [  12];   /* 수신시 YYYYMMDDhhmm                            */
	char    sEdTime   [  12];   /* 수신시 YYYYMMDDhhmm                            */
	char    sFiller   [  35];   /* 예비                                           */
} MSG_SSN_LOGOUT;

/*----------------------------------------------------------------------------*/
/*
 *  100, 110 전문  : MSG_FILE
 *
 */
typedef struct _MSG_SSN_FILE_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* 전문번호 (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* 응답코드                                    */
	char    sBizFld   [  20];  /* 업무필드 (VAN에서 활용)                     */
	char    sReserved [   5];  /* 예비                                        */

	char    sSequence [  10];   /* 수신시 특정파일만 수신하고자할 때              */
	char    sSubject  [  10];   /* 수신시 Subject (공백이면 모두 수신)            */
	char    sFileSize [  10];   /* 송신할 파일의 size                             */
	char    sSendId   [  20];   /* 송신자 ID                                      */
	char    sRecvId   [  20];   /* 수신자 ID                                      */
	char    sFiller1  [   1];   /* 예비-1                                         */
	char    sFiller2  [  10];   /* 예비-2                                         */
	char    sYnLast   [   3];   /* 마지막파일 여부 NXT:다음있음 END:다음없음      */
	char    sTranType [   3];   /* 전송종류 : 'NEW'                               */
	char    sFiller3  [   1];   /* 예비-3                                         */
	char    sFiller4  [  22];   /* 예비-4                                         */
} MSG_SSN_FILE;

/*----------------------------------------------------------------------------*/
/*
 *  130 전문  : MSG_CHECK
 *
 */
typedef struct _MSG_SSN_CHECK_   /* SIZE(150) */
{
	char    sTrCode   [   9];  /* Transaction Code                            */
	char    sTranNum  [   3];  /* 전문번호 (003,030,007,070,100,110,130)      */
	char    sResCode  [   3];  /* 응답코드                                    */
	char    sBizFld   [  20];  /* 업무필드 (VAN에서 활용)                     */
	char    sReserved [   5];  /* 예비                                        */

	char    sSequence [  10];   /* 수신시 특정파일만 수신하고자할 때              */
	char    sSubject  [  10];   /* 수신시 Subject (공백이면 모두 수신)            */
	char    sFileSize [  10];   /* 송신할 파일의 size                             */
	char    sYnTrans  [   1];   /* 처리여부 : Y.이상없음 N.이상있음               */
	char    sFiller   [  79];   /* 예비                                           */
} MSG_SSN_CHECK;


/*----------------------------------------------------------------------------*/
/*
 *  전문  : MSG_LINE
 */
typedef struct _MSG_SSN_LINE_
{
	char    sHdrType  [  10];   /* 헤더구분                                       */
	char    sBizType  [   1];   /* 업무구분                                       */
	char    sDatType  [   1];   /* 자료형태                                       */
	char    sFileSize [  10];   /* 파일크기                                       */
	char    sCompSize [  10];   /* 압축후파일크기                                 */
	char    sSendId   [  10];   /* 송신자ID                                       */
	char    sUpcheGb  [   2];   /* 업체구분                                       */
	char    sInfoGb   [   3];   /* 정보구분                                       */
	char    sFiller   [   1];   /* 예비                                           */
	char    sRecvCnt  [   3];   /* 수신자수                                       */
	char    sRecvId   [  10];   /* 수신자ID                                       */
	char    sRecSep   [   2];   /* 레코드구분자                                   */

	char    sData     [   1];   /* 파일의 자료 MAX = 1985                         */
} MSG_SSN_LINE;



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * MAP 를 위한 구조체 : 미사용
 */
typedef struct _MAP_
{                                  /*                                 */
	char    sSequence      [ 8];   /* 전문순번                        */
	char    sReceiver      [10];   /* 처음 공백이고, 읽은 놈이 기록함 */
	char    sDataLen       [ 5];   /* sData의 길이이고 '\n'까지 포함  */

	char    sData          [ 1];   /* 전송하고자하는 데이터           */
} MAP;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
 * Mapper의 수신자료구조
 */
typedef struct _IBRIDGE_RPC_REQ_
{                                  /*                                 */
	char    sLength        [ 4];   /* 전문길이                        */
	char    sVersion       [ 4];   /* 관리용 버젼 M1.0                */
	char    sClosure       [32];   /* session 정보                    */
	char    sHint          [10];   /* MSGID  예) X_KBB01, I_KBB01     */

	char    sData          [ 1];   /* 전송하고자하는 데이터           */
} IBRIDGE_RPC_REQ;

/*----------------------------------------------------------------------------*/
/*
 * Mapper의 송신자료구조
 */
typedef struct _IBRIDGE_RPC_RES_
{                                  /*                                 */
	char    sLength        [ 4];   /* 전문길이                        */
	char    sVersion       [ 4];   /* 관리용 버젼 M1.0                */
	char    sClosure       [32];   /* session 정보                    */
	char    sStatus        [ 4];   /* 상태코드                        */

	char    sData          [ 1];   /* 전송하고자하는 데이터           */
} IBRIDGE_RPC_RES;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*
 * SC은행 (504 bytes) - len (4) + header (57) + data (443)
 */
typedef struct _SCB_DATA_
{
	char sMsgLen       [  4];  /* 전문길이                          */
	char sTrCode       [  9];  /* transaction code                  */
	char sSystemType   [  3];  /* "STK"                             */
	char sServiceType  [  3];  /* "DDD":대행업무, "GGG":결제업무    */
	char sCompanyCode  [ 10];  /* 업체번호:증권사번호(7)+SPACE(3)   */
	char sSeqNo        [  7];  /* 전문일련번호                      */
	char sMsgType      [  4];  /* 전문구분코드                      */
	char sJobCode      [  3];  /* 업무구분코드                      */
	char sSrFlag       [  1];  /* 송수신구분 (은행:B 증권:E)        */
	char sReplyCode    [  3];  /* 응답코드                          */
	char sTransDateTime[ 14];  /* 송수신일시 (YYYYMMDDhhmmss)       */
	char sData         [443];  /* data                              */
} SCB_DATA;



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

typedef struct _RUN_TIME_
{
	char sBeg          [ 10];  /* 시작시간 (HHMM)                   */
	char sFin          [ 10];  /* 종료시간 (HHMM)                   */
} RUN_TIME;

/*
 * biz.properties 정보 구조체
 */
typedef struct _BIZ_PROP_
{
	char sSvcId        [ 20];  /* 서비스 ID                         */
	char sDesc         [128];  /* 설명 Description                  */
	char sObsFlag      [  5];  /* 관심 플래그                       */
	char sRunScpt      [128];  /* 실행 스크립트                     */
	char sRunSvr       [ 20];  /* 실행 서버 (AP/FEP)                */
	char sRunTime      [128];  /* 실행 시간 (0~HH:MM~HH:MM)         */

	RUN_TIME  rt       [  7];  /* 요일별RunTime 0(일요일) 6(토요일) */
} BIZ_PROP;

/*
 * proc 정보 구조체
 */
typedef struct _PROC_INFO_
{
	char sSvcId        [ 20];  /* 서비스 ID                         */
	char sProcess      [128];  /* 실행프로세스                      */
	char sPid          [ 20];  /* PID 문자열                        */
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

	int                   ssiNo       ;     /* client에서 사용. 연결 서버번호 */

	time_t                lastRecved  ;     /* poll sec 저장용                */
	int                   iPollFlag   ;     /* poll 상태                      */

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
	char                  sUseYn    [  1+1];   /* 사용여부 : Y / N          */
	char                  sEmpNo    [ 10+1];   /* 사원번호 : 999999999      */
	char                  sTmCode   [ 10+1];   /* 팀점코드 : 21085          */
	char                  sName     [ 20+1];   /* 이름     : 주원장         */
	char                  sPhone    [ 20+1];   /* 전화번호 : 0237727804     */
} UMS_CFG;

/*----------------------------------------------------------------------------*/
/*
 *  Ums File Structure (109)
 */

typedef struct
{
	char                  sWriteTm  [   14];   /* 기록일시(YYYYMMDDhhmmss)  */
	char                  sSendTm   [   14];   /* 발송일시(YYYYMMDDhhmmss)  */
	char                  sSendStat [    6];   /* 발송상태                  */
	char                  sMessage  [  160];   /* 메시지:한글40자(UTF-8)    */
	char                  sLF       [    1];   /* LF(0x0A)                  */
} UMS_FILE;

/*----------------------------------------------------------------------------*/
/*
 *  Ums Send Structure (266) : 업무상세코드 : FEP(뱅킹) -> SA00010
 */

typedef struct
{
	char            sWorkDay        [    8];   /* 발송일자(YYYYMMDD)        */
	char            sUmsSendType    [    3];   /* UMS발송구분코드 : SYS     */
	char            sChannelCode    [    3];   /* UMS업무구분코드 : 012     */
	char            sWorkCode       [    7];   /* UMS업무상세코드 : SA00010 */
	char            sToId           [    9];   /* 사원번호 : 999999999      */
	char            sToName         [   20];   /* 수신인명                  */
	char            sToPhone        [   11];   /* 수신인전화번호            */
	char            sFromId         [    9];   /* 발송사원번호 : 999999999  */
	char            sFromDept       [    5];   /* 발신자팀점코드 : 21085    */
	char            sFromName       [   20];   /* 발신자명 : 주원장         */
	char            sFromPhone      [   11];   /* 발신자전화번호:0237727804 */
	char            sContent        [  160];   /* SMS 내용 : 한글40/영문80  */
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
	char            sObjPath        [128+1];   /* 대상폴더                    */
	int             iExpNo                 ;   /* Expire Number               */
	char            sPattern        [128+1];   /* 파일검색 패턴               */
} LOG_DEL;


/*----------------------------------------------------------------------------*/
/*
 *  Log Zip Config Structure
 */

typedef struct
{
	char            sObjPath        [128+1];   /* 대상폴더                    */
	int             iExpNo                 ;   /* Expire Number               */
	char            sPattern        [128+1];   /* 파일검색 패턴               */
} LOG_ZIP;

/*----------------------------------------------------------------------------*/
/*
 *  Log Sec Config Structure
 */

typedef struct
{
	char            sObjPath        [128+1];   /* 대상폴더                    */
	int             iExpNo                 ;   /* Expire Number               */
	char            sPattern        [128+1];   /* 파일검색 패턴               */
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


