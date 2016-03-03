/*
 * PROGRAM ID   : biscom2.c
 * PROGRAM NAME : biscom2 ���
 * AUTHOR       : (��)ICA
 * COMMENT      : biscom2 ���
 * HISTORY      : 2013.10.09 ����. �����ۼ�
 *
 * FEP ��ġ���� �۽�
 *
 *  1. ��ġFQ�� �д´�.
 *      BAT_INF(100) ������ �д´�.
 *
 *    typedef struct _FEP_HDR_50_
 *    {
 *        char    sLength     [ 4];  // ��������    : �������� Field�� ������ ������ Length
 *        char    sBsnCode    [ 6];  // �����ĺ��ڵ�: KBBS01
 *        char    sTrCode     [ 4];  // TR-CODE     : LINK,LIOK,DATA,DAOK,POLL,POOK,RJCT ��
 *        char    sResCode    [ 2];  // �����ڵ�    : '00'5. ��������ڵ�ǥ ����
 *        char    sSeq        [ 8];  // �����Ϸù�ȣ: '99999999' ? ����SEQ��ȣ
 *        char    sDataCnt    [ 2];  // Data�Ǽ�    : 01 ~ (2 KByte �̳�)
 *        char    sDateTime   [20];  // �ð�        : YYYYMMDDHHMMSSssssss
 *        char    sAckYn      [ 1];  // ACK����     : Data�� ���� ACK : ���(Y), �̻��(N)
 *        char    sFiller     [ 3];
 *    } FEP_HDR;
 *
 *    typedef struct _BAT_INF_
 *    {
 *        char    sRecLen     [ 4]; // ���ڵ� ����
 *        char    sRecCnt     [ 7]; // ���ڵ� �Ǽ�
 *        char    sSR         [ 1]; // �ۼ��� ����
 *        char    sOrgCode    [ 3]; // ����ڵ�
 *        char    sTrCode     [11]; // �ϰ����� TR Code
 *        char    sTrCodeLen  [ 2]; // �ϰ����� TR Code ����
 *        char    sDate       [ 8]; // ������������ YYYYMMDD
 *        char    sTime       [ 9]; // �������۽ð� hhmmssSSS
 *        char    sTrCodeSeq  [ 2]; // ���ں� TR Code ȸ��
 *        char    sFiller     [53]; // ����
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
 *
 *  2. ��ġ�۽� ����
 *      �۽�:BABG : FEP_HDR(50:BABG) + BAT_INF(100)    ->
 *      ����:BABG : FEP_HDR(50:BABG)                   <-
 *
 *  3. ��ġ���� �۽�(�ݺ� ���ڵ�۽�)
 *      �۽�:DATA : FEP_HDR(50:DATA) + sDataLine(����)  ->
 *      ����:DAOK : FEP_HDR(50:DAOK)                    <-
 *
 *  4. ��ġ�۽� ����
 *      �۽�:BAED : FEP_HDR(50:BAED)    ->
 *      ����:BAED : FEP_HDR(50:BAED)    <-
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.28.001: �����۾�"

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

#define       MAX_DATA_LINE        (10*1024)

char          sBatLine             [MAX_DATA_LINE];
int           iBatLen              ;
BAT_INF*      pBat                 = (BAT_INF*) &sBatLine;

char          sDataLine            [MAX_DATA_LINE];   /* ���� ó���� �ڷ� */
char          sSendLine            [MAX_DATA_LINE];   /* �۽ſ� �ڷ� */
char          sRecvLine            [MAX_DATA_LINE];   /* ���ſ� �ڷ� */

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

