/*
 * PROGRAM ID   : bxs.c
 * PROGRAM NAME : bxs 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : bxs 모듈
 * HISTORY      : 2013.08.04 강석. 최초작성
 *
 * KSN 배치파일 송신
 *
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: 최초작업"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.11.001: 최초작업"

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   RECONN_SEC               5
#define   MAX_DATA_LINE         8192

#define   MAX_MSG_DTL           2000

#define   BLOCK_RECS             100

#define   LOOP_TIME_SEC         (2)

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
 * EDI 배치 관련 헤더 : KS-NET   KSN
 *
 *    typedef struct _MSG_HDR_KSN_  // (23)
 *    {
 *        char    sMsgLen   [   4];  // TCP/IP 전문송신 Byte수
 *        char    sBizKnd   [   3];  // 업무구분코드 'KTB'
 *        char    sOrgzCd   [   3];  // 금융기관코드 '269'
 *        char    sMsgCls   [   4];  // 전문종별코드
 *        char    sTrxCls   [   1];  // 거래구분코드 'S', 'R'
 *        char    sSrFlag   [   1];  // 송수신 Flag C:Center, B:Bank
 *        char    sFileNm   [   8];  // 파일구분코드  0600/001, 0600/004, SPACE
 *        char    sRespCd   [   3];  // 응답코드
 *    } MSG_KSN_HDR;
 *
 */
/*----------------------------------------------------------------------------*/

typedef MSG_KSN_HDR    MSG_HDR ;
typedef MSG_KSN_LINE   MSG_LINE;
typedef MSG_KSN_0600   MSG_MANG;
typedef MSG_KSN_0630   MSG_FILE;
typedef MSG_KSN_0310   MSG_DATA;
typedef MSG_KSN_0620   MSG_MREQ;
typedef MSG_KSN_0300   MSG_MRES;

MSG_HDR *       pHdr            ;
MSG_LINE*       pLine           ;

MSG_MANG*       pMang           ;                  /* 0600, 0610 전문  : MSG_MANG  */
MSG_FILE*       pFile           ;                  /* 0630, 0640 전문  : MSG_FILE  */
MSG_DATA*       pData           ;                  /* 0310, 0320 전문  : MSG_DATA  */
MSG_MREQ*       pMreq           ;                  /* 0620 전문        : MSG_MREQ  */
MSG_MRES*       pMres           ;                  /* 0300 전문        : MSG_MRES  */

/*----------------------------------------------------------------------------*/

int             iForkFlag       ;

int             iRSeq           ;                  /* read sequence number */

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

char            sUserid   [20+1];                  /* Userid                    */
char            sPasswd   [16+1];                  /* Passwd                    */

char            sFileId   [20+1];                  /* 송수신 파일명             */

FILE*           fp              ;                  /* 배치파일 fp               */
long            lPos            ;                  /* 배치파일 포지션           */

int             iMsgRecs        ;                  /* Message 안의 Record 수    */

int             iFileIdx        ;
int             iFileSize       ;                  /* 송신할 배치파일 갯수      */

int             iBlockIdx       ;
int             iBlockSize      ;                  /* 송신할 배치파일의 블럭 수 */

int             iRecIdx         ;
int             iRecSize        ;                  /* 송신할 배치파일의 라인 수 */

int             iMissSeq        ;

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
 * 내용 : FQ의 내용을 읽는다. sBatInfo 값을 세팅한다. 그리고 BAT_INF 출력.
 */
