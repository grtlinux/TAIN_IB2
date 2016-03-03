/*
 * PROGRAM ID   : bxs.c
 * PROGRAM NAME : bxs 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : bxs 모듈
 * HISTORY      : 2013.08.04 강석. 최초작성
 *
 * 삼성네트웍 배치파일 송신
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
#define   MAX_DATA_LINE      8192000

#define   BLOCK_RECS             100
#define   MAX_MSG_DTL           2000

#define   MSGTYPE_LOGIN_REQ       10
#define   MSGTYPE_LOGIN_RES       20
#define   MSGTYPE_LOGOUT_REQ      30
#define   MSGTYPE_LOGOUT_RES      40
#define   MSGTYPE_FILE_REQ        50
#define   MSGTYPE_FILE_RES        60
#define   MSGTYPE_CHECK           70
#define   MSGTYPE_NONE            90        /* not defined. */

#define   FLAG_DANGBAL          TRUE        /* 당발발생 */

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * 배치 관련 헤더 : 삼성네트웍
 */
/*----------------------------------------------------------------------------*/

typedef MSG_SSN_HDR     MSG_HDR   ;
typedef MSG_SSN_LOGIN   MSG_LOGIN ;
typedef MSG_SSN_LOGOUT  MSG_LOGOUT;
typedef MSG_SSN_FILE    MSG_FILE  ;
typedef MSG_SSN_CHECK   MSG_CHECK ;
typedef MSG_SSN_LINE    MSG_LINE  ;

MSG_HDR   *             pHdr      ;
MSG_LINE  *             pLine     ;

MSG_LOGIN *             pLogin    ;
MSG_LOGOUT*             pLogout   ;
MSG_FILE  *             pFile     ;
MSG_CHECK *             pCheck    ;

BAT_INF   *             pInf      ;

/*----------------------------------------------------------------------------*/

int             iForkFlag       ;

int             iRSeq           ;                  /* read sequence number */

char            sBatInf         [MAX_DATA_LINE];   /* FEP 배치정보 from FQ */

char            sFileInf        [MAX_DATA_LINE];   /* 배치파일정보         */

char            sSendLine       [MAX_DATA_LINE];   /* 송신전문 */
int             iSendLen        ;

char            sRecvLine       [MAX_DATA_LINE];   /* 수신전문 */
int             iRecvLen        ;

char            sBatHdr         [MAX_DATA_LINE];   /* 배치전문의 헤더부 */

char            sDataLine       [MAX_DATA_LINE];   /* 배치전문의 데이터부 */
int             iDataLen        ;

/*----------------------------------------------------------------------------*/

char            sUserid   [20+1];                  /* Userid          */
char            sPasswd   [20+1];                  /* Passwd          */
char            sFileId   [20+1];                  /* 송수신 파일명   */

char            sFileName [ 512];                  /* 파일명          */

FILE*           fp              ;                  /* 배치파일 fp     */

int             iRecLen         ;
int             iRecCnt         ;

int             iTrCodeSeq      ;                  /* TrCode Sequence : 01 ~ 99 */

char            sSequence [  50];
char            sSubject  [  50];


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

		pInf = (BAT_INF*) &sBatInf;

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
		pInf = (BAT_INF*) &sBatInf;
		sprintf(sSR    , "%-*.*s", sizeof(pInf->sSR    ), sizeof(pInf->sSR    ), pInf->sSR    );
		sprintf(sTrCode, "%-*.*s", sizeof(pInf->sTrCode), sizeof(pInf->sTrCode), pInf->sTrCode);

		if (flag) printf("STATUS: CfgBxaTr('%s', '%s');\n", sSR, sTrCode);

		CfgBxaTr(sSR, sTrCode);
	}

	if (flag)
	{
		/*
		 * 파일명을 만든다.
		 */
		pInf = (BAT_INF*) &sBatInf;
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
		if (flag) LOG(_FL_, 0, 0, "INF: BAT_FILE [RecLen=%d][RecCnt=%d][%s]"
			, iRecLen, iRecCnt, sFileName);
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
void PrintBatHdr()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatHdr == 0x00)
			return;

		MSG_HDR* p = (MSG_HDR*) &sBatHdr;

		printf("---- MSG_HDR* p --------------------------------\n");
		printf("[%*.*s]\n", sizeof(MSG_HDR), sizeof(MSG_HDR), sBatHdr);
		printf("\tp->sTrCode   = [%*.*s]\n", sizeof(p->sTrCode)  , sizeof(p->sTrCode)  , p->sTrCode  );
		printf("\tp->sTranNum  = [%*.*s]\n", sizeof(p->sTranNum) , sizeof(p->sTranNum) , p->sTranNum );
		printf("\tp->sResCode  = [%*.*s]\n", sizeof(p->sResCode) , sizeof(p->sResCode) , p->sResCode );
		printf("\tp->sReserved = [%*.*s]\n", sizeof(p->sReserved), sizeof(p->sReserved), p->sReserved);
		printf("---------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * 내용 : sDataLine 자료를 송신한다.
 */
/*----------------------------------------------------------------------------*/
BOOL SendData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			iSendLen = strlen(sSendLine);

			/*
			 * 송신한다.
			 */
			int ret = SockSend(cSock, sSendLine, iSendLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}
		}

		/*
		 * 송신데이터를 출력한다.
		 */
		if (flag)
		{
			LOG(_FL_, 0, 0, "SEND(%d): HDR+DATA [%*.*s][%s]"
				, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), sSendLine
				, sSendLine + sizeof(MSG_HDR));
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : 수신한 자료를 sDataLine 복사한다.
 */
