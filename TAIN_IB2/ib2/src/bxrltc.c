/*
 * PROGRAM ID   : bxr.c
 * PROGRAM NAME : bxr 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : bxr 모듈
 * HISTORY      : 2013.08.04 강석. 최초작성
 *
 * 신한카드 배치파일 수신
 *
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: 최초작업"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.11.001: 최초작업."

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   RECONN_SEC               5
#define   MAX_DATA_LINE         8192

#define   BLOCK_RECS             100

/*#define   LOOP_TIME_SEC    (10 * 60) */
#define   LOOP_TIME_SEC          (2)

#define   MSGTYPE_OPEN            10
#define   MSGTYPE_FILE            20
#define   MSGTYPE_DATA            30
#define   MSGTYPE_MISS            40
#define   MSGTYPE_MDAT            50
#define   MSGTYPE_CONT            60
#define   MSGTYPE_FEND            70
#define   MSGTYPE_DONE            80
#define   MSGTYPE_NONE            90        /* not defined. */

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * EDI 배치 관련 헤더 : 신한카드
 *
 *    typedef struct _MSG_HDR_SHC_  // (27)
 *    {
 *        char    sMsgLen   [   4];  // TCP/IP 전문송신 Byte수
 *        char    sBizKnd   [   3];  // 업무구분코드 'EFT'
 *        char    sLength   [   4];  // 전문SIZE Byte수 = sMsgLen
 *        char    sOrgzCd   [   2];  // 회원사코드. 02(은행코드)
 *        char    sMsgCls   [   4];  // 전문구분코드
 *        char    sTrxCls   [   1];  // 거래구분코드 'S'기관송신, 'R'카드사송신
 *        char    sSrFlag   [   1];  // 전문발생기관.송수신Flag 'C'기관 'B'카드사
 *        char    sFileNm   [   8];  // 파일구분코드  0600/001, 0600/004, SPACE
 *        char    sRespCd   [   3];  // 응답코드
 *    } MSG_SHC_HDR;
 *
 */
/*----------------------------------------------------------------------------*/

typedef MSG_LTC_HDR  MSG_HDR ;
typedef MSG_LTC_LINE MSG_LINE;
typedef MSG_LTC_0600 MSG_MANG;
typedef MSG_LTC_0630 MSG_FILE;
typedef MSG_LTC_0310 MSG_DATA;
typedef MSG_LTC_0620 MSG_MREQ;
typedef MSG_LTC_0300 MSG_MRES;

MSG_HDR*        pHdr            ;
MSG_LINE*       pLine           ;

MSG_MANG*       pMang           ;                  /* 0600, 0610 전문  : MSG_MANG */
MSG_FILE*       pFile           ;                  /* 0630, 0640 전문  : MSG_FILE */
MSG_DATA*       pData           ;                  /* 0310, 0320 전문  : MSG_DATA */
MSG_MREQ*       pMreq           ;                  /* 0620 전문        : MSG_MREQ */
MSG_MRES*       pMres           ;                  /* 0300 전문        : MSG_MRES */

/*----------------------------------------------------------------------------*/

int             iForkFlag       ;

char            sBatInf         [MAX_DATA_LINE];   /* FEP 배치정보 from FQ */

char            sFileInf        [MAX_DATA_LINE];   /* 배치파일정보         */

char            sSendEdiLine    [MAX_DATA_LINE];   /* 송신전문 */
int             iSendEdiLen     ;

char            sRecvEdiLine    [MAX_DATA_LINE];   /* 수신전문 */
int             iRecvEdiLen     ;

char            sBatHdr         [MAX_DATA_LINE];   /* 배치전문의 헤더부 */

char            sDataLine       [MAX_DATA_LINE];   /* 배치전문의 데이터부 */
int             iDataLen        ;

/*----------------------------------------------------------------------------*/

char            sUserid   [20+1];                  /* Userid          */
char            sPasswd   [16+1];                  /* Passwd          */

char            sFileId   [20+1];                  /* 송수신 파일명   */

FILE*           fp              ;                  /* 배치파일 fp     */
long            lPos            ;                  /* 배치파일 포지션 */

int             iRecLen         ;                  /* 레코드한라인의 길이  */
int             iRecCnt         ;                  /* 레코드의 갯수        */

int             iTrCodeSeq      ;                  /* TrCode Sequence : 01 ~ 99 */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : BAT_INF 구조체를 출력한다.
 */