/*----------------------------------------------------------------------------*/
int ReadBatFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * FQ에서 배치정보를 읽는다.
		 */
		FQ2* pFq = FQ2Open("OAS", sServiceid);

		char* p = FQ2Read(pFq);
		if (p == NULL)
		{
			FQ2Close(pFq);
			return -1;
		}
		strcpy(sBatInf, p);

		FQ2Close(pFq);
	}

	if (flag)
	{
		/*
		 * 배치정보에서 sSR, sTrCode로 bxa_trlst.cfg 파일 정보를 읽는다.
		 */
		char sSR[10];
		char sTrCode[20];
		BAT_INF* pInf = (BAT_INF*) &sBatInf;
		sprintf(sSR    , "%*.*s", sizeof(pInf->sSR    ), sizeof(pInf->sSR    ), pInf->sSR    );
		sprintf(sTrCode, "%*.*s", sizeof(pInf->sTrCode), sizeof(pInf->sTrCode), pInf->sTrCode);

		CfgBxaTr(sSR, sTrCode);

		sprintf(sFileId, "%-8.8s", CfgGetBxaTr("FILEID"));

		if (flag) printf("CfgBxaTr('%s','%s') -> sFileId=[%s]\n", sSR, sTrCode, sFileId);
	}

	if (flag) PrintBatInf();

	return 0;
}

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

			sprintf(sSendEdiLine, "%04d%3.3s%3.3s%4.4s%1.1s%1.1s%-8.8s%3.3s%s"
				, iSendEdiLen - 4
				, "KTB"
				, "269"
				, sMsgCls
				, "R"
				, "B"
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
				LOG(_FL_, 0, 0, "SEND(%d): EDI_HDR+DATA [%*.*s][%s]"
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
				LOG(_FL_, 0, 0, "RECV(%d): EDI_HDR+DATA [%*.*s][%s]"
					, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), (char*)&pLine->stHdr
					, sDataLine);
			}
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : 배치파일 정보를 세팅한다.
 *     실재 파일정보(BAT_INF)를 읽어 처리를 한다.
 *     임시 테스트를 위한 함수임.
 */