/*----------------------------------------------------------------------------*/
BOOL RecvData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			iRecvLen = 150;

			/*
			 * 150바이트 전문을 읽는다.
			 * HDR(40) + CMD(110)
			 */
			int ret = SockRecv(cSock, sRecvLine, iRecvLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}
		}

		sRecvLine[iRecvLen] = 0x00;

		/*
		 * 수신데이터를 출력한다.
		 */
		if (flag)
		{
			LOG(_FL_, 0, 0, "RECV(%d): HDR+DATA [%*.*s][%s]"
				, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), sRecvLine
				, sRecvLine + sizeof(MSG_HDR));
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : 업무개시 (당사/타사)   0600/001 -> 0610/001
 */
/*----------------------------------------------------------------------------*/
int GetMsgType(char* sData)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		pHdr = (MSG_HDR*) sData;

		switch (UtilToIntN(pHdr->sTranNum, sizeof(pHdr->sTranNum)))
		{
			case   3: return MSGTYPE_LOGIN_REQ ;
			case  30: return MSGTYPE_LOGIN_RES ;
			case   7: return MSGTYPE_LOGOUT_REQ;
			case  70: return MSGTYPE_LOGOUT_RES;
			case 100: return MSGTYPE_FILE_REQ  ;
			case 110: return MSGTYPE_FILE_RES  ;
			case 130: return MSGTYPE_CHECK     ;
			default:
				if (flag) LOG(_FL_, 0, 0, "[GetMsgType]...not defined[%s] ", sRecvLine);
				return MSGTYPE_NONE;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * 내용 : Recv LOGIN
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // 전문번호 (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // 응답코드
 *     sBizFld   [  20];  // 업무필드 (VAN에서 활용)
 *     sReserved [   5];  // 예비
 *     sUserid   [  20];  // VAN 사용자ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(송신), RD(수신), RS(송신List), RR(수신List)
 *     sSequence [  10];  // 수신시 특정파일만 수신하고자할 때
 *     sSubject  [  10];  // 수신시 Subject (공백이면 모두 수신)
 *     sFlag     [   1];  // 수신시 E(미수신),R(기수신),A(미수신+기수신)
 *     sStTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sEdTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sYnChgPw  [   1];  // Y : change password
 *     sNewPass  [   8];  // new password
 *     sCommSize [   4];  // default : 2048
 *     sChgCode  [   1];  // Code변환여부
 *     sTransId  [  10];  // 거래처ID
 *     sFiller   [  11];  // 예비
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatLogin(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * LOGIN 전문을 세팅한다.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%1.1s%-8.8s%-4.4s%1.1s%-10.10s%-11.11s"
			, ""                                 /* sTrCode   */
			, "030"                              /* sTranNum  */
			, ""                                 /* sResCode  */
			, ""                                 /* sBizFld   */
			, ""                                 /* sReserved */
			, "HANCMS"                           /* sUserid   */
			, "HANCMS"                           /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, "FS082004"                                 /* sSubject  */
			, "E"                                /* sFlag     */
			, ""                                 /* sStTime   */
			, ""                                 /* sEdTime   */
			, ""                                 /* sYnChgPw  */
			, ""                                 /* sNewPass  */
			, "2048"                             /* sCommSize */
			, "N"                                /* sChgCode  */
			, ""                                 /* sTransId  */
			, ""                                 /* sFiller   */
		);

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : Send LOGIN
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // 전문번호 (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // 응답코드
 *     sBizFld   [  20];  // 업무필드 (VAN에서 활용)
 *     sReserved [   5];  // 예비
 *     sUserid   [  20];  // VAN 사용자ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(송신), RD(수신), RS(송신List), RR(수신List)
 *     sSequence [  10];  // 수신시 특정파일만 수신하고자할 때
 *     sSubject  [  10];  // 수신시 Subject (공백이면 모두 수신)
 *     sFlag     [   1];  // 수신시 E(미수신),R(기수신),A(미수신+기수신)
 *     sStTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sEdTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sYnChgPw  [   1];  // Y : change password
 *     sNewPass  [   8];  // new password
 *     sCommSize [   4];  // default : 2048
 *     sChgCode  [   1];  // Code변환여부
 *     sTransId  [  10];  // 거래처ID
 *     sFiller   [  11];  // 예비
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatLogin(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sStTime[20];
		char sEdTime[20];
		
		if (flag)
		{
			char sYear[10];
			sprintf(sYear, "%4.4s", TimeGetYear());
			
			if (strcmp(sYear, "2014") <= 0)
			{
				/* INFO : 20140102 : 원우석차장이 당일까지 배치파일을 직접받아 처리함 */
				sprintf(sStTime, "201401030000");
				sprintf(sEdTime, "201412312350");
			}
			else
			{
				/* 2015년 이후부터 적용 됨 */
				sprintf(sStTime, "%4.4s01010000", sYear);
				sprintf(sEdTime, "%4.4s12312350", sYear);
			}
		}

		/*
		 * LOGIN 전문을 세팅한다.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%1.1s%-8.8s%-4.4s%1.1s%-10.10s%-11.11s"
			, "         "                        /* sTrCode   */
			, "003"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, "     "                            /* sReserved */
			, "HANCMS             "              /* sUserid   */
			, "HANCMS "                          /* sPasswd   */
			, "SD"                               /* sJobType  */
			, "          "                       /* sSequence */
			, "FS082004"                         /* sSubject  */
			, "E"                                /* sFlag     */
			, sStTime                            /* sStTime   */
			, sEdTime                            /* sEdTime   */
			, " "                                /* sYnChgPw  */
			, "        "                         /* sNewPass  */
			, "2048"                             /* sCommSize */
			, "N"                                /* sChgCode  */
			, "          "                       /* sTransId  */
			, "           "                      /* sFiller   */
		);


		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * 내용 : FILE INFO
 *
 *    sTrCode   [   9];  // Transaction Code
 *    sTranNum  [   3];  // 전문번호 (003,030,007,070,100,110,130)
 *    sResCode  [   3];  // 응답코드
 *    sBizFld   [  20];  // 업무필드 (VAN에서 활용)
 *    sReserved [   5];  // 예비
 *    sSequence [  10];  // 수신시 특정파일만 수신하고자할 때
 *    sSubject  [  10];  // 수신시 Subject (공백이면 모두 수신)
 *    sFileSize [  10];  // 송신할 파일의 size
 *    sSendId   [  20];  // 송신자 ID
 *    sRecvId   [  20];  // 수신자 ID
 *    sFiller1  [   1];  // 예비-1
 *    sFiller2  [  10];  // 예비-2
 *    sYnLast   [   3];  // 마지막파일 여부 NXT:다음있음 END:다음없음
 *    sTranType [   3];  // 전송종류 : 'NEW'
 *    sFiller3  [   1];  // 예비-3
 *    sFiller4  [  22];  // 예비-4
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatFileInfo(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int iFileSize = 0;

		if (flag)
		{
			struct stat fst;
			lstat(sFileName, &fst);
			iFileSize = fst.st_size;
		}

			iRecLen = UtilToIntN(pInf->sRecLen, sizeof(pInf->sRecLen));
			iRecCnt = UtilToIntN(pInf->sRecCnt, sizeof(pInf->sRecCnt));


		/*
		 * LOGIN 전문을 세팅한다.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-10.10s%-10.10s%010d%-20.20s%-20.20s%1.1s%-10.10s%-3.3s%-3.3s%1.1s%-22.22s"
			, "         "                        /* sTrCode   */
			, "100"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, "     "                            /* sReserved */
			, ""                                 /* sSequence */
			, "FS082004"                         /* sSubject  */
			, iRecLen + 34                       /* sFileSize */
			, "HANCMS"                           /* sSendId   */
			, "HHFEDI"                           /* sRecvId   */
			/*, "SENDER"     */                  /* sSendId   */
			/*, "RECEIVER" */                    /* sRecvId   */
			, ""                                 /* sFiller1  */
			, ""                                 /* sFiller2  */
			, "NXT"                              /* sYnLast   */
			, "NEW"                              /* sTranType */
			, " "                                /* sFiller3  */
			, "                      "           /* sFiller4  */
		);

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : DATA
 *
 *     sHdrType  [  10];   // 헤더구분
 *     sBizType  [   1];   // 업무구분
 *     sDatType  [   1];   // 자료형태
 *     sFileSize [  10];   // 파일크기
 *     sCompSize [  10];   // 압축후파일크기
 *     sSendId   [  10];   // 송신자ID
 *     sUpcheGb  [   2];   // 업체구분
 *     sInfoGb   [   3];   // 정보구분
 *     sFiller   [   1];   // 예비
 *     sRecvCnt  [   3];   // 수신자수
 *     sRecvId   [  10];   // 수신자ID
 *     sRecSep   [   2];   // 레코드구분자
 *     sData     [   1];   // 파일의 자료 MAX = 1985
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatFileData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sData[2048*100];
		char sLine[4096];
		int iRead;

		*sData = 0x00;
		
		fp = fopen(sFileName, "r");

		while (TRUE)
		{
			iRead = fread(sLine, 1, 1985, fp);
			if (iRead == 0)
				break;

			sLine[iRead] = 0x00;

			strcat(sData, sLine);
		}

		fclose(fp);


		/*
		 * LOGIN 전문을 세팅한다.
		 */
		sprintf(sSendLine, "%-10.10s%1.1s%1.1s%010d%-10.10s%-10.10s%-2.2s%-3.3s%1.1s%-3.3s%-10.10s%-2.2s%s"
			, "<<NEHEAD>>"                       /* sHdrType  */
			, "1"                                /* sBizType  */
			, "N"                                /* sDatType  */
			, strlen(sData)                      /* sFileSize */  /* TODO : DATE.20130924 변경 필요 */
			, ""                                 /* sCompSize */
			, ""                                 /* sSendId   */
			, ""                                 /* sUpcheGb  */
			, ""                                 /* sInfoGb   */
			, ""                                 /* sFiller   */
			, "1"                                /* sRecvCnt  */
			, ""                                 /* sRecvId   */
			, "LF"                               /* sRecSep   */
			, sData                              /* sData     */
		);

		if (flag) printf("DATA : [%s]\n", sSendLine);

		if (flag)
		{
			iSendLen = strlen(sSendLine);

			/*
			 * 송신한다.
			 */
			int ret = SockSend(cSock, sSendLine, iSendLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}
		}

		/*
		 * 송신데이터를 출력한다.
		 */
		if (!flag)
		{
			LOG(_FL_, 0, 0, "SEND(%d): HDR+DATA [%*.*s][%s]"
				, cSock, sizeof(MSG_LINE)-1, sizeof(MSG_LINE)-1, sSendLine
				, sSendLine + sizeof(MSG_LINE)-1);
		}

	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : CHECK
 */
/*----------------------------------------------------------------------------*/
int RecvBatCheck(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * 내용 : Recv LOGOUT
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // 전문번호 (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // 응답코드
 *     sBizFld   [  20];  // 업무필드 (VAN에서 활용)
 *     sReserved [   5];  // 예비
 *     sUserid   [  20];  // VAN 사용자ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(송신), RD(수신), RS(송신List), RR(수신List)
 *     sSequence [  10];  // 수신시 특정파일만 수신하고자할 때
 *     sSubject  [  10];  // 수신시 Subject (공백이면 모두 수신)
 *     sFlag     [   1];  // 수신시 E(미수신),R(기수신),A(미수신+기수신)
 *     sStTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sEdTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sFiller   [  35];  // 예비
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatLogout(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * LOGIN 전문을 세팅한다.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%-35.35s"
			, ""                                 /* sTrCode   */
			, "070"                              /* sTranNum  */
			, ""                                 /* sResCode  */
			, ""                                 /* sBizFld   */
			, ""                                 /* sReserved */
			, "HANCMS"                           /* sUserid   */
			, "HANCMS"                           /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, "FS082004"                         /* sSubject  */
			, "A"                                /* sFlag     */
			, ""                                 /* sStTime   */
			, ""                                 /* sEdTime   */
			, ""                                 /* sFiller   */
		);

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : Send LOGOUT
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // 전문번호 (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // 응답코드
 *     sBizFld   [  20];  // 업무필드 (VAN에서 활용)
 *     sReserved [   5];  // 예비
 *     sUserid   [  20];  // VAN 사용자ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(송신), RD(수신), RS(송신List), RR(수신List)
 *     sSequence [  10];  // 수신시 특정파일만 수신하고자할 때
 *     sSubject  [  10];  // 수신시 Subject (공백이면 모두 수신)
 *     sFlag     [   1];  // 수신시 E(미수신),R(기수신),A(미수신+기수신)
 *     sStTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sEdTime   [  12];  // 수신시 YYYYMMDDhhmm
 *     sFiller   [  35];  // 예비
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatLogout(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sStTime[20];
		char sEdTime[20];
		
		if (flag)
		{
			char sYear[10];
			sprintf(sYear, "%4.4s", TimeGetYear());
			
			if (strcmp(sYear, "2014") <= 0)
			{
				/* INFO : 20140102 : 원우석차장이 당일까지 배치파일을 직접받아 처리함 */
				sprintf(sStTime, "201401030000");
				sprintf(sEdTime, "201412312350");
			}
			else
			{
				/* 2015년 이후부터 적용 됨 */
				sprintf(sStTime, "%4.4s01010000", sYear);
				sprintf(sEdTime, "%4.4s12312350", sYear);
			}
		}

		/*
		 * LOGIN 전문을 세팅한다.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%-35.35s"
			, "         "                        /* sTrCode   */
			, "007"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, ""                                 /* sReserved */
			, "HANCMS             "              /* sUserid   */
			, "HANCMS "                          /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, "FS082004"                         /* sSubject  */
			, "E"                                /* sFlag     */
			, sStTime                            /* sStTime   */
			, sEdTime                            /* sEdTime   */
			, "                         "        /* sFiller   */
		);

		/*
		 * 송신한다. 송신데이터는 sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int SendBatFile(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret;

	if (flag)
	{
		if (flag)
		{
			if (FLAG_DANGBAL)
			{
				/*
				 * Login (당사발생)
				 */
				ret = SendBatLogin(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
			else
			{
				/*
				 * Login (타사발생)
				 */
				ret = RecvBatLogin(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
		}

		while (TRUE)
		{
			if (flag)
			{
				if (flag) printf("STATUS : ReadBatFQ()..\n");

				/*
				 * FQ에서 송신자료를 읽는다.
				 * 송신자료가 없으면 *sDataLine = 0x00
				 */
				if (ReadBatFQ() < 0)
				{
					if (flag) break;
					
					/*sleep(5 * 60); */
					sleep(10);
					continue;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : SendBatFileInfo()..\n");

				/*
				 * 파일정보
				 */
				ret = SendBatFileInfo(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : SendBatFileData()..\n");

				/*
				 * DATA송신
				 */
				ret = SendBatFileData(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : RecvBatCheck()..\n");

				/*
				 * 수신확인
				 */
				ret = RecvBatCheck(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag) break;
			
			sleep(30);
		}

		if (flag)
		{
			if (FLAG_DANGBAL)
			{
				/*
				 * Logout (당사발생)
				 */
				ret = SendBatLogout(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
			else
			{
				/*
				 * Logout (타사발생)
				 */
				ret = RecvBatLogout(cSock);
				if (ret < 0)
				{
					return ret;
				}
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

			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SENDER_ID"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SENDER_PWD"));

			strcpy(sUserid, CfgGetBxa("SENDER_ID" ));
			strcpy(sPasswd, GetPassword("KFT51", 1, "HCMSTEST", CfgGetBxa("SENDER_PWD"), "269" ));

			printf("CfgGetBxa    [%s]\n", sPasswd);
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

	if (FLAG_DANGBAL)
	{
		/*
		 * 프로그램이 클라이언트인 경우
		 */
		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (flag && CSockGetFdCount('C') == 0)
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

			if (!flag)
			{
				/*
				 * FQ에서 송신자료를 읽는다.
				 * 송신자료가 없으면 *sDataLine = 0x00
				 */
				if (ReadBatFQ() < 0)
				{
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
				if (CSockWriteRead(SendBatFile) < 0)
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
}

#endif
