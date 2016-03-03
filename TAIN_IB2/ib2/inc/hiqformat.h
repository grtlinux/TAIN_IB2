/*
 *  Host Interface Header Format
 */
typedef struct _FEP_HEADER 
{
/*  
 *  Interface Header 
 */
    char sStx       [ 1];  /* STX 0X02 */
    char sLength    [ 4];  /* STX & Length Field�� ������ ������ Field Data�� Length */
    char sBsnClas   [ 4];  /* ������ ID(TCP PORT ��ȣ�� ������ */
    char sHostClas  [ 2];  /* HOST ������ */
    char sDataClas  [ 2];  /* LK: LINK, LO: LINK-OK, PL: POLLING, PO: POLLING-OK, DT: DATA */
    char sSeq       [ 8];  /* Data: Serial �ϰ� 1�� ����, Block���� ù��° Data-SEQ, POLLING/LINK: ������ ó���� Data�� SEQ */
    char sDataCnt   [ 2];  /* DATA BLOCK���� DATA�Ǽ� */
    char sDateTime  [20];  /* YYYYMMDDHHMMSSssssss */
    char sFiller    [ 4];
}   FEP_HEADER;

/*  
 *  Data Header      
 */
typedef struct _FEP_DATA
{
    FEP_HEADER       stH;
    char sDataLength[ 4];  /* �� Length FIELD�� ������ ���� DATA SIZE */
    char sDataSeq   [ 8];  /* �ش� Data�� �Ϸù�ȣ */
    char sData      [DATA_MAX_SIZE];
    
/*  char sDate      [ 8];  ** ���� ������(YYYYMMDD) */
/*  char sTime      [12];  ** ���� �ۼ��� �ð�(HHMMSSssssss) */
/*  char sDataClas2 [ 2];  **  */
/*  char sRespCode  [ 4];  ** ����� '0000', RESP-Code ���� */
/*  char sDataFiller[80];  **  */
}   FEP_DATA;

