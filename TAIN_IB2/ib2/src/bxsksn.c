/*
 * PROGRAM ID   : bxs.c
 * PROGRAM NAME : bxs ���
 * AUTHOR       : (��)ICA
 * COMMENT      : bxs ���
 * HISTORY      : 2013.08.04 ����. �����ۼ�
 *
 * KSN ��ġ���� �۽�
 *
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.11.001: �����۾�"

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
 * EDI ��ġ ���� ��� : KS-NET   KSN
 *
 *    typedef struct _MSG_HDR_KSN_  // (23)
 *    {
 *        char    sMsgLen   [   4];  // TCP/IP �����۽� Byte��
 *        char    sBizKnd   [   3];  // ���������ڵ� 'KTB'
 *        char    sOrgzCd   [   3];  // ��������ڵ� '269'
 *        char    sMsgCls   [   4];  // ���������ڵ�
 *        char    sTrxCls   [   1];  // �ŷ������ڵ� 'S', 'R'
 *        char    sSrFlag   [   1];  // �ۼ��� Flag C:Center, B:Bank
 *        char    sFileNm   [   8];  // ���ϱ����ڵ�  0600/001, 0600/004, SPACE
 *        char    sRespCd   [   3];  // �����ڵ�
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

MSG_MANG*       pMang           ;                  /* 0600, 0610 ����  : MSG_MANG  */
MSG_FILE*       pFile           ;                  /* 0630, 0640 ����  : MSG_FILE  */
MSG_DATA*       pData           ;                  /* 0310, 0320 ����  : MSG_DATA  */
MSG_MREQ*       pMreq           ;                  /* 0620 ����        : MSG_MREQ  */
MSG_MRES*       pMres           ;                  /* 0300 ����        : MSG_MRES  */

/*----------------------------------------------------------------------------*/

int             iForkFlag       ;

int             iRSeq           ;                  /* read sequence number */

char            sBatInf         [MAX_DATA_LINE];   /* FEP ��ġ���� from FQ */

char            sFileInf        [MAX_DATA_LINE];   /* ��ġ��������         */

char            sSendEdiLine    [MAX_DATA_LINE];   /* �۽����� */
int             iSendEdiLen     ;

char            sRecvEdiLine    [MAX_DATA_LINE];   /* �������� */
int             iRecvEdiLen     ;

char            sBatHdr         [MAX_DATA_LINE];   /* ��ġ������ ����� */

char            sDataLine       [MAX_DATA_LINE];   /* ��ġ������ �����ͺ� */
int             iDataLen        ;

/*----------------------------------------------------------------------------*/

char            sUserid   [20+1];                  /* Userid                    */
char            sPasswd   [16+1];                  /* Passwd                    */

char            sFileId   [20+1];                  /* �ۼ��� ���ϸ�             */

FILE*           fp              ;                  /* ��ġ���� fp               */
long            lPos            ;                  /* ��ġ���� ������           */

int             iMsgRecs        ;                  /* Message ���� Record ��    */

int             iFileIdx        ;
int             iFileSize       ;                  /* �۽��� ��ġ���� ����      */

int             iBlockIdx       ;
int             iBlockSize      ;                  /* �۽��� ��ġ������ �� �� */

int             iRecIdx         ;
int             iRecSize        ;                  /* �۽��� ��ġ������ ���� �� */

int             iMissSeq        ;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� : FQ�� ������ �о� �����ͺ��� BAT_INF ����ü�� ����Ѵ�.
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
 * ���� : FQ�� ������ �д´�. sBatInfo ���� �����Ѵ�. �׸��� BAT_INF ���.
 */
