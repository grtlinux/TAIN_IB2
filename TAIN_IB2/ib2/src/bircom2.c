/*
 * PROGRAM ID   : bircom2.c
 * PROGRAM NAME : bircom2 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : bircom2 모듈
 * HISTORY      : 2013.08.01 강석. 최초작성
 *
 * FEP 배치파일 수신
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
 *    typedef struct _FEP_HDR_50_
 *    {
 *        char    sLength     [ 4];  // 전문길이    : 전문길이 Field를 제외한 나머지 Length
 *        char    sBsnCode    [ 6];  // 업무식별코드: KBBS01
 *        char    sTrCode     [ 4];  // TR-CODE     : LINK,LIOK,DATA,DAOK,POLL,POOK,RJCT 등
 *        char    sResCode    [ 2];  // 응답코드    : '00'5. 장애응답코드표 참조
 *        char    sSeq        [ 8];  // 전문일련번호: '99999999' ? 전문SEQ번호
 *        char    sDataCnt    [ 2];  // Data건수    : 01 ~ (2 KByte 이내)
 *        char    sDateTime   [20];  // 시각        : YYYYMMDDHHMMSSssssss
 *        char    sAckYn      [ 1];  // ACK구분     : Data에 대한 ACK : 사용(Y), 미사용(N)
 *        char    sFiller     [ 3];
 *    } FEP_HDR;
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
 *
 *    typedef struct _FEP_BAT_INF_
 *    {
 *        FEP_HDR         stFep;  // FEP_HEADER (50)
 *        BAT_INF         stBat;  // BATCH_INFO (100)
 *    } FEP_BAT_INF;
 *
 *    typedef struct _FEP_BAT_DAT_
 *    {
 *        FEP_HDR         stFep;  // FEP_HEADER (50)
 *
 *        char    sData    [ 1];  // variable length...
 *    } FEP_DAT;
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: 최초작업"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.10.09.001: 최초작업"

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

#define       MAX_DATA_LINE        (10*1024)

char          sBatLine             [MAX_DATA_LINE];
int           iBatLen              ;
BAT_INF*      pBat                 = (BAT_INF*) &sBatLine;

char          sDataLine            [MAX_DATA_LINE];   /* 내부 처리용 자료 */
char          sSendLine            [MAX_DATA_LINE];   /* 송신용 자료 */
char          sRecvLine            [MAX_DATA_LINE];   /* 수신용 자료 */

int           iDataLen             ;
int           iSendLen             ;
int           iRecvLen             ;

FEP_BAT_INF*  pFepBatInf           = (FEP_BAT_INF*) &sDataLine;
FEP_BAT_INF*  pSndFepBatInf        = (FEP_BAT_INF*) &sSendLine;
FEP_BAT_INF*  pRcvFepBatInf        = (FEP_BAT_INF*) &sRecvLine;

FEP_DAT*      pFepDat              = (FEP_DAT*) &sDataLine;
FEP_DAT*      pSndFepDat           = (FEP_DAT*) &sSendLine;
FEP_DAT*      pRcvFepDat           = (FEP_DAT*) &sRecvLine;

/*----------------------------------------------------------------------------*/

int           iForkFlag            ;      /* fork 처리 flag */
int           iProcessAlive = 0    ;
int           iProcessDiff  = 0    ;

char          sGUID           [100];      /* Global User ID */

/*----------------------------------------------------------------------------*/