int           iForkFlag            ;      /* fork ó�� flag */
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
 * ���� : BAT_INF ����ü�� ����Ѵ�.
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
#if 0
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
#else
        if (flag) LOG(_FL_, 0, 0, " ---- BAT_INF* pBat --------------------------------");
		if (flag) LOG(_FL_, 0, 0, " [%s]", sBatLine);
		if (flag) LOG(_FL_, 0, 0, " pBat->sRecLen    = [%-*.*s]", sizeof(pBat->sRecLen   ), sizeof(pBat->sRecLen   ), pBat->sRecLen   );
		if (flag) LOG(_FL_, 0, 0, " pBat->sRecCnt    = [%-*.*s]", sizeof(pBat->sRecCnt   ), sizeof(pBat->sRecCnt   ), pBat->sRecCnt   );
		if (flag) LOG(_FL_, 0, 0, " pBat->sSR        = [%-*.*s]", sizeof(pBat->sSR       ), sizeof(pBat->sSR       ), pBat->sSR       );
		if (flag) LOG(_FL_, 0, 0, " pBat->sOrgCode   = [%-*.*s]", sizeof(pBat->sOrgCode  ), sizeof(pBat->sOrgCode  ), pBat->sOrgCode  );
		if (flag) LOG(_FL_, 0, 0, " pBat->sTrCode    = [%-*.*s]", sizeof(pBat->sTrCode   ), sizeof(pBat->sTrCode   ), pBat->sTrCode   );
		if (flag) LOG(_FL_, 0, 0, " pBat->sTrCodeLen = [%-*.*s]", sizeof(pBat->sTrCodeLen), sizeof(pBat->sTrCodeLen), pBat->sTrCodeLen);
		if (flag) LOG(_FL_, 0, 0, " pBat->sDate      = [%-*.*s]", sizeof(pBat->sDate     ), sizeof(pBat->sDate     ), pBat->sDate     );
		if (flag) LOG(_FL_, 0, 0, " pBat->sTime      = [%-*.*s]", sizeof(pBat->sTime     ), sizeof(pBat->sTime     ), pBat->sTime     );
		if (flag) LOG(_FL_, 0, 0, " pBat->sTrCodeSeq = [%-*.*s]", sizeof(pBat->sTrCodeSeq), sizeof(pBat->sTrCodeSeq), pBat->sTrCodeSeq);
		if (flag) LOG(_FL_, 0, 0, " pBat->sFiller    = [%-*.*s]", sizeof(pBat->sFiller   ), sizeof(pBat->sFiller   ), pBat->sFiller   );
		if (flag) LOG(_FL_, 0, 0, " ---------------------------------------------------");

#endif
	}
}

/*============================================================================*/
/*
 * ���� : sDataLine �ڷḦ �۽��Ѵ�.
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
			 * �۽ű��̸� ���Ѵ�.
			 */
			iSendLen = sizeof(FEP_HDR) + iDataLen;

			/*
			 * �۽������� �����Ѵ�.
			 */
			sprintf(sSendLine, "%04d%-6.6s%-4.4s%02d%08d%02d%-20.20s%1.1s%-3.3s%s"
				, iSendLen - 4
				, sFepid
				, sTrCode
				, 0
				, SeqGetNo(sFepid)  /* TODO : �Ϻ����� �����Ϸù�ȣ */
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
			 * �۽��Ѵ�.
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
 * ���� : ������ �ڷḦ sDataLine �����Ѵ�.
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
			 * �����Ѵ�.
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
			 * ���Ź��� �ڷḦ sDataLine�� �����Ѵ�.
			 */
			strcpy(sDataLine, pRcvFepDat->sData);
			iDataLen = strlen(sDataLine);

			/*
			 * ���ŵ����͸� ����Ѵ�.
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
 * ���� : �������� ���� �����Ѵ�.
 * DATE.2013.09.27 : AP#2 ������ FQ (FEPID02)������ ���� �۾�
 *                   FQOpen(SEND_FQ, sServiceid);
 *
 * char          sBatLine        [MAX_DATA_LINE];
 * int           iBatLen         ;
 * BIZ_DAT*      pBat            = (BIZ_DAT*) &sBatLine;
 *
 */
/*----------------------------------------------------------------------------*/
int ReadBatFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FQ2* pFq = FQ2Open("OAR", sServiceid);

		char* p = FQ2Read(pFq);
		if (p == NULL)
		{
			FQ2Close(pFq);
			return 0;
		}
		strcpy(sBatLine, p);
		iBatLen = strlen(sBatLine);

		FQ2Close(pFq);
	}

	if (flag) PrintBatInf();

	return iBatLen;
}