/*----------------------------------------------------------------------------*/
void PrintBatHdrEDI()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatHdr == 0x00)
			return;

		MSG_HDR* p = (MSG_HDR*) &sBatHdr;

		printf("---- MSG_HDR* p --------------------------------\n");
		printf("[%s]\n", sBatHdr);
		printf("\tp->sMsgLen = [%*.*s]\n", sizeof(p->sMsgLen), sizeof(p->sMsgLen), p->sMsgLen);
		printf("\tp->sBizKnd = [%*.*s]\n", sizeof(p->sBizKnd), sizeof(p->sBizKnd), p->sBizKnd);
		printf("\tp->sOrgzCd = [%*.*s]\n", sizeof(p->sOrgzCd), sizeof(p->sOrgzCd), p->sOrgzCd);
		printf("\tp->sMsgCls = [%*.*s]\n", sizeof(p->sMsgCls), sizeof(p->sMsgCls), p->sMsgCls);
		printf("\tp->sTrxCls = [%*.*s]\n", sizeof(p->sTrxCls), sizeof(p->sTrxCls), p->sTrxCls);
		printf("\tp->sSrFlag = [%*.*s]\n", sizeof(p->sSrFlag), sizeof(p->sSrFlag), p->sSrFlag);
		printf("\tp->sFileNm = [%*.*s]\n", sizeof(p->sFileNm), sizeof(p->sFileNm), p->sFileNm);
		printf("\tp->sRespCd = [%*.*s]\n", sizeof(p->sRespCd), sizeof(p->sRespCd), p->sRespCd);
		printf("---------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * 내용 : sDataLine 자료를 송신한다.
 */
/*----------------------------------------------------------------------------*/
BOOL SendDataEDI(int cSock, char* sMsgCls)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		pLine = (MSG_LINE*) &sSendEdiLine;

		if (flag)
		{

			/*
			 * 송신길이를 구한다.
			 */
			iSendEdiLen = sizeof(MSG_HDR) + strlen(sDataLine);

			/*
			 * 송신전문을 세팅한다.
			 */

			sprintf(sSendEdiLine, "%04d%3.3s%8.8s%4.4s%1.1s%1.1s%-8.8s%3.3s%s"
				, iSendEdiLen - 4
				, "FTE"
				, "LOTTE   "
				, sMsgCls
				, "S"
				, "C"
				, sFileId
				, "000"
				, sDataLine);

			if (!flag) LOG(_FL_, 0, 0, "[SendDataEDI]...[%s] ", sSendEdiLine);
		}

		if (flag)
		{
			/*
			 * 송신한다. :       MSG_HDR + sDataLine ->
			 */
			int ret = SockSend(cSock, sSendEdiLine, iSendEdiLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "SEND(%d): EDI_HDR+DATA [%*.*s][%s]"
					, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), (char*)&pLine->stHdr
					, sDataLine);
			}
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : 수신한 자료를 sDataLine 복사한다.
 */
/*----------------------------------------------------------------------------*/
BOOL RecvDataEDI(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, " [ START ]...");
	
	if (flag)
	{
		pLine = (MSG_LINE*) &sRecvEdiLine;

		if (flag)
		{
			/*
			 * 수신한다. :   <- MSG_HDR + sDataLine
			 */

			if (flag)
			{
				/*
				 * 전문에서 길이부분을 읽는다.
				 */
				int ret = SockRecv(cSock, pLine->stHdr.sMsgLen, 4);
				if (ret <= 0)
				{
					CSockFDCLR(cSock);
					return FALSE;
				}
			}

			iRecvEdiLen = 4 + UtilToIntN( pLine->stHdr.sMsgLen, 4 );

			if (flag)
			{
				/*
				 * 전문에서 길이부분을 제외한 자료부분을 읽는다.
				 */
				int ret = SockRecv(cSock, pLine->stHdr.sBizKnd, iRecvEdiLen - 4);
				if (ret <= 0)
				{
					CSockFDCLR(cSock);
					return FALSE;
				}
			}

			sRecvEdiLine[iRecvEdiLen] = 0x00;

			/*
			 * 수신받은 자료를 sDataLine에 복사한다.
			 */
			strcpy(sDataLine, pLine->sData);

			/*
			 * 수신데이터를 출력한다.
			 */
			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "RECV(%d): EDI_HDR+DATA [%*.*s][%s]"
					, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), (char*)&pLine->stHdr
					, sDataLine);
			}
		}
	}

	if (!flag) LOG(_FL_, 0, 0, " [ END ]...");

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : 업무개시 (당사/타사)   0600/001 -> 0610/001
 */
