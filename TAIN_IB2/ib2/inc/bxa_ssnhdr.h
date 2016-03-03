/*
 *  PROGRAM ID      : bxa_header.h
 *  PROGRAM NAME    : 대외 배치 전문 header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 *  삼성네트워크 일괄전송 TCP/IP 통신 전문 FORMAT
 */

#ifndef _BXA_SSNHDR_H_
#define _BXA_SSNHDR_H_

#define TRX_CLS_SEND "SD" /* Header 당사기준 파일송신의 거래구분코드 */
#define TRX_CLS_RECV "RD" /* Header 당사기준 파일수신의 거래구분코드 */
#define MSG_DTL_LEN  2048 /* DATA 전문의 버퍼 크기                   */

#define MSGTYPE_LOGIN      10
#define MSGTYPE_FILEINFO   20
#define MSGTYPE_FILEHEADER 30
#define MSGTYPE_RECVNOTIFY 40
#define MSGTYPE_DATA       50
#define MSGTYPE_LOGOUT     90

/*
 *  전문 헤더
 */
typedef struct
{
    char sTrCode[   9]; /* Transaction Code                         */
    char sMsgCls[   3]; /* 전문구분코드                             */
    char sRespCd[   3]; /* 응답코드                                 */
    char sBizFld[  20]; /* 업무필드                                 */
    char sFiller[   5]; /* 예비                                     */
}   MSG_HEADER;

/*
 *  LOGIN(003/030)
 */
typedef struct
{
    MSG_HEADER stH;
    char sSender[  20]; /* Van ID                                   */
    char sPasswd[   8]; /* Van Password                             */
    char sSrFlag[   2]; /* Job Type(SD:자료송신, RD:자료수신, RS:송신목록수신, RR:수신목록수신) */
    char sRcvSeq[  10]; /* Sequence(수신시 수신목록 값)             */
    char sSubjct[  10]; /* subject space                            */
    char sRdFlag[   1]; /* Flag(E:미수신, R:기수신, A:미/기수신)    */
    char sStDtTm[  12]; /* 시작시간(YYYYMMDDhhmm)                   */
    char sEdDtTm[  12]; /* 끝시간  (YYYYMMDDhhmm)                   */
    char sChgPwd[   1]; /* Password변경여부(Y:변경)                 */
    char sNewPwd[   8]; /* Password변경시 신규 Password             */
    char sCommSz[   4]; /* commsize(기본:2048)                      */
    char sCnvFlg[   1]; /* 코드 변환여부(N:변환없음)                */
    char sSendId[  10]; /* 거래처 ID                                */
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
    char sSrFlag[   2]; /* Job Type(SD:자료송신, RD:자료수신, RS:송신목록수신, RR:수신목록수신) */
    char sRcvSeq[  10]; /* Sequence(수신시 수신목록 값)             */
    char sSubjct[  10]; /* subject space                            */
    char sRdFlag[   1]; /* Flag(E:미수신, R:기수신, A:미/기수신)    */
    char sStDtTm[  12]; /* 시작시간(YYYYMMDDhhmm)                   */
    char sEdDtTm[  12]; /* 끝시간  (YYYYMMDDhhmm)                   */
    char sFiller[  35]; /* FILLER                                   */
}   MSG_LOGOUT;

/*
 *  파일정보(100/110)
 */
typedef struct
{
    MSG_HEADER stH;
    char sRcvSeq[  10]; /* Sequence(수신시 수신목록 값)             */
    char sSubjct[  10]; /* 8자리 이내 사용                          */
    char sFileSz[  10]; /* 파일 SIZE                                */
    char sSendId[  20]; /* 송신자 ID                                */
    char sRecvId[  20]; /* 수신자 ID                                */
    char sFill01[   1]; /* FILLER                                   */
    char sFill02[  10]; /* FILLER                                   */
    char sNxtEnd[   3]; /* 마지막여부(NXT:파일계속, END:마지막파일) */
    char sFileTp[   3]; /* 전송종류(NEW:일반)                       */
    char sFill03[   1]; /* FILLER                                   */
    char sFill04[  22]; /* FILLER                                   */
}   MSG_FILEINFO;

/*
 *  수신확인(130)
 */
typedef struct
{
    MSG_HEADER stH;
    char sRcvSeq[  10]; /* Sequence(수신시 수신목록 값)             */
    char sSubjct[  10]; /* 8자리 이내 사용                          */
    char sFileSz[  10]; /* 파일 SIZE                                */
    char sRecvYn[   1]; /* 정상수신여부(Y:정상, N:이상있음)         */
    char sFiller[  79]; /* FILLER                                   */
}   MSG_RECVNOTIFY;

/*
 *  파일헤더()
 */
typedef struct
{
    char sHeadTp[  10]; /* 헤더구분 <<NEHEAD>>                      */
    char sJob_Tp[   1]; /* 업무구분                                 */
    char sDataTp[   1]; /* 자료형태(N:텍스트, B:Binary, C:압축)     */
    char sFileSz[  10]; /* 파일 SIZE                                */
    char sCompSz[  10]; /* 압축 SIZE                                */
    char sSendId[  10]; /* 송신자 ID                                */
    char sSendTp[   2]; /* 업체구분                                 */
    char sBodyTp[   2]; /* BODY의 정보구분                          */
    char sFiller[   1]; /* FILLER                                   */
    char sRecvNu[   3]; /* 수신자 수                                */
    char sRecvId[  10]; /* 수신자 ID                                */
    char sRcdDel[   2]; /* 레코드 구분자                            */
}   MSG_FILEHEADER;

#endif