/*============================================================================*/
/*
 * ���� : ��ġ���ϰ� ��ġ������ ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int ValidCheck()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * ���ϸ��� �����.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.biscom")
				, sizeof(pBat->sSR       ), sizeof(pBat->sSR       ), pBat->sSR
				, sizeof(pBat->sOrgCode  ), sizeof(pBat->sOrgCode  ), pBat->sOrgCode
				, sizeof(pBat->sTrCode   ), sizeof(pBat->sTrCode   ), pBat->sTrCode
				, sizeof(pBat->sDate     ), sizeof(pBat->sDate     ), pBat->sDate
				, sizeof(pBat->sTrCodeSeq), sizeof(pBat->sTrCodeSeq), pBat->sTrCodeSeq
				);
			UtilSetYYYYMMDD(sFileName);

			iRecLen = UtilToIntN(pBat->sRecLen, sizeof(pBat->sRecLen));
			iRecCnt = UtilToIntN(pBat->sRecCnt, sizeof(pBat->sRecCnt));

			if (flag) LOG(_FL_, 0, 0, "[iRecLen=%d] [iRecCnt=%d] [sFileName:%s]"
				, iRecLen, iRecCnt, sFileName);
		}

		if (flag)
		{
			/*
			 * ������ �����ϴ��� Ȯ���Ѵ�.
			 */

			if (access(sFileName, R_OK) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : (��ġ)������ �������� �ʽ��ϴ�.[%s]", sFileName);
				return -1;
			}
		}

		if (flag)
		{
			/*
			 * ������ ũ�⸦ Ȯ���Ѵ�.
			 * ���ڵ��� ���̴� '\n' (new line) ������ ���̷� �Ѵ�.
			 */

			int iFileSize;
			struct stat fStat;

			iFileSize = (iRecLen + 1) * iRecCnt;

			if (lstat(sFileName, &fStat) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : (��ġ)���� ���¸� ���� �� �����ϴ�.[%s]", sFileName);
				return -1;
			}

			if (iFileSize != fStat.st_size)
			{
				LOG(_FL_, 0, 0, "ERROR : (��ġ)���� ũ��� (REC x CNT)�� �ٸ��ϴ�.[%s]", sFileName);
				return -1;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ��������
 */
/*----------------------------------------------------------------------------*/
int SendBatFileBABG(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �۽��Ѵ�. sDataLine -> BAT_INF
		 */
		strcpy(sDataLine, sBatLine);
		iDataLen = iBatLen;

		if (SendData(sock, TC_BABG) < 0)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(sock) < 0)
		{
			return -1;
		}
	}

	return iDataLen;
}

/*============================================================================*/
/*
 * ���� : ��ġ����
 */