char          sFileName       [128];
int           iRecLen              ;
int           iRecCnt              ;

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
void PrintBatInf()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatLine == 0x00)
			return;

		printf("\t ---- BAT_INF* pBat --------------------------------\n");
		printf("\t [%s]\n", sBatLine);
		printf("\t pBat->sRecLen    = [%-*.*s]\n", sizeof(pBat->sRecLen   ), sizeof(pBat->sRecLen   ), pBat->sRecLen   );
		printf("\t pBat->sRecCnt    = [%-*.*s]\n", sizeof(pBat->sRecCnt   ), sizeof(pBat->sRecCnt   ), pBat->sRecCnt   );
		printf("\t pBat->sSR        = [%-*.*s]\n", sizeof(pBat->sSR       ), sizeof(pBat->sSR       ), pBat->sSR       );
		printf("\t pBat->sOrgCode   = [%-*.*s]\n", sizeof(pBat->sOrgCode  ), sizeof(pBat->sOrgCode  ), pBat->sOrgCode  );
		printf("\t pBat->sTrCode    = [%-*.*s]\n", sizeof(pBat->sTrCode   ), sizeof(pBat->sTrCode   ), pBat->sTrCode   );
		printf("\t pBat->sTrCodeLen = [%-*.*s]\n", sizeof(pBat->sTrCodeLen), sizeof(pBat->sTrCodeLen), pBat->sTrCodeLen);
		printf("\t pBat->sDate      = [%-*.*s]\n", sizeof(pBat->sDate     ), sizeof(pBat->sDate     ), pBat->sDate     );
		printf("\t pBat->sTime      = [%-*.*s]\n", sizeof(pBat->sTime     ), sizeof(pBat->sTime     ), pBat->sTime     );
		printf("\t pBat->sTrCodeSeq = [%-*.*s]\n", sizeof(pBat->sTrCodeSeq), sizeof(pBat->sTrCodeSeq), pBat->sTrCodeSeq);
		printf("\t pBat->sFiller    = [%-*.*s]\n", sizeof(pBat->sFiller   ), sizeof(pBat->sFiller   ), pBat->sFiller   );
		printf("\t ---------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * 내용 : sDataLine 자료를 송신한다.
 */
/*----------------------------------------------------------------------------*/
int SendData(int sock, char* sTrCode)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * 송신길이를 구한다.
			 */
			iSendLen = sizeof(FEP_HDR) + iDataLen;

			/*
			 * 송신전문을 세팅한다.
			 */
			sprintf(sSendLine, "%04d%-6.6s%-4.4s%02d%08d%02d%-20.20s%1.1s%-3.3s%s"
				, iSendLen - 4
				, sFepid
				, sTrCode
				, 0
				, SeqGetNo(sFepid)  /* TODO : 일별생성 전문일련번호 */
				, 1
				, TimeGetDateTime()
				, "N"
				, ""
				, sDataLine);

			if (!flag) LOG(_FL_, 0, 0, "[SendData]...[%s] ", sSendLine);
		}

		if (flag)
		{
			/*
			 * 송신한다.
			 */
			int ret = MSockMsgSend(sock, sSendLine, iSendLen);
			if (ret <= 0)
			{
				return -1;
			}

			if (flag)
			{
				LOG(_FL_, 0, 0, "SEND(%d): FEP_HDR+DATA [%*.*s][%d:%s]"
					, sock, sizeof(FEP_HDR), sizeof(FEP_HDR), (char*)&pSndFepDat->stFep
					, iDataLen, (char*)&pSndFepDat->sData);
			}
		}
	}

	return iSendLen;
}

/*============================================================================*/
/*
 * 내용 : 수신한 자료를 sDataLine 복사한다.
 */
