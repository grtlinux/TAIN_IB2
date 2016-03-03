/*
 * PROGRAM ID   : bxr.c
 * PROGRAM NAME : bxr ���
 * AUTHOR       : (��)ICA
 * COMMENT      : bxr ���
 * HISTORY      : 2013.08.04 ����. �����ۼ�
 *
 * ����ī�� ��ġ���� ����
 *
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.11.001: �����۾�."

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
 * EDI ��ġ ���� ��� : ����ī��
 *
 *    typedef struct _MSG_HDR_SHC_  // (27)
 *    {
 *        char    sMsgLen   [   4];  // TCP/IP �����۽� Byte��
 *        char    sBizKnd   [   3];  // ���������ڵ� 'EFT'
 *        char    sLength   [   4];  // ����SIZE Byte�� = sMsgLen
 *        char    sOrgzCd   [   2];  // ȸ�����ڵ�. 02(�����ڵ�)
 *        char    sMsgCls   [   4];  // ���������ڵ�
 *        char    sTrxCls   [   1];  // �ŷ������ڵ� 'S'����۽�, 'R'ī���۽�
 *        char    sSrFlag   [   1];  // �����߻����.�ۼ���Flag 'C'��� 'B'ī���
 *        char    sFileNm   [   8];  // ���ϱ����ڵ�  0600/001, 0600/004, SPACE
 *        char    sRespCd   [   3];  // �����ڵ�
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

MSG_MANG*       pMang           ;                  /* 0600, 0610 ����  : MSG_MANG */
MSG_FILE*       pFile           ;                  /* 0630, 0640 ����  : MSG_FILE */
MSG_DATA*       pData           ;                  /* 0310, 0320 ����  : MSG_DATA */
MSG_MREQ*       pMreq           ;                  /* 0620 ����        : MSG_MREQ */
MSG_MRES*       pMres           ;                  /* 0300 ����        : MSG_MRES */

/*----------------------------------------------------------------------------*/

int             iForkFlag       ;

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

char            sUserid   [20+1];                  /* Userid          */
char            sPasswd   [16+1];                  /* Passwd          */

char            sFileId   [20+1];                  /* �ۼ��� ���ϸ�   */

FILE*           fp              ;                  /* ��ġ���� fp     */
long            lPos            ;                  /* ��ġ���� ������ */

int             iRecLen         ;                  /* ���ڵ��Ѷ����� ����  */
int             iRecCnt         ;                  /* ���ڵ��� ����        */

