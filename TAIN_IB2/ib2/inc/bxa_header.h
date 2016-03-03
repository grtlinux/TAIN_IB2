/*
 *  PROGRAM ID      : bxa_header.h
 *  PROGRAM NAME    : 대외 배치 전문 header
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 *  일괄전송 TCP/IP 통신 전문 FORMAT
 *  1) 통상전문
 *     0320.DATA전문
 *     0300.결번자료내역 전송
 *     0310.결번DATA 전송
 *
 *  2) 일반관리전문
 *     0600.업무관리
 *     0610.업무관리응답
 *     0620.결번전문확인
 *     0630.화일정보관리
 *     0640.화일정보관리응답
 */

#ifndef _BXA_HEADER_H_
#define _BXA_HEADER_H_

#if defined KFT         /* 금융결제원,카드사매입정보 */
#  define LEN_FIELD1   1    /* Header 첫번째 length field 사용여부     */
#  define LEN_FIELD2   0    /* Header 두번째 length field 사용여부     */
#  define ORG_CODE_LEN 3    /* Header 기관코드의 길이                  */
#  define TRX_CLS_SEND "R"  /* Header 당사기준 파일송신의 거래구분코드 */
#  define TRX_CLS_RECV "S"  /* Header 당사기준 파일수신의 거래구분코드 */
#  define SR_FLAG_SEND "B"  /* Header 당사기준 전문송신의 송수신플래그 */
#  define MSG_DTL_LEN  4058 /* DATA 전문의 버퍼 크기                   */
#  define WE_OPEN_ALL  1    /* 파일수신시에도 당사가 개시하는지 여부   */
#elif defined KFT1      /* 금융결제원(이용기관용) */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "R"
#  define TRX_CLS_RECV "S"
#  define SR_FLAG_SEND "B"
#  define MSG_DTL_LEN  4058
#  define WE_OPEN_ALL  1
#elif defined SHC       /* 신한카드 */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   1
#  define ORG_CODE_LEN 2
#  define TRX_CLS_SEND "R"
#  define TRX_CLS_RECV "S"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2011
#  define WE_OPEN_ALL  1
#elif defined HDC       /* 현대카드 */
#  define LEN_FIELD1   0
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined LTC       /* 롯데카드 */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined SSC       /* 삼성카드 */
#  define LEN_FIELD1   1
#  define LEN_FIELD2   0
#  define LEN_FIELD3   0
#  define ORG_CODE_LEN 8
#  define TRX_CLS_SEND "S"
#  define TRX_CLS_RECV "R"
#  define SR_FLAG_SEND "C"
#  define MSG_DTL_LEN  2000
#  define WE_OPEN_ALL  0
#elif defined CTC       /* 씨티카드 */
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

#define  MSGTYPE_OPEN     10 /* 업무개시 */
#define  MSGTYPE_FILE     20 /* 파일정보 */
#define  MSGTYPE_DATA     30 /* DATA     */
#define  MSGTYPE_MISS     40 /* 결번     */
#define  MSGTYPE_MDAT     50 /* 결번DATA */
#define  MSGTYPE_CONT     60 /* 파일계속 */
#define  MSGTYPE_FEND     70 /* 파일종료 */
#define  MSGTYPE_DONE     80 /* 업무종료 */

/*
 *  전문 헤더
 */
typedef struct
{
  /*char sTrCode[   9];                 ** Transaction Code                         */
#if LEN_FIELD1
    char sMsgLen[   4];                 /* TCP/IP전문송신Byte수                     */
#endif
    char sBizKnd[   3];                 /* 업무구분코드 'KTB'                       */
#if LEN_FIELD2
    char sMsgLen[   4];                 /* TCP/IP전문송신Byte수                     */
#endif
#if LEN_FIELD3
    char sLength[   4];                 /* TCP/IP전문송신Byte수                     */
#endif
    char sOrgzCd[ORG_CODE_LEN];         /* 기관코드                                 */
    char sMsgCls[   4];                 /* 전문구분코드                             */
    char sTrxCls[   1];                 /* 거래구분코드 'S','R'                     */
    char sSrFlag[   1];                 /* 송수신 Flag  'C','B'                     */
    char sFileNm[   8];                 /* 파일구분코드  0600/001, 0600/004 Space   */
    char sRespCd[   3];                 /* 응답코드                                 */
}   MSG_HEADER;

