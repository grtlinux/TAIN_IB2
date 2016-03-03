/*
 *  PROGRAM ID      : oaacom0000.h
 *  PROGRAM NAME    : FEP 송수신 공통 header
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
#define TC_BABG "BABG" /* 배치프로세스만 이용 */
#define TC_BAED "BAED" /* 배치프로세스만 이용 */
#define TC_RSEQ "RSEQ" /* 백업프로세스만 이용 */

#define SEARCH_TYPE_ASC  0
#define SEARCH_TYPE_DESC 1
#define SEARCH_TYPE_NEXT 2
#define SEARCH_TYPE_MAX  3

#define IS_BATCH(fepid) ((fepid)[3] > '4')

/*
 *  FEP전문 Fep Header
 */
typedef struct _FEP_HEADER 
{
    char sLength    [ 4];  /* 전문길이    : 전문길이 Field를 제외한 나머지 Length */
    char sBsnCode   [ 6];  /* 업무식별코드: KBBS01 */
    char sTrCode    [ 4];  /* TR-CODE     : LINK, LIOK, DATA, DAOK, POLL, POOK, RJCT 등 */
    char sResCode   [ 2];  /* 응답코드    : ‘00’5. 장애응답코드표 참조 */
    char sSeq       [ 8];  /* 전문일련번호: ‘99999999’? 전문SEQ번호 */
    char sDataCnt   [ 2];  /* Data건수    : 01 ~ (2 KByte 이내) */
    char sDateTime  [20];  /* 시각        : YYYYMMDDHHMMSSssssss */
    char sAckYn     [ 1];  /* ACK구분     : Data에 대한 ACK : 사용(Y), 미사용(N) LINK시에 결정 됨 */
    char sFiller    [ 3];
}   FEP_HEADER;

/*
 *  FEP전문 Biz Header
 */
typedef struct _BIZ_HEADER
{
    char sDataLength[ 4];  /* 본 Length FIELD를 제외한 개별 DATA SIZE */
    char sDataSeq   [ 8];  /* 해당 Data의 일련번호 */
    char sGlobalId  [32];
    char sDateTime  [20];
    char sRespCode  [ 4];
    char sMediaTp   [ 2];
    char sMediaSeq  [ 1];
    char sFiller    [29];
}   BIZ_HEADER;

/*
 *  FEP전문 Biz 전문
 */
typedef struct _BIZ_DATA
{
    BIZ_HEADER       stH;
    char sData      [DATA_MAX_SIZE];
}   BIZ_DATA;

/*
 *  FEP전문
 */
typedef struct _FEP_DATA
{
    FEP_HEADER       stH;
    BIZ_DATA         stB;
}   FEP_DATA;

/*
 *  FEP전문 Batch Info
 */
typedef struct _BATCH_INFO
{
    char sRecLen    [ 4]; /* 레코드 길이 */
    char sRecCnt    [ 7]; /* 레코드 건수 */
    char sSR        [ 1]; /* 송수신 구분 */
    char sOrgCode   [ 3]; /* 기관코드 */
    char sTrCode    [11]; /* 일괄파일 TR Code */
    char sTrCodeLen [ 2]; /* 일괄파일 TR Code 길이 */
    char sDate      [ 8]; /* 전문전송일자 */
    char sTime      [ 9]; /* 전문전송시각 */
    char sTrCodeSeq [ 2]; /* 일자별 TR Code 회차 */
    char sFiller    [53]; /* 예비 */
}   BATCH_INFO;

/*
 *  FEP 배치전문
 */
typedef struct _BATCH_FEP_DATA
{
    FEP_HEADER       stH;
    char sData      [DATA_MAX_SIZE];
}   BATCH_FEP_DATA;

/*
 *  TR LIST 포맷(bfa_trlist.cfg)
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