/*----------------------------------------------------------------------------*/
int ReadBatFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * FQ���� ��ġ������ �д´�.
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
		 * ��ġ�������� sSR, sTrCode�� bxa_trlst.cfg ���� ������ �д´�.
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
 * ���� : BAT_INF ����ü�� ����Ѵ�.
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
 * ���� : sDataLine �ڷḦ �۽��Ѵ�.
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
			 * �۽ű��̸� ���Ѵ�.
			 */
			iSendEdiLen = sizeof(MSG_HDR) + strlen(sDataLine);

			/*
			 * �۽������� �����Ѵ�.
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
			 * �۽��Ѵ�. :       MSG_HDR + sDataLine ->
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
 * ���� : ������ �ڷḦ sDataLine �����Ѵ�.
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
			 * �����Ѵ�. :   <- MSG_HDR + sDataLine
			 */

			if (flag)
			{
				/*
				 * �������� ���̺κ��� �д´�.
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
				 * �������� ���̺κ��� ������ �ڷ�κ��� �д´�.
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
			 * ���Ź��� �ڷḦ sDataLine�� �����Ѵ�.
			 */
			strcpy(sDataLine, pLine->sData);

			/*
			 * ���ŵ����͸� ����Ѵ�.
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
 * ���� : ��ġ���� ������ �����Ѵ�.
 *     ���� ��������(BAT_INF)�� �о� ó���� �Ѵ�.
 *     �ӽ� �׽�Ʈ�� ���� �Լ���.
 */
/*----------------------------------------------------------------------------*/
int GetBatchFileInfo( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �Ʒ� ���� ���� �ڷḦ �о ó���Ѵ�.
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
 * ���� : �������� (���/Ÿ��)   0600/001 -> 0610/001
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
 * ���� : �������� (���/Ÿ��)   0600/001 -> 0610/001
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
 * ���� : �����ڵ�
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
 * ���� : �������� (���/Ÿ��)   0600/001 -> 0610/001
 *     TODO DATE.2013.08.06 : ���/Ÿ�� ó�� �ʿ�....
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // ���������Ͻ� MMDDhhmmss
 *        char    sBizCls   [   3];  // ������������ 001(��������),002(�̾����),003(��������),004(��������)
 *        char    sSender   [  20];  // �۽��ڸ�   SPACE
 *        char    sPasswd   [  16];  // �۽��ھ�ȣ SPACE
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
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvDataEDI(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * OPEN : �������� ������ �����Ѵ�.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "001"
			, sUserid
			, sPasswd);

		if (!flag)
		{
			/*
			 * Password�� ��ȣȭ�Ѵ�.
			 */
			char sDecStr[20];
			strcpy(sDecStr, GetPassword("KFT51", 2, "HCMSTEST", sPasswd, "269" ));
			printf("OUTPUT ���ڵ�=[%s]\n", sDecStr );
		}

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
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
 * ���� : �������� (���/Ÿ��)   0600/001 -> 0610/001
 *     TODO DATE.2013.08.06 : ���/Ÿ�� ó�� �ʿ�....
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // ���������Ͻ� MMDDhhmmss
 *        char    sBizCls   [   3];  // ������������ 001(��������),002(�̾����),003(��������),004(��������)
 *        char    sSender   [  20];  // �۽��ڸ�   SPACE
 *        char    sPasswd   [  16];  // �۽��ھ�ȣ SPACE
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
		 * OPEN : �������� ������ �����Ѵ�.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "001"
			, sUserid
			, sPasswd);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0600") == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
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
 * ���� : �������� 0630 -> 0640
 *     TODO DATE.2013.08.06 : FileID�� ���� ���� �ʿ�
 *
 *    typedef struct _MSG_0630_LTC_  // header + (24)
 *    {
 *      char    sFileId   [   4];  // ���� NAME
 *      char    sCdDate   [   4];  // �ڷ����� MMDD
 *      char    sFileSz   [  12];  // ���� SIZE    0630�� '000000000000', 0640����� ���� ����
 *      char    sMsgByte  [   4];  // ���� BYTE ��
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
		 * ��ġ���Ͽ� ���� ó���� �Ѵ�.
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
				 * ���ϸ��� �����.
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

				/* ��ġ���� ������ ��� */
				if (flag) LOG(_FL_, 0, 0, "INF(%d): BAT_FILE [RecLen=%d][RecCnt=%d][%s]"
					, cSock, iRecLen, iRecCnt, sFileName);
			}

			if (flag)
			{
				/*
				 * ��ġ������ �����Ѵ�.
				 */
				fp = fopen(sFileName, "r+");

				/*
				 * ��ġ���� ó���� ���� �ʱⰪ�� �����Ѵ�.
				 */
				iRecSize = iRecCnt;       /* Header, Body, Tailer ��� ������ record �Ǽ� */

				if (flag)
				{
					iMsgRecs = MAX_MSG_DTL / iRecLen;  /* �Ѱ� Msg�� ���� �� �ִ� record �Ǽ� */
				}
				else
				{
					iMsgRecs = 1;                      /* �Ѱ� Msg�� �Ѱ��� record */
				}

				iFileSize  = 1;
				iBlockSize = (((iRecSize + iMsgRecs - 1) / iMsgRecs) + BLOCK_RECS - 1) / BLOCK_RECS;

				iFileIdx   = 0;
				iBlockIdx  = 0;
				iRecIdx    = 0;
			}

			if (flag)
			{
				/* ���� ���ϻ���� �ƴ� (���ڵ���� * �Ǽ�)  */

				fstat(fileno(fp), &fst);

				/*
				 *FILE : �������� ������ �����Ѵ�.
				 */
				sprintf(sDataLine, "%-8.8s%012d%04d"
					, sFileId
					, iRecLen * iRecCnt    /* �������� ���� */
					, iRecLen);            /* ���� Byte     */
			}

			if (!flag)
			{
				/* ���� ���ϻ���� �ƴ� (���ڵ���� * �Ǽ�)  */

				fstat(fileno(fp), &fst);

				/*
				 *FILE : �������� ������ �����Ѵ�.
				 */
				sprintf(sDataLine, "%4.4s%4.4s%012d%04d"
					, CfgGetBxaTr("FILEID")
					, TimeGetDate() + 4
					, fst.st_size        /* ���� ������ */
					, iRecLen);            /* ���� Byte   */

				/*
				 * �۽����ϸ��� �����.
				 */
				sprintf(sFileId, "%-4.4s%4.4s"
					, CfgGetBxaTr("FILEID")
					, TimeGetDate() + 4);
			}
		}

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0630") == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvDataEDI(cSock) == FALSE)
		{
			return -1;
		}

		if (flag)
		{
			/*
			 * DATE.2013.09.26
			 * ������ ������ �����ڵ带 Ȯ���Ѵ�.
			 */
			char sResCode[10];

			sprintf(sResCode, "%*.*s", sizeof(pLine->stHdr.sRespCd)
				, sizeof(pLine->stHdr.sRespCd), pLine->stHdr.sRespCd);

			if (strcmp("000", sResCode) != 0)
			{
				if (strcmp("090", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : �ý��� ��� = '%s'", sResCode);
				}
				else if (strcmp("310", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : �۽��ڸ� ���� = '%s'", sResCode);
				}
				else if (strcmp("320", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : �����ھ�ȣ ���� = '%s'", sResCode);
				}
				else if (strcmp("630", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : ������ �Ϸ� = '%s'", sResCode);
				}
				else if (strcmp("631", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : �̵�� ���� = '%s'", sResCode);
				}
				else if (strcmp("632", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : ������ ���ϸ� = '%s'", sResCode);
				}
				else if (strcmp("633", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : ������ ���� BYTE �� = '%s'", sResCode);
				}
				else if (strcmp("800", sResCode) == 0)
				{
					LOG(_FL_, 0, 0, "ERROR : FORMAT ���� = '%s'", sResCode);
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
 * ���� : DATA�۽� (BLOCK)   0320
 *     TODO DATE.2013.08.06 : ������ ���ڵ带 �о� ó��
 *
 *    typedef struct _MSG_0310_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sSlByte   [   4];  // ��DATA BYTE��
 *        char    sMsgDtl   [4058];  // ���� ����
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
				 * �������� record�� ���� Msg�� �����.
				 * �������� buf�� ���� str�� �����.
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
			 * DATA : ���� ���ڵ带 �����Ѵ�.
			 */
			sprintf(sDataLine, "%04d%03d%04d%s"
				, (iBlockIdx + 1)
				, (i + 1)
				, strlen(str)
				, str);

			/*
			 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
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
 * ���� : ���Ȯ��   0620 -> 0300
 *
 *    typedef struct _MSG_0620_LTC_  // header + (7)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO : ���ó���� BLOCK ��ȣ
 *        char    sSeqNo    [   3];  // SEQUENCE NO : ���ó���� BLOCK�� ���� ���� DATA�� SEQ-NO
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
		 * FILE : �������� ������ �����Ѵ�.
		 */
		sprintf(sDataLine, "%04d%03d"
			, (iBlockIdx + 1)
			, iMissSeq);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0620") == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
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
 * ���� : ��� DATA�۽�    0310
 *
 *    typedef struct _MSG_0300_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sMisCnt   [   3];  // ��� ����
 *        char    sMisBit   [ 100];  // ��� DATA
 *    } MSG_LTC_0300;
 *
 *    typedef struct _MSG_0310_LTC_  // header + (110)
 *    {
 *        char    sBlockNo  [   4];  // BLOCK NO
 *        char    sSeqNo    [   3];  // SEQUENCE NO
 *        char    sSlByte   [   4];  // ��DATA BYTE��
 *        char    sMsgDtl   [4058];  // ���� ����
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
		 * ����ڷ�
		 */
		char sMissDat[1024];
		MSG_MRES* pMres = (MSG_MRES*) &sMissDat;

		strcpy(sMissDat, sDataLine);

		/*
		 * ������ ��ġ�� �̵��Ѵ�.
		 */
		long lPosOld = ftell(fp);
		fseek(fp, lPos, SEEK_SET);

		static char str[MAX_DATA_LINE];

		/*
		 * �� ���� ���۹�ȣ�� ���Ѵ�.
		 */
		int idx = ((iRecIdx - 1) / BLOCK_RECS) * BLOCK_RECS;
		if (!flag) LOG(_FL_, 0, 0, "[SendBatFileMDAT]...[idx=%d][iRecIdx=%d][iRecSize=%d] ", idx, iRecIdx, iRecSize);

		int i;
		for (i=0; i < BLOCK_RECS && pMres->sMisBit[i] && idx < iRecSize; i++, idx++)
		{
			/*
			 * ��ġ���� �ڷḦ �д´�.
			 */
			if (fgets(str, MAX_DATA_LINE, fp))
			{
				str[strlen(str) - 1] = 0x00;     /* '\n'�� �����Ѵ�. */
			}
			else
			{
				str[0] = 0x00;     /* �ڷᰡ ����. */
			}

			/*
			 * �÷��� : 0.���, 1.����
			 */
			if (pMres->sMisBit[i] == '1')
			{
				continue;
			}

			/*
			 * DATA : ���� ���ڵ带 �����Ѵ�.
			 */
			sprintf(sDataLine, "%04d%03d%04d%s"
				, (iBlockIdx + 1)
				, (i + 1)
				, strlen(str)
				, str);

			/*
			 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
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
 * ���� : �̾����    0600/002 -> 0610/002
 *     ���� ������ �ִ� ���, �̾����� ó���Ѵ�.
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // ���������Ͻ� MMDDhhmmss
 *        char    sBizCls   [   3];  // ������������ 001(��������),002(�̾����),003(��������),004(��������)
 *        char    sSender   [  20];  // �۽��ڸ�   SPACE
 *        char    sPasswd   [  16];  // �۽��ھ�ȣ SPACE
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
		 * ��ġ������ Ŭ�ο����Ѵ�.
		 */
		if (flag)
		{
			fclose(fp);
		}

		/*
		 * CONT : �̾���� ������ �����Ѵ�.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "002"
			, sUserid
			, sPasswd);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0600") == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
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
 * ���� : ��������      0600/003 -> 0610/003
 *     ���� �۽��� ��ħ..
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // ���������Ͻ� MMDDhhmmss
 *        char    sBizCls   [   3];  // ������������ 001(��������),002(�̾����),003(��������),004(��������)
 *        char    sSender   [  20];  // �۽��ڸ�   SPACE
 *        char    sPasswd   [  16];  // �۽��ھ�ȣ SPACE
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
		 * FEND : �������� ������ �����Ѵ�.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "003"
			, sUserid
			, sPasswd);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0600") == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
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
 * ���� : ��������      0600/004 -> 0610/004
 *    ���þ����� ������.
 *
 *    typedef struct _MSG_0600_LTC_  // header + (49)
 *    {
 *        char    sSendDt   [  10];  // ���������Ͻ� MMDDhhmmss
 *        char    sBizCls   [   3];  // ������������ 001(��������),002(�̾����),003(��������),004(��������)
 *        char    sSender   [  20];  // �۽��ڸ�   SPACE
 *        char    sPasswd   [  16];  // �۽��ھ�ȣ SPACE
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
		 * DONE : �������� ������ �����Ѵ�.
		 */
		sprintf(sDataLine, "%10.10s%3.3s%-20.20s%-16.16s"
			, TimeGetDateTime() + 4
			, "004"
			, sUserid
			, sPasswd);

		*sFileId = 0x00;

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0600") == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
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
 * ���� : initialize socket information
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
		 * ��ġ���� ������ �����Ѵ�.
		 */
		/* GetBatchFileInfo(); */

		if (flag)
		{
			if (flag)
			{
				/*
				 * �������� (���߻�)     0600/001 ->  <- 0610/001
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
				 * �������� (Ÿ��߻�)     0600/001 ->  <- 0610/001
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
			 * ��������          0630 ->  <- 0640
			 */
			ret = SendBatFileFILE(cSock);
			if (ret < 0)
			{
				return ret;
			}

			while (TRUE)
			{
				/*
				 * DATA�۽� (BLOCK)            0320 =>
				 */
				ret = SendBatFileDATA(cSock);
				if (ret < 0)
				{
					return ret;
				}

				while (TRUE)
				{
					/*
					 * ���Ȯ��         0620 ->  <- 0300
					 */
					ret = SendBatFileMISS(cSock);
					if (ret < 0)
					{
						return ret;
					}

					/*
					 * ����ڷ� �ִ��� Ȯ��.
					 */
					MSG_MRES* pMres = (MSG_MRES*) &sDataLine;
					if (UtilToIntN(pMres->sMisCnt, sizeof(pMres->sMisCnt)) != 0)
					{
						/*
						 * ��� DATA�۽�         0310 =>
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
				 * ���� ���� ������ �ٽ� DATA �۽�
				 */
				if (iBlockIdx < iBlockSize)
				{
					continue;
				}

				break;
			}

			iFileIdx ++;

			/*
			 * ���� ������ ������ �ٽ� �������� �۽�
			 */
			if (iFileIdx < iFileSize)
			{
				/*
				 * �̾����      0600/002 ->  <- 0610/002
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
			 * ��������         0600/003 ->  <- 0610/003
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
		 * ��������              0600/004 ->  <- 0610/004
		 */
		ret = SendBatFileDONE(cSock);
		if (ret < 0)
		{
			return ret;
		}
	}

	if (flag)
	{
		/* ������ �����Ѵ�. */
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

		if (flag) printf("\x1b[44m< %s %s %s >\x1b[0m\n", sProgName, sServiceid, sFepid);
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
			printf("OUTPUT ���ڵ���ȣ=[%s]\n", sEnc );

			/*
			 *  ��ȣȭ
			 */
			strcpy(sDec, GetPassword("KFT51", 2, "HCMSTEST", sEnc, "243" ));
			printf("OUTPUT ���ڵ���ȣ=[%s]\n", sDec );
		}
	}

	if (!flag && iForkFlag)
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
			if (!flag) sprintf(strFile, "%s/%s", strPath, sFepid);   /* ERROR : �� ���μ��� ���� */
			if (flag) LOG(_FL_, 0, 0, "LOG FILE => [%s]\n", strFile);

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

			if (flag) LOG(_FL_, 0, 0, "[%s]\n", strFile);
			if (flag) LOG(_FL_, 0, 0, "[%s]\n", TimeGetDateTime2());
			fflush(stdout);
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

    if (flag) LOG(_FL_, 0, 0, "���α׷��� �����մϴ�.");
    	
    SigSetSignal();

	if (flag)
	{
		/*
		 * ���α׷��� Ŭ���̾�Ʈ�� ���
		 */
		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (CSockGetFdCount('C') == 0)
			{
				if (flag) LOG(_FL_, 0, 0, "������ ���� �õ�");
				/*
				 * ������ ������ �õ��Ѵ�.
				 */

				CSockFDZERO();
				if (CSockSocket(CfgGetBxa("SND_HOST"), UtilToInt(CfgGetBxa("SND_PORT")), 1) == FALSE)
				{
					sleep(RECONN_SEC);
					continue;
				}

				if (flag) CSockPrintSocketTable();

				/*
				 * link ó���� �Ѵ�. link���� �۽�
				 */
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "FQ���� �۽��ڷ� ����");
				/*
				 * FQ���� �۽��ڷḦ �д´�.
				 * �۽��ڷᰡ ������ *sDataLine = 0x00
				 */
				if (ReadBatFQ() < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "FQ�ڷ� ���� -> ���� ����");
			        /* ������ �����Ѵ�. */
			        CSockFDCLR_All();
                    if (!flag) sleep(60);
                    if (!flag) continue;
                    if (flag) break;
				}
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "��ġ���� �۽�");
				/*
				 * ��ġ������ ������.
				 */
				if (CSockWriteRead(SendBatFileEDI) < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "ERROR....");
					break;
				}
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "POLL���� �۽�");
				/*
				 * polling ó���� �Ѵ�. poll���� �۽Ÿ�.
				 */
				CSockFDCLR_All();
			}

            if (!flag) sleep(60);
            if (flag) break;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "���α׷��� �����մϴ�.");
}

#endif