/*
 *  0600/0610 전문
 */
typedef struct
{
    MSG_HEADER stH;
    char sSendDt[  10];                 /* 전문전송일시MMDDhhmmss                   */
    char sBizCls[   3];                 /* 업무관리정보                             */
    char sSender[  20];                 /* 송신자명                                 */
    char sPasswd[  16];                 /* 송신자암호                               */
}   MSG_0600;
typedef MSG_0600 MSG_MANG;

/*
 *  0620 전문
 */
typedef struct
{
    MSG_HEADER stH;
    char  sBlockN[   4];                 /* BLOCK-NO                                 */
    char  sSequeN[   3];                 /* SEQUENCE-NO                              */
}   MSG_0620;
typedef MSG_0620 MSG_MISS_REQ;

/*
 *  0630/0640 전문
 */
typedef struct
{
    MSG_HEADER stH;
    char sFileId[   8];                 /* 파일 NAME                                */
    char sFileSz[  12];                 /* 파일 SIZE                                */
    char sMsgByt[   4];                 /* 전문 BYTE수                              */
}   MSG_0630;
typedef MSG_0630 MSG_FILE;

/*
 *  0300 전문
 */
typedef struct
{
    MSG_HEADER stH;
    char sBlockN[   4];                 /* BLOCK-NO                                 */
    char sSequeN[   3];                 /* SEQUENCE-NO                              */
    char sMisCnt[   3];                 /* 결번 갯수                                */
    char sMisBit[ 100];                 /* 결번 Data                                */
}   MSG_0300;
typedef MSG_0300 MSG_MISS_RESP;

/*
 *  0310/0320 전문
 */
typedef struct
{
    MSG_HEADER stH;
    char sBlockN[   4];                 /* BLOCK-NO                                 */
    char sSequeN[   3];                 /* SEQUENCE-NO                              */
    char sSlByte[   4];                 /* 실DATA BYTE수                            */
    char sMsgDtl[MSG_DTL_LEN];          /* 파일 내역                                */
}   MSG_0320;
typedef MSG_0320 MSG_DATA;


#if 0
/*
 *  EA13MMDD Header
 */
typedef struct
{
    char   sRecdTp[ 1];  /* Record 구분("H")           */
    char   sSeqNo [ 7];  /* 일련번호("0000000")        */
    char   sOrgzCd[10];  /* 기관코드(이용기관식별코드) */
    char   sFileNm[ 8];  /* FILE 이름(EA13MMDD)        */
    char   sAcptDt[ 6];  /* 신청접수일자(YYMMDD") (FILE이름과 동일한 날짜) */
    char   sFiller[68];  /* FILLER                     */
}   EA13_H;

/*
 *  EA13MMDD Trailer
 */
typedef struct
{
    char   sRecdTp[ 1];  /* Record 구분("T")           */
    char   sSeqNo [ 7];  /* 일련번호(9999999")         */
    char   sOrgzCd[10];  /* 기관코드(이용기관식별코드) */
    char   sFileNm[ 8];  /* FILE 이름                  */
    char   sTotCnt[ 7];  /* 총 Data Record수           */
    char   sNewCnt[ 7];  /* 신규등록건수               */
    char   sUpdCnt[ 7];  /* 변경등록건수(ZERO)         */
    char   sCanCnt[ 7];  /* 해지등록건수               */
    char   sOptCnt[ 7];  /* 임의해지건수               */
    char   sFiller[29];  /* FILLER                     */
    char   sMacKey[10];  /* MAC 검증값                 */
}   EA13_T;
#endif


/*
 *  송신자 암호 입력
 */
typedef struct _PASSWORD_I_
{
    int   iProcTp;       /* 처리구분 1: 암호화 2: 복호화 */
    char  sUserId[20+1]; /* 송신자명 */
    char  sUserPw[20+1]; /* 송신자암호 */
    char  sBizYmd[20+1]; /* 거래일 */
    char  sOrgzCd[20+1]; /* 이용기관식별코드 */
    char  sEnCode[20+1]; /* 송신자암호 암호문 */
}   PASSWORD_I;

/*
 *  송신자 암호 출력
 */
typedef struct _PASSWORD_O_
{
    char  sUserPw[20+1]; /* 송신자암호 */
    char  sEnCode[20+1]; /* 송신자암호 암호문 */
}   PASSWORD_O;

#endif