/*----------------------------------------------------------------------------*/
int RecvData(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * 수신한다.
			 */
			iRecvLen = MSockMsgRecv(sock, sRecvLine);
			if (iRecvLen <= 0)
			{
				return -1;
			}

			sRecvLine[iRecvLen] = 0x00;

			if (!flag)
			{
				LOG(_FL_, 0, 0, "RECV(%d): FEP_HDR+DATA [%*.*s][%s]"
					, sock, sizeof(FEP_HDR), sizeof(FEP_HDR), (char*)&pRcvFepDat->stFep
					, (char*)&pRcvFepDat->sData);
			}
		}

		if (flag)
		{
			/*
			 * 수신받은 자료를 sDataLine에 복사한다.
			 */
			strcpy(sDataLine, pRcvFepDat->sData);
			iDataLen = strlen(sDataLine);

			/*
			 * 수신데이터를 출력한다.
			 */
			if (flag)
			{
				LOG(_FL_, 0, 0, "RECV(%d): FEP_HDR+DATA [%*.*s][%d:%s]"
					, sock, sizeof(FEP_HDR), sizeof(FEP_HDR), (char*)&pRcvFepDat->stFep
					, iDataLen, (char*)&pRcvFepDat->sData);
			}
		}
	}

	return iRecvLen;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileBABG(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		*sBatLine = 0x00;
		iBatLen = 0;
		
		/*
		 * 수신한다. 수신데이터는 sDataLine
		 */
		if (RecvData(sock) < 0)
		{
			return -1;
		}

		strcpy(sBatLine, sDataLine);
		iBatLen = strlen(sBatLine);

		/*
		 * 송신한다. sDataLine -> BAT_INF
		 */
		*sDataLine = 0x00;
		iDataLen = 0;
		
		if (SendData(sock, TC_BABG) < 0)
		{
			return -1;
		}
	}

	return iBatLen;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileDATA(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * 파일명을 만든다.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.bircom")
				, sizeof(pBat->sSR       ), sizeof(pBat->sSR       ), pBat->sSR
				, sizeof(pBat->sOrgCode  ), sizeof(pBat->sOrgCode  ), pBat->sOrgCode
				, sizeof(pBat->sTrCode   ), sizeof(pBat->sTrCode   ), pBat->sTrCode
				, sizeof(pBat->sDate     ), sizeof(pBat->sDate     ), pBat->sDate
				, sizeof(pBat->sTrCodeSeq), sizeof(pBat->sTrCodeSeq), pBat->sTrCodeSeq
				);
			UtilSetYYYYMMDD(sFileName);

			iRecLen = UtilToIntN(pBat->sRecLen, sizeof(pBat->sRecLen));
			iRecCnt = UtilToIntN(pBat->sRecCnt, sizeof(pBat->sRecCnt));

			/* 배치파일 정보를 출력 */
			if (flag) LOG(_FL_, 0, 0, "INF(%d): BAT_FILE [RecLen=%d][RecCnt=%d][%s]"
				, sock, iRecLen, iRecCnt, sFileName);
		}

		if (flag)
		{
			/*
			 * 배치파일 관련 폴더를 만든다.
			 */
			
			char sPath[128];
			
			strcpy(sPath, sFileName);
			
			char* p = rindex(sPath, '/');
			*p = 0x00;
			
			mkdir(sPath, 0777);
		}
		
		if (flag)
		{
			/*
			 * 배치파일을 반복적으로 쓴다.
			 */

			FILE* fp = fopen(sFileName, "w+");
			/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
			chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);

			int i;
			for (i=1; i <= iRecCnt ; i++)
			{
				if (flag)
				{
					/*
					 * 수신한다. 수신데이터는 sDataLine
					 */
					if (RecvData(sock) == FALSE)
					{
						return -1;
					}
				}

				if (flag)
				{
					/*
					 * sDataLine 을 구하고
					 * 수신배치파일에 쓴다.
					 */
					if (!flag) printf("%05d) DATA [%d:%s]\n", i, iDataLen, sDataLine);
					fprintf(fp, "%s\n", sDataLine);
				}

				if (flag)
				{
					/*
					 * 송신한다. 송신데이터는 없다.
					 */
					*sDataLine = 0x00;
					iDataLen = 0;
					
					if (SendData(sock, TC_DAOK) == FALSE)
					{
						return -1;
					}
				}
			}

			fclose(fp);
		}
	}
	
	return iRecCnt;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int RecvBatFileBAED(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 수신한다. 수신데이터는 sDataLine 사이즈 0
		 */
		if (RecvData(sock) < 0)
		{
			return -1;
		}

		/*
		 * 송신한다. sDataLine 사이즈 0
		 */
		*sDataLine = 0x00;
		iDataLen = 0;
		
		if (SendData(sock, TC_BAED) < 0)
		{
			return -1;
		}
	}

	return 0;
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
		if (*sBatLine == 0x00)
			return;

		FQ2* pFq = FQ2Open("OAS", sServiceid);

		FQ2Write(pFq, sBatLine);

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