/*----------------------------------------------------------------------------*/
int GetMsgType()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sMsgType[20];
		pLine = (MSG_LINE*) &sRecvEdiLine;

		if (strncmp(pLine->stHdr.sMsgCls, "0600", 4) == 0)
		{
			pMang = (MSG_MANG*) &pLine->sData;
			sprintf(sMsgType, "%4.4s%3.3s", pLine->stHdr.sMsgCls, pMang->sBizCls);
		}
		else
		{
			sprintf(sMsgType, "%4.4s%3.3s", pLine->stHdr.sMsgCls, "000");
		}

		switch (UtilToInt(sMsgType))
		{
			case 600001: return MSGTYPE_OPEN;
			case 600002: return MSGTYPE_CONT;
			case 600003: return MSGTYPE_FEND;
			case 600004: return MSGTYPE_DONE;
			case 620000: return MSGTYPE_MISS;
			case 630000: return MSGTYPE_FILE;
			case 310000: return MSGTYPE_MDAT;
			case 320000: return MSGTYPE_DATA;
			default:
				if (flag) LOG(_FL_, 0, 0, "[GetMsgType]...not defined[%s] ", sRecvEdiLine);
				return MSGTYPE_NONE;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 업무개시 (당사/타사)   0600/001 -> 0610/001
 */
/*----------------------------------------------------------------------------*/
char* GetStrMsgType( int iMsgType )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char* ret = NULL;

	if (flag)
	{
		;
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : 응답코드
 */
/*----------------------------------------------------------------------------*/
char* GetStrRespCode( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char* ret = NULL;

	if (flag)
	{
		;
	}

	return ret;
}

/*============================================================================*/
/*============================================================================*/
/*
 * 내용 : 업무개시 (당사/타사)   0600/001 -> 0610/001
 *     TODO DATE.2013.08.06 : 당사/타사 처리 필요....
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // 전문전송일시 MMDDhhmmss
 *        char    sBizCls   [   3];  // 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)
 *        char    sSender   [  20];  // 송신자명   SPACE
 *        char    sPasswd   [  16];  // 송신자암호 SPACE
 *    } MSG_LTC_0600;
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatFileOPEN(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * OPEN : 업무개시 전문을 세팅한다.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "001"
			, ""
			, "");

		*sFileId = 0x00;

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0600") == FALSE)
		{
			return -1;
		}

		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvDataEDI(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 업무개시 (당사/타사)   0600/001 -> 0610/001
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // 전문전송일시 MMDDhhmmss
 *        char    sBizCls   [   3];  // 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)
 *        char    sSender   [  20];  // 송신자명   SPACE
 *        char    sPasswd   [  16];  // 송신자암호 SPACE
 *    } MSG_LTC_0600;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileOPEN(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		*sFileId = 0x00;

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0610") == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 파일정보 0630 -> 0640
 *
 *    typedef struct _MSG_0630_LTC_  // header + (24)
 *    {
 *      char    sFileId   [   4];  // 파일 NAME
 *      char    sCdDate   [   4];  // 자료일자 MMDD
 *      char    sFileSz   [  12];  // 파일 SIZE    0630은 '000000000000', 0640응답시 받은 내용
 *      char    sMsgByte  [   4];  // 전문 BYTE 수
 *    } MSG_LTC_0630;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileFILE(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * 응답자료를 만든다.
			 */
			strcpy(sFileInf, sDataLine);
			pFile = (MSG_FILE*) &sFileInf;

			sprintf(sDataLine, "%4.4s%4.4s%012d%4.4s"
				, pFile->sFileId
				, pFile->sCdDate
				, 0
				, pFile->sMsgByte);

			/*
			 * 헤더부분에 파일명을 붙이기 위한 작업.
			 */
			sprintf(sFileId, "%4.4s%4.4s", pFile->sFileId, pFile->sCdDate);
		}

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0640") == FALSE)
		{
			return -1;
		}

		/*
		 *  TODO DATE.2013.08.06 : BAT_INF를 구성하여 파일을 쌓는다.
		 *
		 *    typedef struct _BAT_INF_
		 *    {
		 *        char    sRecLen     [ 4]; // 레코드 길이
		 *        char    sRecCnt     [ 7]; // 레코드 건수
		 *        char    sSR         [ 1]; // 송수신 구분
		 *        char    sOrgCode    [ 3]; // 기관코드
		 *        char    sTrCode     [11]; // 일괄파일 TR Code
		 *        char    sTrCodeLen  [ 2]; // 일괄파일 TR Code 길이
		 *        char    sDate       [ 8]; // 전문전송일자 YYYYMMDD
		 *        char    sTime       [ 9]; // 전문전송시각 hhmmssSSS
		 *        char    sTrCodeSeq  [ 2]; // 일자별 TR Code 회차
		 *        char    sFiller     [53]; // 예비
		 *    } BAT_INF;
		 */
		if (flag)
		{
			/*
			 * 수신을 위한 배치 정보
			 */

			char* sSR         = "R"           ;    /* S 송신, R 수신              */
			char* sOrgCode    = "LTC"         ;    /* 기관코드                    */
			char  sTrCode[20]                 ;    /* TrCode                      */
			char  sDateTime[50]               ;    /* 날짜 : YYYYMMDDhhmmssSSSSSS */
			char  sFileName[124]              ;    /* 배치파일명                  */

			char buf[20];
			sprintf(buf, "%4.4s", pFile->sFileId);
			UtilFillCopy(sTrCode, buf, 11, '0');

			strcpy(sDateTime, TimeGetDateTime());

			/*
			 * 배치정보에서 sSR, sTrCode로 bxa_trlst.cfg 파일 정보를 읽는다.
			 */
			CfgBxaTr(sSR, sTrCode);

			/*
			 * 레코드의 길이를 구한다.
			 */
			iRecLen = UtilToInt(CfgGetBxaTr("RECLEN"));
			iRecCnt = 0;

			if (flag)
			{
				/*
				 * 송신용 배치파일을 만든다.
				 */

				for (iTrCodeSeq = 1; iTrCodeSeq < 100; iTrCodeSeq++)
				{
					BAT_INF* pInf = (BAT_INF*) &sBatInf;

					sprintf(sBatInf, "%04d%07d%1.1s%3.3s%-11.11s%02d%8.8s%9.9s%02d%53.53s"
						, iRecLen
						, iRecCnt
						, sSR
						, sOrgCode
						, sTrCode
						, 11
						, sDateTime
						, sDateTime + 8
						, iTrCodeSeq
						, "");

					sprintf(sFileName, CfgGetMaster("ib.path.file.bxrcom")
						, sizeof(pInf->sSR       ), sizeof(pInf->sSR       ), pInf->sSR
						, sizeof(pInf->sOrgCode  ), sizeof(pInf->sOrgCode  ), pInf->sOrgCode
						, sizeof(pInf->sTrCode   ), sizeof(pInf->sTrCode   ), pInf->sTrCode
						, sizeof(pInf->sDate     ), sizeof(pInf->sDate     ), pInf->sDate
						, sizeof(pInf->sTrCodeSeq), sizeof(pInf->sTrCodeSeq), pInf->sTrCodeSeq
						);

					UtilSetYYYYMMDD(sFileName);

					if (access(sFileName, R_OK) < 0)
					{
						/* file not exist.... */
						break;
					}
				}
			}

			if (flag)
			{
				/*
				 * 배치파일을 오픈한다.
				 */
				fp = fopen(sFileName, "w+");
				/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
				chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : DATA송신 (BLOCK)   0320
 *
 *    typedef struct _MSG_0310_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sSlByte   [   4];  // 실DATA BYTE수
 *        char    sMsgDtl   [4058];  // 파일 내용
 *    } MSG_LTC_0310;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileDATA(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신자료가 DATA가 아니면 리턴한다.
		 */

		if (flag)
		{
			/*
			 * sDataLine 을 구하고
			 * 수신배치파일에 쓴다.
			 * 수신 Msg를 record 길이로 쪼개서 파일에 기록한다.
			 */
			int iMsgLen;
			int iCnt;

			pData = (MSG_DATA*) &sDataLine;    /* 0310, 0320 전문  : MSG_DATA */

			iMsgLen = UtilToIntN(pData->sSlByte, sizeof(pData->sSlByte));
			iCnt = iMsgLen / iRecLen;

			int i;
			char* p;
			for (i=0, p=pData->sMsgDtl; i < iCnt; i++, p += iRecLen)
			{
				fprintf(fp, "%*.*s\n", iRecLen, iRecLen, p);
				iRecCnt ++;

				if (!flag) printf("%05d) DATA [%*.*s]\n",iRecCnt, iRecLen, iRecLen, p);
			}

			fflush(fp);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 결번확인   0620 -> 0300
 *
 *    typedef struct _MSG_0620_LTC_  // header + (7)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *    } MSG_LTC_0620;
 *
 *    typedef struct _MSG_0300_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sMisCnt   [   3];  // 결번 갯수
 *        char    sMisBit   [ 100];  // 결번 DATA
 *    } MSG_LTC_0300;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileMISS(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		static int iFlag = 0;

		pMreq = (MSG_MREQ*) &sDataLine;

		int iBlockNo = UtilToIntN(pMreq->sBlockNo, sizeof(pMreq->sBlockNo));
		int iSeqNo = UtilToIntN(pMreq->sSeqNo  , sizeof(pMreq->sSeqNo  ));

		/*
		 * 결번자료를 확인하여 결번처리를 한다.
		 */

		/*switch (iFlag) */
		switch (-1)
		{
			case 0:
				{
					sprintf(sDataLine, "%04d%03d%03d%s"
						, iBlockNo
						, iSeqNo
						, 20
						, "1111111111" "1111111111" "0000000000" "0000000000");
					iFlag++;
				}
				break;
			case 11:
				{
					sprintf(sDataLine, "%04d%03d%03d%s"
						, iBlockNo
						, iSeqNo
						, 10
						, "1111111111" "0000000000");
					iFlag++;
				}
				break;
			case 12:
				{
					sprintf(sDataLine, "%04d%03d%03d%s"
						, iBlockNo
						, iSeqNo
						, 20
						, "0000000000" "0000000000");
					iFlag++;
				}
				break;
			default:
				if (flag)
				{
					sprintf(sDataLine, "%04d%03d%03d%s"
						, iBlockNo
						, iSeqNo
						, 0
						, "1"
						);
					iFlag = 0;
				}
				else
				{
					sprintf(sDataLine, "%04d%03d%03d%s"
						, iBlockNo
						, iSeqNo
						, 0
						, "");
					iFlag = 0;
				}
				break;
		}

		if (!flag) LOG(_FL_, 0, 0, "[RecvBatFileMISS]...[%s]", sDataLine);

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */

		if (SendDataEDI(cSock, "0300") == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 결번 DATA송신    0310
 *
 *    typedef struct _MSG_0310_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sSlByte   [   4];  // 실DATA BYTE수
 *        char    sMsgDtl   [4058];  // 파일 내용
 *    } MSG_LTC_0310;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileMDAT(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신자료가 DATA가 아니면 리턴한다.
		 */

	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 이어보내기    0600/002 -> 0610/002
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // 전문전송일시 MMDDhhmmss
 *        char    sBizCls   [   3];  // 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)
 *        char    sSender   [  20];  // 송신자명   SPACE
 *        char    sPasswd   [  16];  // 송신자암호 SPACE
 *    } MSG_LTC_0600;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileCONT(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 여러개의 배치파일 처리시에 CONT에서
		 * 배치파일을 클로우즈한다.
		 */
		if (flag)
		{
			fclose(fp);
		}

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0610") == FALSE)
		{
			return -1;
		}

		/*
		 * 배치파일 정보를 세팅한다.
		 */
		if (flag)
		{
			char* sSR         = "R"           ;    /* S 송신, R 수신              */
			char* sOrgCode    = "LTC";
			char* sTrCode     = sFileId;
			char  sDateTime[50];

			char buf[20];
			sprintf(buf, "%4.4s", pFile->sFileId);
			UtilFillCopy(sTrCode, buf, 11, '0');

			strcpy(sDateTime, TimeGetDateTime());

			sprintf(sBatInf, "%04d%07d%1.1s%3.3s%-11.11s%02d%8.8s%9.9s%02d%53.53s"
				, iRecLen
				, iRecCnt
				, sSR
				, sOrgCode
				, sTrCode
				, 11
				, sDateTime
				, sDateTime + 8
				, iTrCodeSeq
				, "");

			if (flag) LOG(_FL_, 0, 0, "[RecvBatFileCONT] : sBatInf [%s]", sBatInf);

			WriteBatFQ();
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 파일종료      0600/003 -> 0610/003
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // 전문전송일시 MMDDhhmmss
 *        char    sBizCls   [   3];  // 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)
 *        char    sSender   [  20];  // 송신자명   SPACE
 *        char    sPasswd   [  16];  // 송신자암호 SPACE
 *    } MSG_LTC_0600;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileFEND(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 최종 배치파일 처리 후에
		 * 배치파일을 클로우즈한다.
		 */
		if (flag)
		{
			fclose(fp);
		}

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0610") == FALSE)
		{
			return -1;
		}

		/*
		 * 배치파일 정보를 세팅한다.
		 */
		if (flag)
		{
			char* sSR         = "R"           ;    /* S 송신, R 수신              */
			char* sOrgCode    = "LTC";
			char* sTrCode     = sFileId;
			char  sDateTime[50];

			char buf[20];
			sprintf(buf, "%4.4s", pFile->sFileId);
			UtilFillCopy(sTrCode, buf, 11, '0');

			strcpy(sDateTime, TimeGetDateTime());

			sprintf(sBatInf, "%04d%07d%1.1s%3.3s%-11.11s%02d%8.8s%9.9s%02d%53.53s"
				, iRecLen
				, iRecCnt
				, sSR
				, sOrgCode
				, sTrCode
				, 11
				, sDateTime
				, sDateTime + 8
				, iTrCodeSeq
				, "");

			if (flag) LOG(_FL_, 0, 0, "[RecvBatFileFEND] : sBatInf [%s]", sBatInf);

			WriteBatFQ();
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 업무종료      0600/004 -> 0610/004
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // 전문전송일시 MMDDhhmmss
 *        char    sBizCls   [   3];  // 업무관리정보 001(업무개시),002(이어보내기),003(파일종료),004(업무종료)
 *        char    sSender   [  20];  // 송신자명   SPACE
 *        char    sPasswd   [  16];  // 송신자암호 SPACE
 *    } MSG_LTC_0600;
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileDONE(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		*sFileId = 0x00;

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0610") == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileEDI(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int flagDone = 0;
		int ret;

		if (!flag)
		{
			/*
			 * 업무개시 (당사발생)     0600/001 ->  <- 0610/001
			 */
			if (SendBatFileOPEN(cSock) < 0)
			{
				return -1;
			}
		}

		while (!flagDone)
		{
			/*
			 * 수신한다. 수신데이터는 sDataLine
			 */
			*sDataLine = 0x00;
			if (RecvDataEDI(cSock) == FALSE)
			{
				return -1;
			}

			/*
			 * 수신데이터에 따른 처리를 한다.
			 */
			switch (GetMsgType())
			{
				case MSGTYPE_OPEN:
					ret = RecvBatFileOPEN(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_FILE:
					ret = RecvBatFileFILE(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_DATA:
					ret = RecvBatFileDATA(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_MISS:
					ret = RecvBatFileMISS(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_MDAT:
					ret = RecvBatFileMDAT(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_CONT:
					ret = RecvBatFileCONT(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_FEND:
					ret = RecvBatFileFEND(cSock);
					if (ret < 0)
					{
						return ret;
					}
					break;

				case MSGTYPE_DONE:
					ret = RecvBatFileDONE(cSock);
					if (ret < 0)
					{
						return ret;
					}
					flagDone = 1;
					break;
			}
		}
	}

	if (flag)
	{
		/* 세션을 종료한다. */
		CSockFDCLR(cSock);

		if (flag) LOG(_FL_, 0, 0, "STATUS : close session");
	}

	return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : FQ의 내용을 읽어 데이터부의 BAT_INF 구조체를 출력한다.
 */
/*----------------------------------------------------------------------------*/
void PrintBatInf()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatInf == 0x00)
			return;

		BAT_INF* pInf = (BAT_INF*) &sBatInf;

		printf("---- BAT_INF* pInf --------------------------------\n");
		printf("[%s]\n", sBatInf);
		printf("\tpInf->sRecLen    = [%*.*s]\n", sizeof(pInf->sRecLen   ), sizeof(pInf->sRecLen   ), pInf->sRecLen   );
		printf("\tpInf->sRecCnt    = [%*.*s]\n", sizeof(pInf->sRecCnt   ), sizeof(pInf->sRecCnt   ), pInf->sRecCnt   );
		printf("\tpInf->sSR        = [%*.*s]\n", sizeof(pInf->sSR       ), sizeof(pInf->sSR       ), pInf->sSR       );
		printf("\tpInf->sOrgCode   = [%*.*s]\n", sizeof(pInf->sOrgCode  ), sizeof(pInf->sOrgCode  ), pInf->sOrgCode  );
		printf("\tpInf->sTrCode    = [%*.*s]\n", sizeof(pInf->sTrCode   ), sizeof(pInf->sTrCode   ), pInf->sTrCode   );
		printf("\tpInf->sTrCodeLen = [%*.*s]\n", sizeof(pInf->sTrCodeLen), sizeof(pInf->sTrCodeLen), pInf->sTrCodeLen);
		printf("\tpInf->sDate      = [%*.*s]\n", sizeof(pInf->sDate     ), sizeof(pInf->sDate     ), pInf->sDate     );
		printf("\tpInf->sTime      = [%*.*s]\n", sizeof(pInf->sTime     ), sizeof(pInf->sTime     ), pInf->sTime     );
		printf("\tpInf->sTrCodeSeq = [%*.*s]\n", sizeof(pInf->sTrCodeSeq), sizeof(pInf->sTrCodeSeq), pInf->sTrCodeSeq);
		printf("\tpInf->sFiller    = [%*.*s]\n", sizeof(pInf->sFiller   ), sizeof(pInf->sFiller   ), pInf->sFiller   );
		printf("---------------------------------------------------\n");
	}

	if (flag)
	{
		printf("---- bxa_trlst.cfg --------------------------------\n");
		printf("\tSERVICE = [%s]\n", CfgGetBxaTr("SERVICE"));
		printf("\tFEPID   = [%s]\n", CfgGetBxaTr("FEPID"  ));
		printf("\tORGCD   = [%s]\n", CfgGetBxaTr("ORGCD"  ));
		printf("\tSNDRCV  = [%s]\n", CfgGetBxaTr("SNDRCV" ));
		printf("\tFILEID  = [%s]\n", CfgGetBxaTr("FILEID" ));
		printf("\tTRCODE  = [%s]\n", CfgGetBxaTr("TRCODE" ));
		printf("\tTRCDLEN = [%s]\n", CfgGetBxaTr("TRCDLEN"));
		printf("\tRECLEN  = [%s]\n", CfgGetBxaTr("RECLEN" ));
		printf("\tCOMMENT = [%s]\n", CfgGetBxaTr("COMMENT"));
		printf("---------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * 내용 : 순차적인 값을 리턴한다.
 */
/*----------------------------------------------------------------------------*/
int WriteBatFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatInf == 0x00)
			return;

		FQ2* pFq = FQ2Open("OAR", sServiceid);

		FQ2Write(pFq, sBatInf);

		FQ2Close(pFq);
	}

	if (flag) PrintBatInf();

	return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef _EXE

/*============================================================================*/
/*
 * 위에서 정의한 함수를 테스트 한다.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 기본 처리
		 */
		sProgName = argv[0];

		if (argc < 2)
		{
			/*
			 * USAGE
			 */
			printf("ERROR : Invalid argument... (Usage : %s SERVICE_NAME)\n", sProgName);
			exit(0);
		}

		if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-ver") == 0)
		{
			/*
			 * VERSION
			 */
			printf("Program : [%s], Version : %s\n", sProgName, VERSION);
			exit(0);
		}
	}

	if (flag)
	{
		/*
		 * 프로세스에서 사용하는 서비스명과 FEPID를 얻는다.
		 */
		if (isupper(argv[1][0]))
		{
			iForkFlag = 1;
		}
		else
		{
			iForkFlag = 0;
		}

		UtilToUpper(argv[1]);
		sprintf(sServiceid, "%-10.10s", argv[1]);
		sprintf(sFepid, "%-5.5s", sServiceid + 3);

		if (flag) printf("\x1b[44m< %s %s %s >\x1b[0m\n", sProgName, sServiceid, sFepid);
	}
	
	if (flag)
	{
		/*
		 * master 환경파일을 읽는다. biz.properties 파일도 읽는다.
		 */
		if (CfgMaster() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		CfgBxa();

		if (flag)
		{
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.version"));
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.serialkey"));
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.author"));
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.company"));
			printf("CfgGetBiz    [%s]\n", CfgGetBiz("RUN_SCRIPT"));

			printf("CfgGetBxa    [%s]\n", CfgGetBxa("ORG_DESC"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SND_HOST"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SND_PORT"));
			printf("Port         [%d]\n", UtilToInt(CfgGetBxa("SND_PORT")));

			/*
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SENDER_ID"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SENDER_PWD"));
			*/

			strcpy(sUserid, "");
			strcpy(sPasswd, "");
		}
	}

	if (flag && iForkFlag)
	{
		/*
		 * fork를 이용하여 child process 를 실행한다.
		 */
		setpgrp();
		switch(fork())
		{
			case -1: printf("ERROR : fork error : [errno=%d]\n", errno); exit(9);
			case  0: break;    /* child process */
			default: exit(0);  /* parent process */
		}

		/*
		 * 표준출력을 LOG처리한다.
		 */
		if (flag)
		{
			char strPath[128];
			char strFile[128];

			sprintf(strPath, CfgGetMst("ib.path.log"));
			UtilSetYYYYMMDD(strPath);
			mkdir(strPath, 0777);
			
			if (flag) sprintf(strFile, "%s/%s", strPath, sServiceid);
			if (!flag) sprintf(strFile, "%s/%s", strPath, sFepid);   /* ERROR : 두 프로세스 사용시 */
			if (flag) LOG(_FL_, 0, 0, "LOG FILE => [%s]\n", strFile);
			
			if (access(strFile, R_OK) < 0)
			{
				/* 최초생성 */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND 모드 */
				freopen(strFile, "a+", stdout);
			}
			
			if (flag) LOG(_FL_, 0, 0, "\x1b[44m---------- process START (%s) ----------\x1b[0m" , sServiceid);
			if (flag) LOG(_FL_, 0, 0,"LOG_FILE => [%s]", strFile );
				
			fflush(stdout);
		}
		
		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (!flag)
	{
		/*
		 * 프로그램이 클라이언트인 경우
		 */

		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (CSockGetFdCount('C') == 0)
			{
				/*
				 * 서버에 접속을 시도한다.
				 */

				CSockFDZERO();
				if (CSockSocket(CfgGetBxa("SND_HOST"), UtilToInt(CfgGetBxa("SND_PORT")), 1) == FALSE)
				{
					sleep(RECONN_SEC);
					continue;
				}

				if (flag) CSockPrintSocketTable();

				/*
				 * link 처리를 한다. link전문 송신
				 */
			}

			if (flag)
			{
				/*
				 * 배치파일을 받는다.
				 */
				if (CSockWriteRead(RecvBatFileEDI) < 0)
				{
					sleep(LOOP_TIME_SEC);
					continue;
				}
			}

			if (!flag)
			{
				/*
				 * FQ에 수신자료를 쓴다.
				 * 쓴 후에는 *sDataLine = 0x00
				 * 이 함수는 RecvBatFileEDI에서 처리하도록 됨.
				 * 그래서 사용하지 않는다.
				 */
				WriteBatFQ();
			}

			if (flag)
			{
				/*
				 * polling 처리를 한다. poll전문 송신만.
				 */
			}

			if (flag) sleep(60);
		}
	}
	
	/******************************************************************************/

    if (flag) LOG(_FL_, 0, 0, "프로그램을 시작합니다.");
    	
    SigSetSignal();
	
	if (flag)
	{
		/*
		 * 프로그램이 서버인 경우
		 */
		SSockFDZERO();
		
		SSockSocket(UtilToInt(CfgGetBxa("SND_PORT")));

		if (flag) SSockPrintSocketTable();

		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "클리이언트 접속 대기");
				/*
				 * 클라이언트의 접속을 처리한다.
				 */
				 
				SSockAccept();
				
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "배치 수신");
				/*
				 * 배치파일을 받는다.
				 */
				if (SSockWriteRead(RecvBatFileEDI) < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "sleep 60");
			        /* 세션을 종료한다. */
                    if (flag) sleep(60);
                    if (flag) continue;
                    if (!flag) break;
				}
			}

			if (!flag)
			{
				/*
				 * FQ에 수신자료를 쓴다.
				 * 쓴 후에는 *sDataLine = 0x00
				 */
				WriteBatFQ();
			}

			if (flag) sleep(60);
            if (!flag) break;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "프로그램을 종료합니다.");
}

#endif
