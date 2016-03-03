/*
 *  Host Interface Header Format
 */
typedef struct _FEP_HEADER 
{
/*  
 *  Interface Header 
 */
    char sStx       [ 1];  /* STX 0X02 */
    char sLength    [ 4];  /* STX & Length Field를 제외한 나머지 Field Data의 Length */
    char sBsnClas   [ 4];  /* 업무별 ID(TCP PORT 번호와 병행사용 */
    char sHostClas  [ 2];  /* HOST 구분자 */
    char sDataClas  [ 2];  /* LK: LINK, LO: LINK-OK, PL: POLLING, PO: POLLING-OK, DT: DATA */
    char sSeq       [ 8];  /* Data: Serial 하게 1씩 증가, Block내의 첫번째 Data-SEQ, POLLING/LINK: 마지막 처리한 Data의 SEQ */
    char sDataCnt   [ 2];  /* DATA BLOCK내의 DATA건수 */
    char sDateTime  [20];  /* YYYYMMDDHHMMSSssssss */
    char sFiller    [ 4];
}   FEP_HEADER;

/*  
 *  Data Header      
 */
typedef struct _FEP_DATA
{
    FEP_HEADER       stH;
    char sDataLength[ 4];  /* 본 Length FIELD를 제외한 개별 DATA SIZE */
    char sDataSeq   [ 8];  /* 해당 Data의 일련번호 */
    char sData      [DATA_MAX_SIZE];
    
/*  char sDate      [ 8];  ** 전문 생성일(YYYYMMDD) */
/*  char sTime      [12];  ** 전문 송수신 시간(HHMMSSssssss) */
/*  char sDataClas2 [ 2];  **  */
/*  char sRespCode  [ 4];  ** 정상시 '0000', RESP-Code 참조 */
/*  char sDataFiller[80];  **  */
}   FEP_DATA;