/*----------------------------------------------------------------------------*/
int SendBatFileDATA(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * ��ġ������ �ݺ������� �д´�.
			 */

			FILE* fp = fopen(sFileName, "r+");

			int i;
			for (i=1; i <= iRecCnt ; i++)
			{
				if (flag)
				{
					/*
					 * ���ڵ� �Ǽ���ŭ �ݺ��Ͽ� ó���Ѵ�.
					 * BatFile -> sDataLine
					 */
					if (fgets(sDataLine, MAX_DATA_LINE, fp))
					{
						iDataLen = strlen(sDataLine) - 1;
						sDataLine[iDataLen] = 0x00;     /* '\n'�� �����Ѵ�. */
					}
					else
					{
						iDataLen = 0;
						sDataLine[0] = 0x00;     /* �ڷᰡ ����. */
					}

					if (!flag) printf("%05d) DATA [%s]\n", i, sDataLine);
				}

				if (flag)
				{
					/*
					 * �۽��Ѵ�. sDataLine
					 */
					if (SendData(sock, TC_DATA) < 0)
					{
						return -1;
					}

					/*
					 * �����Ѵ�. ���ŵ����ʹ� ����
					 */
					if (RecvData(sock) < 0)
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
 * ���� : ��������
 */
/*----------------------------------------------------------------------------*/
int SendBatFileBAED(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �۽��Ѵ�. sDataLine ������ 0
		 */
		*sDataLine = 0x00;
		iDataLen = 0;

		if (SendData(sock, TC_BAED) < 0)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine ������ 0
		 */
		if (RecvData(sock) < 0)
		{
			return -1;
		}
	}

	return iDataLen;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� : ��ġ���� �۽�
 *     1. FQ���� ��ġ���������� �д´�.
 *     2. ��ġ���������� Ȯ���Ѵ�.
 *     3. ���������� �۽��Ѵ�.
 *     4. ��ġ������ �۽��Ѵ�.
 *     5. ���������� �۽��Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int SendBatFile(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "[SendBatFile]...[sock:%d]", csi->sock);

	if (flag)
	{
		int ret;

		if (flag)
		{
			/*
			 * FQ���� �۽��ڷḦ �д´�.
			 * �۽��ڷᰡ ������ *sDataLine = 0x00
			 */
			ret = ReadBatFQ();
			if (ret <= 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * Validation Check :
			 * ��ġ���ϰ� ��ġ������ ���Ѵ�.
			 */
			ret = ValidCheck();
			if (ret < 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * BABG ó��
			 */
			ret = SendBatFileBABG(csi->sock);
			if (ret < 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * DATA �ݺ�ó��
			 */
			ret = SendBatFileDATA(csi->sock);
			if (ret < 0)
			{
				return ret;
			}
		}

		if (flag)
		{
			/*
			 * BAED ó��
			 */
			ret = SendBatFileBAED(csi->sock);
			if (ret < 0)
			{
				return ret;
			}
		}
	}

	return 0;
}


/*============================================================================*/
/*
 * ���� : /hw03/ibridge/DAT2/YYYYMMDD/PS_ALIVE ���Ͽ� ALIVE�� ����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int CheckAlive()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "JUN........");

	if (flag)
	{
		/* ���μ��� �⵿ �ʱ�ȭ */
		if (iProcessAlive == 0 ) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
		}
		
		iProcessDiff = time(NULL) - iProcessAlive;
		
		/* 1�и��� Alive ��� */
		if (iProcessDiff >= 60) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
		}
	}
}
/*============================================================================*/
/*
 * ���� : initialize socket information
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
			FQ2Init("OAR", sServiceid);
		}

		if (flag)
		{
			/* ���������� �Ľ��Ͽ� �����Ѵ�. */
			MSockParsingSockInfo(strInfo);
		}

		while (TRUE)
		{
			/* ���μ��� ����üũ */
			if (flag) CheckAlive();
				
			/* server / accept / connect ������ �����ϰ� ������ �����Ѵ�. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect ������ üũ�Ѵ�. TimeOut, Poll üũ */
			if (flag) MSockCheckHandler(NULL);

			/* ������ send ó���� �ٷ�� handler */
			if (flag) MSockSendHandler(SendBatFile);

			/* ������ recv ó���� �ٷ�� handler */
			if (flag) MSockRecvHandler(NULL);

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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �⺻ ó��
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
		 * ���μ������� ����ϴ� ���񽺸�� FEPID�� ��´�.
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
		 * master ȯ�������� �д´�. biz.properties ���ϵ� �д´�.
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

			CfgOis2();

			iLLen = 4;
			iLPls = 0;
			printf("\t iLLen = [%d]\n", iLLen);
			printf("\t iLPls = [%d]\n", iLPls);
			printf("\t -------------------------------------------\n");

			printf("\n���α׷��� �����մϴ�.\n");
		}
	}

	if (flag && iForkFlag)
	{
		/*
		 * fork�� �̿��Ͽ� child process �� �����Ѵ�.
		 */
		setpgrp();
		switch(fork())
		{
			case -1: printf("ERROR : fork error : [errno=%d]\n", errno); exit(9);
			case  0: break;    /* child process */
			default: exit(0);  /* parent process */
		}

		/*
		 * ǥ������� �����Ѵ�.
		 */
		if (!flag) freopen("/dev/null", "w", stdout);

		/*
		 * ǥ������� LOGó���Ѵ�.
		 */
		if (flag)
		{
			char strPath[128];
			char strFile[128];

			sprintf(strPath, CfgGetMst("ib.path.log"));
			UtilSetYYYYMMDD(strPath);
			mkdir(strPath, 0777);

			if (flag) sprintf(strFile, "%s/%s", strPath, sServiceid);
			if (flag) LOG(_FL_, 0, 0, "LOG FILE => [%s]", strFile);

			if (access(strFile, R_OK) < 0)
			{
				/* ���ʻ��� */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND ��� */
				freopen(strFile, "a+", stdout);
			}

			if (flag) LOG(_FL_, 0, 0, "[%s][strFile:%s]", TimeGetDateTime3(), strFile);

			fflush(stdout);
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOis2("SVR_INFO"));  /* �������� */
}

#endif