/*============================================================================*/
/*
 * 내용 : 배치파일 수신
 *     1. 개시전문을 수신한다.
 *     2. 배치파일정보를 확인한다.
 *     3. 배치파일을 수신한다.
 *     4. 마감전문을 수신한다.
 *     5. FQ에서 배치파일정보를 쓴다.
 */
/*----------------------------------------------------------------------------*/
int RecvBatFile(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "[RecvBatFile]...[sock:%d]", csi->sock);

	if (flag)
	{
		int ret;

		if (flag)
		{
			/*
			 * BABG 처리
			 */
			ret = RecvBatFileBABG(csi->sock);
			if (ret < 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * DATA 반복처리
			 */
			ret = RecvBatFileDATA(csi->sock);
			if (ret < 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * BAED 처리
			 */
			ret = RecvBatFileBAED(csi->sock);
			if (ret < 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * FQ에서 송신자료를 읽는다.
			 * 송신자료가 없으면 *sDataLine = 0x00
			 */
			ret = WriteBatFQ();
			if (ret <= 0)
			{
				return ret;
			}
		}
	}

	return 0;
}


/*============================================================================*/
/*
 * 내용 : /hw03/ibridge/DAT2/YYYYMMDD/PS_ALIVE 파일에 ALIVE를 기록한다.
 */
/*----------------------------------------------------------------------------*/
int CheckAlive()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "JUN........");

	if (flag)
	{
		/* 프로세스 기동 초기화 */
		if (iProcessAlive == 0 ) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
		}
		
		iProcessDiff = time(NULL) - iProcessAlive;
		
		/* 1분마다 Alive 기록 */
		if (iProcessDiff >= 60) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
		}
	}
}


/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int MainService(char* strInfo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 0, 0, "[MainService]...strInfo[%s]", strInfo);
		
    SigSetSignal();
    
    if (flag) CheckAlive();

	if (flag)
	{
		if (flag)
		{
			FQ2Init("OAS", sServiceid);
		}

		if (flag)
		{
			/* 소켓정보를 파싱하여 정리한다. */
			MSockParsingSockInfo(strInfo);
		}

		while (TRUE)
		{
			/* 프로세스 동작체크 */
			if (flag) CheckAlive();
				
			/* server / accept / connect 소켓을 구성하고 소켓을 생성한다. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect 소켓을 체크한다. TimeOut, Poll 체크 */
			if (flag) MSockCheckHandler(NULL);

			/* 소켓의 send 처리를 다루는 handler */
			if (flag) MSockSendHandler(NULL);

			/* 소켓의 recv 처리를 다루는 handler */
			if (flag) MSockRecvHandler(RecvBatFile);

			if (flag) sleep(1);
		}
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
		sprintf(sSpace, "%1.1s", sServiceid + 1);

		if (flag) printf("\x1b[32m< %s %s %s (%s) >\x1b[0m\n"
			, sProgName, sServiceid, sFepid, sSpace);
		if (flag) printf("Program : [%s], Version : %s\n"
			, sProgName, VERSION);
		if (flag) printf("\t -------------------------------------------\n");
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

		if (flag)
		{
			printf("\t CfgGetMaster [%s]\n", CfgGetMaster("ib.version"));
			printf("\t CfgGetMaster [%s]\n", CfgGetMaster("ib.serialkey"));
			printf("\t CfgGetMaster [%s]\n", CfgGetMaster("ib.author"));
			printf("\t CfgGetMaster [%s]\n", CfgGetMaster("ib.company"));

			CfgOir2();

			iLLen = 4;
			iLPls = 0;
			printf("\t iLLen = [%d]\n", iLLen);
			printf("\t iLPls = [%d]\n", iLPls);
			printf("\t -------------------------------------------\n");

			printf("\n프로그램을 시작합니다.\n");
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
		 * 표준출력을 제거한다.
		 */
		if (!flag) freopen("/dev/null", "w", stdout);

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
			
			if (flag) LOG(_FL_, 0, 0, "[%s][strFile:%s]", TimeGetDateTime3(), strFile);

			fflush(stdout);
		}
		
		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOir2("SVR_INFO"));  /* 접속정보 */
}

#endif