int             iTrCodeSeq      ;                  /* TrCode Sequence : 01 ~ 99 */

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
 * ���� : ������ �ڷḦ sDataLine �����Ѵ�.
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
			, ""
			, "");

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
/*
 * ���� : �������� (���/Ÿ��)   0600/001 -> 0610/001
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
		*sFileId = 0x00;

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
 * ���� : �������� 0630 -> 0640
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
int RecvBatFileFILE(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			/*
			 * �����ڷḦ �����.
			 */
			strcpy(sFileInf, sDataLine);
			pFile = (MSG_FILE*) &sFileInf;

			sprintf(sDataLine, "%4.4s%4.4s%012d%4.4s"
				, pFile->sFileId
				, pFile->sCdDate
				, 0
				, pFile->sMsgByte);

			/*
			 * ����κп� ���ϸ��� ���̱� ���� �۾�.
			 */
			sprintf(sFileId, "%4.4s%4.4s", pFile->sFileId, pFile->sCdDate);
		}

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0640") == FALSE)
		{
			return -1;
		}

		/*
		 *  TODO DATE.2013.08.06 : BAT_INF�� �����Ͽ� ������ �״´�.
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
		 */
		if (flag)
		{
			/*
			 * ������ ���� ��ġ ����
			 */

			char* sSR         = "R"           ;    /* S �۽�, R ����              */
			char* sOrgCode    = "LTC"         ;    /* ����ڵ�                    */
			char  sTrCode[20]                 ;    /* TrCode                      */
			char  sDateTime[50]               ;    /* ��¥ : YYYYMMDDhhmmssSSSSSS */
			char  sFileName[124]              ;    /* ��ġ���ϸ�                  */

			char buf[20];
			sprintf(buf, "%4.4s", pFile->sFileId);
			UtilFillCopy(sTrCode, buf, 11, '0');

			strcpy(sDateTime, TimeGetDateTime());

			/*
			 * ��ġ�������� sSR, sTrCode�� bxa_trlst.cfg ���� ������ �д´�.
			 */
			CfgBxaTr(sSR, sTrCode);

			/*
			 * ���ڵ��� ���̸� ���Ѵ�.
			 */
			iRecLen = UtilToInt(CfgGetBxaTr("RECLEN"));
			iRecCnt = 0;

			if (flag)
			{
				/*
				 * �۽ſ� ��ġ������ �����.
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
				 * ��ġ������ �����Ѵ�.
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
 * ���� : DATA�۽� (BLOCK)   0320
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
int RecvBatFileDATA(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �����ڷᰡ DATA�� �ƴϸ� �����Ѵ�.
		 */

		if (flag)
		{
			/*
			 * sDataLine �� ���ϰ�
			 * ���Ź�ġ���Ͽ� ����.
			 * ���� Msg�� record ���̷� �ɰ��� ���Ͽ� ����Ѵ�.
			 */
			int iMsgLen;
			int iCnt;

			pData = (MSG_DATA*) &sDataLine;    /* 0310, 0320 ����  : MSG_DATA */

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
 * ���� : ���Ȯ��   0620 -> 0300
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
 *        char    sMisCnt   [   3];  // ��� ����
 *        char    sMisBit   [ 100];  // ��� DATA
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
		 * ����ڷḦ Ȯ���Ͽ� ���ó���� �Ѵ�.
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
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
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
 * ���� : ��� DATA�۽�    0310
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
int RecvBatFileMDAT(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �����ڷᰡ DATA�� �ƴϸ� �����Ѵ�.
		 */

	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : �̾����    0600/002 -> 0610/002
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
int RecvBatFileCONT(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �������� ��ġ���� ó���ÿ� CONT����
		 * ��ġ������ Ŭ�ο����Ѵ�.
		 */
		if (flag)
		{
			fclose(fp);
		}

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0610") == FALSE)
		{
			return -1;
		}

		/*
		 * ��ġ���� ������ �����Ѵ�.
		 */
		if (flag)
		{
			char* sSR         = "R"           ;    /* S �۽�, R ����              */
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
 * ���� : ��������      0600/003 -> 0610/003
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
int RecvBatFileFEND(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * ���� ��ġ���� ó�� �Ŀ�
		 * ��ġ������ Ŭ�ο����Ѵ�.
		 */
		if (flag)
		{
			fclose(fp);
		}

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendDataEDI(cSock, "0610") == FALSE)
		{
			return -1;
		}

		/*
		 * ��ġ���� ������ �����Ѵ�.
		 */
		if (flag)
		{
			char* sSR         = "R"           ;    /* S �۽�, R ����              */
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
 * ���� : ��������      0600/004 -> 0610/004
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
int RecvBatFileDONE(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		*sFileId = 0x00;

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
/*============================================================================*/
/*
 * ���� : initialize socket information
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
			 * �������� (���߻�)     0600/001 ->  <- 0610/001
			 */
			if (SendBatFileOPEN(cSock) < 0)
			{
				return -1;
			}
		}

		while (!flagDone)
		{
			/*
			 * �����Ѵ�. ���ŵ����ʹ� sDataLine
			 */
			*sDataLine = 0x00;
			if (RecvDataEDI(cSock) == FALSE)
			{
				return -1;
			}

			/*
			 * ���ŵ����Ϳ� ���� ó���� �Ѵ�.
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
 * ���� : �������� ���� �����Ѵ�.
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
		 * ���α׷��� Ŭ���̾�Ʈ�� ���
		 */

		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (CSockGetFdCount('C') == 0)
			{
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
				/*
				 * ��ġ������ �޴´�.
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
				 * FQ�� �����ڷḦ ����.
				 * �� �Ŀ��� *sDataLine = 0x00
				 * �� �Լ��� RecvBatFileEDI���� ó���ϵ��� ��.
				 * �׷��� ������� �ʴ´�.
				 */
				WriteBatFQ();
			}

			if (flag)
			{
				/*
				 * polling ó���� �Ѵ�. poll���� �۽Ÿ�.
				 */
			}

			if (flag) sleep(60);
		}
	}
	
	/******************************************************************************/

    if (flag) LOG(_FL_, 0, 0, "���α׷��� �����մϴ�.");
    	
    SigSetSignal();
	
	if (flag)
	{
		/*
		 * ���α׷��� ������ ���
		 */
		SSockFDZERO();
		
		SSockSocket(UtilToInt(CfgGetBxa("SND_PORT")));

		if (flag) SSockPrintSocketTable();

		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "Ŭ���̾�Ʈ ���� ���");
				/*
				 * Ŭ���̾�Ʈ�� ������ ó���Ѵ�.
				 */
				 
				SSockAccept();
				
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "��ġ ����");
				/*
				 * ��ġ������ �޴´�.
				 */
				if (SSockWriteRead(RecvBatFileEDI) < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "sleep 60");
			        /* ������ �����Ѵ�. */
                    if (flag) sleep(60);
                    if (flag) continue;
                    if (!flag) break;
				}
			}

			if (!flag)
			{
				/*
				 * FQ�� �����ڷḦ ����.
				 * �� �Ŀ��� *sDataLine = 0x00
				 */
				WriteBatFQ();
			}

			if (flag) sleep(60);
            if (!flag) break;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "���α׷��� �����մϴ�.");
}

#endif