/*----------------------------------------------------------------------------*/
int GetBatchFileInfo( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 아래 값은 실재 자료를 읽어서 처리한다.
		 */
		iRecSize   = 225;

		iFileSize  = 1;
		iBlockSize = (iRecSize + BLOCK_RECS - 1) / BLOCK_RECS;

		iFileIdx   = 0;
		iBlockIdx  = 0;
		iRecIdx    = 0;
	}

	return 0;
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
int RecvBatFileOPEN(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvDataEDI(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * OPEN : 업무개시 전문을 세팅한다.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "001"
			, sUserid
			, sPasswd);

		if (!flag)
		{
			/*
			 * Password를 복호화한다.
			 */
			char sDecStr[20];
			strcpy(sDecStr, GetPassword("KFT51", 2, "HCMSTEST", sPasswd, "269" ));
			printf("OUTPUT 디코딩=[%s]\n", sDecStr );
		}

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
			, sUserid
			, sPasswd);

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
 * 내용 : 파일정보 0630 -> 0640
 *     TODO DATE.2013.08.06 : FileID에 대한 정보 필요
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
int SendBatFileFILE(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 배치파일에 관한 처리를 한다.
		 */
		if (flag)
		{
			char sFileName[128];
			int iRecLen;
			int iRecCnt;
			struct stat fst;

			if (flag)
			{
				/*
				 * 파일명을 만든다.
				 */
				BAT_INF* pInf = (BAT_INF*) &sBatInf;
				sprintf(sFileName, CfgGetMaster("ib.path.file.bxscom")
					, sizeof(pInf->sSR       ), sizeof(pInf->sSR       ), pInf->sSR
					, sizeof(pInf->sOrgCode  ), sizeof(pInf->sOrgCode  ), pInf->sOrgCode
					, sizeof(pInf->sTrCode   ), sizeof(pInf->sTrCode   ), pInf->sTrCode
					, sizeof(pInf->sDate     ), sizeof(pInf->sDate     ), pInf->sDate
					, sizeof(pInf->sTrCodeSeq), sizeof(pInf->sTrCodeSeq), pInf->sTrCodeSeq
					);

				UtilSetYYYYMMDD(sFileName);

				iRecLen = UtilToIntN(pInf->sRecLen, sizeof(pInf->sRecLen));
				iRecCnt = UtilToIntN(pInf->sRecCnt, sizeof(pInf->sRecCnt));

				/* 배치파일 정보를 출력 */
				if (flag) LOG(_FL_, 0, 0, "INF(%d): BAT_FILE [RecLen=%d][RecCnt=%d][%s]"
					, cSock, iRecLen, iRecCnt, sFileName);
			}

			if (flag)
			{
				/*
				 * 배치파일을 오픈한다.
				 */
				fp = fopen(sFileName, "r+");

				/*
				 * 배치파일 처리를 위한 초기값을 세팅한다.
				 */
				iRecSize = iRecCnt;       /* Header, Body, Tailer 모두 포함한 record 건수 */

				if (flag)
				{
					iMsgRecs = MAX_MSG_DTL / iRecLen;  /* 한개 Msg에 넣을 수 있는 record 건수 */
				}
				else
				{
					iMsgRecs = 1;                      /* 한개 Msg에 한개의 record */
				}

				iFileSize  = 1;
				iBlockSize = (((iRecSize + iMsgRecs - 1) / iMsgRecs) + BLOCK_RECS - 1) / BLOCK_RECS;

				iFileIdx   = 0;
				iBlockIdx  = 0;
				iRecIdx    = 0;
			}

			if (flag)
			{
				/* 실제 파일사이즈가 아닌 (레코드길이 * 건수)  */

				fstat(fileno(fp), &fst);

				/*
				 *FILE : 파일정보 전문을 세팅한다.
				 */
				sprintf(sDataLine, "%-8.8s%012d%04d"
					, sFileId
					, iRecLen * iRecCnt    /* 데이터의 길이 */
					, iRecLen);            /* 전문 Byte     */
			}

			if (!flag)
			{
				/* 실제 파일사이즈가 아닌 (레코드길이 * 건수)  */

				fstat(fileno(fp), &fst);

				/*
				 *FILE : 파일정보 전문을 세팅한다.
				 */
				sprintf(sDataLine, "%4.4s%4.4s%012d%04d"
					, CfgGetBxaTr("FILEID")
					, TimeGetDate() + 4
					, fst.st_size        /* 파일 사이즈 */
					, iRecLen);            /* 전문 Byte   */

				/*
				 * 송신파일명을 만든다.
				 */
				sprintf(sFileId, "%-4.4s%4.4s"
					, CfgGetBxaTr("FILEID")
					, TimeGetDate() + 4);
			}
		}

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0630") == FALSE)
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

		if (flag)
		{
			/*
			 * DATE.2013.09.26
			 * 수신한 전문의 응답코드를 확인한다.
			 */
			char sResCode[10];

			sprintf(sResCode, "%*.*s", sizeof(pLine->stHdr.sRespCd)
				, sizeof(pLine->stHdr.sRespCd), pLine->stHdr.sRespCd);

			if (strcmp("000", sResCode) != 0)
			{
				if (strcmp("090", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 시스템 장애 = '%s'", sResCode);
				}
				else if (strcmp("310", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 송신자명 오류 = '%s'", sResCode);
				}
				else if (strcmp("320", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 숭신자암호 오류 = '%s'", sResCode);
				}
				else if (strcmp("630", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 기전송 완료 = '%s'", sResCode);
				}
				else if (strcmp("631", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 미등록 업무 = '%s'", sResCode);
				}
				else if (strcmp("632", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 비정상 파일명 = '%s'", sResCode);
				}
				else if (strcmp("633", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : 비정상 전문 BYTE 수 = '%s'", sResCode);
				}
				else if (strcmp("800", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : FORMAT 오류 = '%s'", sResCode);
				}
				else
				{
					LOG(_FL_, 0, 0, "ERROR : stHdr.sRespCd = '%s'", sResCode);
				}

				return -1;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : DATA송신 (BLOCK)   0320
 *     TODO DATE.2013.08.06 : 파일의 레코드를 읽어 처리
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
int SendBatFileDATA(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		static char str[MAX_DATA_LINE];
		BOOL iFlagEof = FALSE;

		lPos = ftell(fp);

		int i;
		for (i=0; i < BLOCK_RECS && iFlagEof == FALSE; i++)
		{
			*str = 0x00;

			if (flag)
			{
				/*
				 * 여러개의 record를 넣은 Msg를 만든다.
				 * 여러개의 buf를 넣은 str를 만든다.
				 */
				char buf[MAX_DATA_LINE];
				int k;
				for (k=0; k < iMsgRecs; k++)
				{
					if (fgets(buf, MAX_DATA_LINE, fp))
					{
						buf[strlen(buf) - 1] = 0x00;
						strcat(str, buf);
						iRecIdx ++;
					}
					else
					{
						iFlagEof = TRUE;
						break;
					}
				}
			}

			/*
			 * DATA : 파일 레코드를 세팅한다.
			 */
			sprintf(sDataLine, "%04d%03d%04d%s"
				, (iBlockIdx + 1)
				, (i + 1)
				, strlen(str)
				, str);

			/*
			 * 송신한다. 송신데이터는 sDataLine
			 */
			if (SendDataEDI(cSock, "0320") == FALSE)
			{
				return -1;
			}
		}

		iMissSeq = i;
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 결번확인   0620 -> 0300
 *
 *    typedef struct _MSG_0620_LTC_  // header + (7)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO : 결번처리할 BLOCK 번호
 *        char    sSeqNo    [   3];  // SEQUENCE NO : 결번처리할 BLOCK의 최종 전송 DATA의 SEQ-NO
 *    } MSG_LTC_0620;
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatFileMISS(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * FILE : 파일정보 전문을 세팅한다.
		 */
		sprintf(sDataLine, "%04d%03d"
			, (iBlockIdx + 1)
			, iMissSeq);

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendDataEDI(cSock, "0620") == FALSE)
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
 * 내용 : 결번 DATA송신    0310
 *
 *    typedef struct _MSG_0300_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sMisCnt   [   3];  // 결번 갯수
 *        char    sMisBit   [ 100];  // 결번 DATA
 *    } MSG_LTC_0300;
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
int SendBatFileMDAT(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 결번자료
		 */
		char sMissDat[1024];
		MSG_MRES* pMres = (MSG_MRES*) &sMissDat;

		strcpy(sMissDat, sDataLine);

		/*
		 * 파일의 위치를 이동한다.
		 */
		long lPosOld = ftell(fp);
		fseek(fp, lPos, SEEK_SET);

		static char str[MAX_DATA_LINE];

		/*
		 * 각 블럭의 시작번호를 구한다.
		 */
		int idx = ((iRecIdx - 1) / BLOCK_RECS) * BLOCK_RECS;
		if (!flag) LOG(_FL_, 0, 0, "[SendBatFileMDAT]...[idx=%d][iRecIdx=%d][iRecSize=%d] ", idx, iRecIdx, iRecSize);

		int i;
		for (i=0; i < BLOCK_RECS && pMres->sMisBit[i] && idx < iRecSize; i++, idx++)
		{
			/*
			 * 배치파일 자료를 읽는다.
			 */
			if (fgets(str, MAX_DATA_LINE, fp))
			{
				str[strlen(str) - 1] = 0x00;     /* '\n'를 제가한다. */
			}
			else
			{
				str[0] = 0x00;     /* 자료가 없다. */
			}

			/*
			 * 플래그 : 0.결번, 1.성공
			 */
			if (pMres->sMisBit[i] == '1')
			{
				continue;
			}

			/*
			 * DATA : 파일 레코드를 세팅한다.
			 */
			sprintf(sDataLine, "%04d%03d%04d%s"
				, (iBlockIdx + 1)
				, (i + 1)
				, strlen(str)
				, str);

			/*
			 * 송신한다. 송신데이터는 sDataLine
			 */
			if (SendDataEDI(cSock, "0310") == FALSE)
			{
				return -1;
			}
		}

		fseek(fp, lPosOld, SEEK_SET);
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 이어보내기    0600/002 -> 0610/002
 *     다음 파일이 있는 경우, 이어보내기로 처리한다.
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
int SendBatFileCONT(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 배치파일을 클로우즈한다.
		 */
		if (flag)
		{
			fclose(fp);
		}

		/*
		 * CONT : 이어보내기 전문을 세팅한다.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "002"
			, sUserid
			, sPasswd);

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
 * 내용 : 파일종료      0600/003 -> 0610/003
 *     파일 송신을 마침..
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
int SendBatFileFEND(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * FEND : 파일종료 전문을 세팅한다.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "003"
			, sUserid
			, sPasswd);

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
 * 내용 : 업무종료      0600/004 -> 0610/004
 *    관련업무를 종료함.
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
int SendBatFileDONE(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * DONE : 업무종료 전문을 세팅한다.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "004"
			, sUserid
			, sPasswd);

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
/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int SendBatFileEDI(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret;

	if (flag)
	{
		/*
		 * 배치파일 정보를 세팅한다.
		 */
		/* GetBatchFileInfo(); */

		if (flag)
		{
			if (flag)
			{
				/*
				 * 업무개시 (당사발생)     0600/001 ->  <- 0610/001
				 */
				ret = SendBatFileOPEN(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
			else
			{
				/*
				 * 업무개시 (타사발생)     0600/001 ->  <- 0610/001
				 */
				ret = RecvBatFileOPEN(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
		}

		while (TRUE)
		{
			/*
			 * 파일정보          0630 ->  <- 0640
			 */
			ret = SendBatFileFILE(cSock);
			if (ret < 0)
			{
				return ret;
			}

			while (TRUE)
			{
				/*
				 * DATA송신 (BLOCK)            0320 =>
				 */
				ret = SendBatFileDATA(cSock);
				if (ret < 0)
				{
					return ret;
				}

				while (TRUE)
				{
					/*
					 * 결번확인         0620 ->  <- 0300
					 */
					ret = SendBatFileMISS(cSock);
					if (ret < 0)
					{
						return ret;
					}

					/*
					 * 결번자료 있는지 확인.
					 */
					MSG_MRES* pMres = (MSG_MRES*) &sDataLine;
					if (UtilToIntN(pMres->sMisCnt, sizeof(pMres->sMisCnt)) != 0)
					{
						/*
						 * 결번 DATA송신         0310 =>
						 */
						ret = SendBatFileMDAT(cSock);
						if (ret < 0)
						{
							return ret;
						}

						continue;
					}

					break;
				}

				iBlockIdx ++;

				/*
				 * 다음 블럭이 있으면 다시 DATA 송신
				 */
				if (iBlockIdx < iBlockSize)
				{
					continue;
				}

				break;
			}

			iFileIdx ++;

			/*
			 * 다음 파일이 있으면 다시 파일정보 송신
			 */
			if (iFileIdx < iFileSize)
			{
				/*
				 * 이어보내기      0600/002 ->  <- 0610/002
				 */
				ret = SendBatFileCONT(cSock);
				if (ret < 0)
				{
					return ret;
				}

				iBlockIdx  = 0;
				iRecIdx    = 0;

				continue;
			}

			/*
			 * 파일종료         0600/003 ->  <- 0610/003
			 */
			ret = SendBatFileFEND(cSock);
			if (ret < 0)
			{
				return ret;
			}

			if (!flag) return 0;

			break;
		}

		/*
		 * 업무종료              0600/004 ->  <- 0610/004
		 */
		ret = SendBatFileDONE(cSock);
		if (ret < 0)
		{
			return ret;
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

		if (!flag)
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

			strcpy(sUserid, "");
			strcpy(sPasswd, "");
		}

		if (!flag)
		{
			char sOrg[100];
			char sEnc[100];
			char sDec[100];

			strcpy(sOrg, CfgGetBxa("SENDER_PWD"));

			strcpy(sEnc, GetPassword("KFT51", 1, "HCMSTEST", sOrg, "243" ));
			printf("OUTPUT 인코딩암호=[%s]\n", sEnc );

			/*
			 *  복호화
			 */
			strcpy(sDec, GetPassword("KFT51", 2, "HCMSTEST", sEnc, "243" ));
			printf("OUTPUT 디코딩암호=[%s]\n", sDec );
		}
	}

	if (!flag && iForkFlag)
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

			if (flag) LOG(_FL_, 0, 0, "[%s]\n", strFile);
			if (flag) LOG(_FL_, 0, 0, "[%s]\n", TimeGetDateTime2());
			fflush(stdout);
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

    if (flag) LOG(_FL_, 0, 0, "프로그램을 시작합니다.");
    	
    SigSetSignal();

	if (flag)
	{
		/*
		 * 프로그램이 클라이언트인 경우
		 */
		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (CSockGetFdCount('C') == 0)
			{
				if (flag) LOG(_FL_, 0, 0, "서버에 접속 시도");
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
				if (flag) LOG(_FL_, 0, 0, "FQ에서 송신자료 읽음");
				/*
				 * FQ에서 송신자료를 읽는다.
				 * 송신자료가 없으면 *sDataLine = 0x00
				 */
				if (ReadBatFQ() < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "FQ자료 없음 -> 세션 종료");
			        /* 세션을 종료한다. */
			        CSockFDCLR_All();
                    if (!flag) sleep(60);
                    if (!flag) continue;
                    if (flag) break;
				}
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "배치파일 송신");
				/*
				 * 배치파일을 보낸다.
				 */
				if (CSockWriteRead(SendBatFileEDI) < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "ERROR....");
					break;
				}
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "POLL전문 송신");
				/*
				 * polling 처리를 한다. poll전문 송신만.
				 */
				CSockFDCLR_All();
			}

            if (!flag) sleep(60);
            if (flag) break;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "프로그램을 종료합니다.");
}

#endif
