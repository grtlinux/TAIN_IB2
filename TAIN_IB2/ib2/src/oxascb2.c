/*
 * PROGRAM ID   : oxascb2.c
 * PROGRAM NAME : oxascb2 관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : oxascb2 관련 모듈
 * HISTORY      : 2013.11.19 강석. 최초작성
 *
 */

#include <Common.h>

#include <xmdevice.h>
#include <xmclient.h>
#include <xmdefine.h>
#include <xmdebug.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.11.19.001: 최초작업"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.11.19.001: 최초작업"

/*============================================================================*/
/*
 * global variables
 *
 *  typedef struct _FEP_HDR_50_
 *  {
 *      char    sLength     [ 4];  // 전문길이    : 전문길이 Field를 제외한 나머지 Length
 *      char    sBsnCode    [ 6];  // 업무식별코드: KBBS01
 *      char    sTrCode     [ 4];  // TR-CODE     : LINK,LIOK,DATA,DAOK,POLL,POOK,RJCT 등
 *      char    sResCode    [ 2];  // 응답코드    : '00'5. 장애응답코드표 참조
 *      char    sSeq        [ 8];  // 전문일련번호: '99999999' ? 전문SEQ번호
 *      char    sDataCnt    [ 2];  // Data건수    : 01 ~ (2 KByte 이내)
 *      char    sDateTime   [20];  // 시각        : YYYYMMDDHHMMSSssssss
 *      char    sAckYn      [ 1];  // ACK구분     : Data에 대한 ACK : 사용(Y), 미사용(N)
 *      char    sFiller     [ 3];
 *  } FEP_HDR;
 *
 *  typedef struct _BIZ_HDR_100_
 *  {
 *      char    sDataLength [ 4];  // 본 Length FIELD를 제외한 개별 DATA SIZE
 *      char    sDataSeq    [ 8];  // 해당 Data의 일련번호
 *      char    sGlobalId   [32];  // GUID
 *      char    sDateTime   [20];  // 시각        : YYYYMMDDHHMMSSssssss
 *      char    sRespCode   [ 4];
 *      char    sMediaTp    [ 2];
 *      char    sMediaSeq   [ 1];
 *      char    sFiller     [29];
 *  } BIZ_HDR;
 *
 *  typedef struct _BIZ_DAT_
 *  {
 *      BIZ_HDR         stBiz;  // BIZ_HEADER (100)
 *
 *      char    sData    [ 1];  // variable length.
 *  } BIZ_DAT;
 *
 *  typedef struct _FEP_BIZ_DAT_
 *  {
 *      FEP_HDR         stFep;  // FEP_HEADER (50)
 *      BIZ_HDR         stBiz;  // BIZ_HEADER (100)
 *
 *      char    sData    [ 1];  // variable length... 대외기관 송수신자료
 *  } FEP_BIZ_DAT;
 *
 *  * SC은행 (504 bytes) - len (4) + header (57) + data (443)
 *  typedef struct _SCB_DATA_
 *  {
 *      char sMsgLen       [  4];  // 전문길이
 *      char sTrCode       [  9];  // transaction code
 *      char sSystemType   [  3];  // "STK"
 *      char sServiceType  [  3];  // "DDD":대행업무, "GGG":결제업무
 *      char sCompanyCode  [ 10];  // 업체번호:증권사번호(7)+SPACE(3)
 *      char sSeqNo        [  7];  // 전문일련번호
 *      char sMsgType      [  4];  // 전문구분코드
 *      char sJobCode      [  3];  // 업무구분코드
 *      char sSrFlag       [  1];  // 송수신구분 (은행:B 증권:E)
 *      char sReplyCode    [  3];  // 응답코드
 *      char sTransDateTime[ 14];  // 송수신일시 (YYYYMMDDhhmmss)
 *      char sData         [443];  // data
 *  } SCB_DATA;
 *
 */
/*----------------------------------------------------------------------------*/
/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define            MAX_DATA_LINE   (10*1024)

#define            SEND_FQ         "OAS"           /* 송신을 위한 FQ */
#define            RECV_FQ         "OAR"           /* 수신을 위한 FQ */

#define            KEY__           3
#define            MSG__           4

/*----------------------------------------------------------------------------*/

char               sDataLine       [MAX_DATA_LINE];   /* 처리용 자료 */
char               sSendLine       [MAX_DATA_LINE];   /* 송신용 자료 */
char               sRecvLine       [MAX_DATA_LINE];   /* 수신용 자료 */

int                iDataLen        ;
int                iSendLen        ;
int                iRecvLen        ;

BIZ_DAT*           pBizDat         = (BIZ_DAT*) &sDataLine;

FEP_BIZ_DAT*       pSndFepBizDat   = (FEP_BIZ_DAT*) &sSendLine;
FEP_BIZ_DAT*       pRcvFepBizDat   = (FEP_BIZ_DAT*) &sRecvLine;

SCB_DATA*          pSndScbLine     = (SCB_DATA*) &sSendLine;
SCB_DATA*          pRcvScbLine     = (SCB_DATA*) &sRecvLine;

/*----------------------------------------------------------------------------*/

int                iForkFlag       ;           /* fork 처리 flag */
int                iProcessAlive = 0    ;
int                iProcessDiff  = 0    ;

char               sGUID           [100];      /* Global User ID */

int                iInitFlag       ;
XM_CLIENT_CTX      ctx;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int ReadFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "INFO : start ReadFQ().");

	if (flag)
	{
		*sDataLine = 0x00;

		if (flag)
		{
			/*
			 * FQ를 읽는다.
			FQ* pFq = FQOpen(SEND_FQ, sFepid);
			 */
			FQ2* pFq = FQ2Open(SEND_FQ, sServiceid);

			char* p = FQ2Read(pFq);

			FQ2Close(pFq);

			/*
			 * 읽은 자료를 sDataLine에 복사한다.
			 */
			if (p == NULL)
			{
				*sDataLine = 0x00;
				return 0;
			}
			strcpy(sDataLine, p);
			iDataLen = strlen(sDataLine);
		}
		
		if (flag)
		{
			/* DATE.2014.05.19 : 중복 체크 */
			static char sOldDataLine[MAX_DATA_LINE] = { 0x00, };
			
			if (strcmp(sOldDataLine, sDataLine) == 0)
			{
				/* 중복 */
				if (flag) LOG(_FL_, 8, 0, "\x1b[41m[WARNING!!]\x1b[0m [FQ_READ](중복)=[%04d:%s] ", iDataLen, sDataLine);

#if 0
				strcpy(sOldDataLine, sDataLine);
#else
				*sDataLine = 0x00;
				return 0;
#endif
			}
			else
			{
				if (flag) LOG(_FL_, 5, 0, "[FQ_READ]  -> [%04d:%s] ", iDataLen, sDataLine);
				strcpy(sOldDataLine, sDataLine);
			}
		}
	}

	return iDataLen;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int WriteFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "INFO : start WriteFQ().");

	if (flag)
	{
		if (*sDataLine == 0x00)
		{
			return 0;
		}

		if (flag)
		{
			/*
			 * 자료를 FQ에 기록한다.
			FQ* pFq = FQOpen(RECV_FQ, sFepid);
			 */
			FQ2* pFq = FQ2Open(RECV_FQ, sServiceid);

			FQ2Write(pFq, sDataLine);

			FQ2Close(pFq);
		}
	}

	if (flag) LOG(_FL_, 0, 0, "[%s] [%04d:%s] ", sFepid, iDataLen, sDataLine);

	return iDataLen;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SEND(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	int nleft, nwritten, err;
	char *buf;

	buf = (char*) msg;
	nleft = len;

	while (nleft > 0) {
		nwritten = send(sock, buf, nleft, 0);
		if (nwritten < 0) {
			/* unix */
			if (errno == EINTR)
				nwritten = 0;
			else
				return -2;
		}

		nleft -= nwritten;
		buf += nwritten;
	}

	return len;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int RECV(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	int nleft, nread, err;
	char* buf;

	buf = (char*) msg;
	nleft = len;

	while (nleft > 0) {
		nread = recv(sock, buf, nleft, 0);
		if (nread < 0) {
			/* unix */
			if (errno == EINTR)
				nread = 0;
			else if (errno == EWOULDBLOCK)
				return -3;
			else
				return -4;
		}
		else if (nread == 0)
			break;

		nleft -= nread;
		buf += nread;
	}

	return (len - nleft);
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SocketMsgSend(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	int ret, pos = 0;
	unsigned char buf[1024*10];

	memcpy(buf, msg, len);

	ret = SEND(sock, buf, len);
	if (ret < 0)
		return (-1);

	return ret;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SocketMsgRecv_BAK(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	int ret, len;

	ret = RECV(sock, msg, 4);
	if (ret < 0)
		return -1;

	len = UtilToIntN(msg, 4);

	ret = RECV(sock, msg + 4, len);
	if (ret < 0)
		return -3;

	msg[len + 4] = 0x00;
	
	return (len + 4);
}

/*============================================================================*/
/*
 * 내용 : 암호화
 */
/*----------------------------------------------------------------------------*/
int XM2_Encrypt()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 암호화
		 */

		int ret;
		uint8   out[2048];
		uint16  outLen;
		uint8   outSend[8192];
		uint16  outSendLen;

		strcpy(out, sSendLine);
		outLen = iSendLen;

		if (!flag)
		{
			SCB_DATA* p = (SCB_DATA*)&out;
			printf("\t ENC.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t ENC.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t ENC.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t ENC.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t ENC.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t ENC.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t ENC.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t ENC.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t ENC.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t ENC.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t ENC.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t ENC.sData          = [%*.*s]\n", sizeof(p->sData         ), sizeof(p->sData         ), p->sData         );
		}

		if (flag)
		{
			/*
			 * sData 부분을 암호화 한다.
			 */
			SCB_DATA* p = (SCB_DATA*)&out;

			ret = XMC2_Encrypt_Message(&ctx, outSend, &outSendLen, sizeof(outSend), p->sData, sizeof(p->sData));
			if (ret > 0)
			{
				if (flag) printf("[%s] SUCCESS : XMC2_Encrypt_Message (MSG) OK....[inLen=%d]\n", TimeGetTime2(), outLen);
			}
			else
			{
				if (flag) printf("[%s] FAIL : XMC2_Encrypt_Message (MSG) Fail [ret=%d]....\n", TimeGetTime2(), ret);
				return -1;
			}
		}

		if (flag)
		{
			/*
			 * outSend(암호화된 전문)을 생성한다. 암호화전문은 binary 코드이다.
			 */
			iSendLen = 4 + 57 + outSendLen;

			sprintf(sSendLine, "%04d%57.57s", iSendLen - 4, out + 4);

			memcpy(pSndScbLine->sData, outSend, outSendLen);
		}

		if (!flag)
		{
			SCB_DATA* p = pSndScbLine;
			printf("\t SEND.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t SEND.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t SEND.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t SEND.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t SEND.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t SEND.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t SEND.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t SEND.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t SEND.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t SEND.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t SEND.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t SEND.sData          = [암호화문자열]\n");
		}
	}

	return 0;
}


/*============================================================================*/
/*
 * 내용 : 복호화
 */
/*----------------------------------------------------------------------------*/
int XM2_Decrypt()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 복호화
		 */

		int ret;
		uint8   in[2048];
		uint16  inLen;
		uint8   inRecv[8192];
		uint16  inRecvLen;

		memcpy(inRecv, sRecvLine, iRecvLen);
		inRecvLen = iRecvLen;
		
		if (!flag)
		{
			SCB_DATA* p = (SCB_DATA*)&inRecv;
			printf("\t RECV.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t RECV.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t RECV.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t RECV.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t RECV.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t RECV.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t RECV.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t RECV.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t RECV.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t RECV.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t RECV.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t RECV.sData          = [암호화문자열]\n");
		}

		if (flag)
		{
			/*
			 * inRecv.sData(암호화된 전문)을 in(복호화된 전문)으로 만든다.
			 */
			SCB_DATA* p = (SCB_DATA*)&inRecv;
			uint16 pLen = inRecvLen - 4 - 57;

			ret = XMC2_Decrypt_Message(&ctx, in, &inLen, sizeof(in), (uint8*)&p->sData, pLen);
			if (ret > 0)
			{
				if (flag) printf("[%s] SUCCESS : XMC2_Decrypt_Message (MSG) OK....[inLen=%d]\n", TimeGetTime2(), inLen);
			}
			else
			{
				if (flag) printf("[%s] FAIL : XMC2_Decrypt_Message (MSG) Fail [ret=%d]....\n", TimeGetTime2(), ret);
				return -4;
			}
		}

		if (flag)
		{
			/*
			 * 복호화 전문을 만든다.
			 */
			iRecvLen = 4 + 57 + inLen;

			sprintf(sRecvLine, "%04d%57.57s%-443.443s", iRecvLen - 4, inRecv + 4, in);
		}

		if (!flag)
		{
			SCB_DATA* p = pRcvScbLine;
			printf("\t DEC.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t DEC.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t DEC.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t DEC.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t DEC.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t DEC.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t DEC.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t DEC.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t DEC.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t DEC.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t DEC.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t DEC.sData          = [%*.*s]\n", sizeof(p->sData         ), sizeof(p->sData         ), p->sData         );
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int XM2_HandShake(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "[SEND] KEY 송신" );

	if (flag)
	{
		/*
		 * XecureConnect HandShake : KEY를 송신을 수행한다.
		 */

		int ret;
		uint8   out[2048];
		uint16  outLen;

		ret = XMC2_Encode_KeyInit_Token(&ctx, out, &outLen, sizeof(out));
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : XMC2_Encode_KeyInit_Token (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : XMC2_Encode_KeyInit_Token Fail (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -1;
		}

        if (flag) LOG(_FL_, 0, 0, "STATUS : (KEY) outLen = %d\n", outLen);
		fflush(stdout);

		iSendLen = 4 + 57 + outLen;

		sprintf(sSendLine, "%04d%9.9s%3.3s%3.3s%-10.10s%07d%4.4s%3.3s%1.1s%3.3s%-14.14s"
			, iSendLen - 4
			, ""
			, "STK"
			, ""
			, "0005789"
			, SeqGetNo(sFepid)  /* 전문일련번호 */
			, "0800"
			, "500"
			, "E"
			, ""
			, TimeGetDateTime());

		memcpy(pSndScbLine->sData, out, outLen);

		ret = SocketMsgSend(cSock, sSendLine, iSendLen);
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : SocketMsgSend (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : SocketMsgSend Fail (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -2;
		}

		if (flag)
		{
			SCB_DATA* p = pSndScbLine;
			printf("\t SEND.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t SEND.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t SEND.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t SEND.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t SEND.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t SEND.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t SEND.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t SEND.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t SEND.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t SEND.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t SEND.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			fflush(stdout);
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "[RECV] KEY 수신" );
	
	if (flag)
	{
		/*
		 * XecureConnect HandShake : KEY를 응답을 수행한다.
		 */

		int ret;
		uint8   in[2048];
		uint16  inLen;

		ret = MSockMsgRecv(cSock, sRecvLine);
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : MSockMsgRecv (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : MSockMsgRecv (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -3;
		}


		if (flag)
		{
			SCB_DATA* p = pRcvScbLine;
			printf("\t RECV.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t RECV.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t RECV.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t RECV.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t RECV.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t RECV.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t RECV.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t RECV.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t RECV.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t RECV.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t RECV.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			fflush(stdout);
		}

		iRecvLen = ret;

		inLen = iRecvLen - 4 - 57;
		memcpy(in, pRcvScbLine->sData, inLen);

		ret = XMC2_Decode_KeyFinal_Token(&ctx, in, inLen);
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : XMC2_Decode_KeyFinal_Token (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : XMC2_Decode_KeyFinal_Token Fail (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -4;
		}
	}
	
	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int XM2_Init(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "XecureConnect Lib Init..");
	
	if (flag)
	{
		/*
		 * XecureConnect의 Lib를 초기화 한다.
		 */

		int ret;
		char sFileName[200];
		strcpy(sFileName, "/hw01/ibridge/lib/XecureConnect2.6/test/test-cert.der");

		ret = XMC2_ClientLib_Init(&ctx, sFileName);
		if (ret > 0)
		{
			if (flag) printf("SUCCESS : XecureConnect Lib Init OK....\n");
			fflush(stdout);
		}
		else
		{
			if (flag) printf("FAIL : XecureConnect Lib Init Fail [ret=%d]....\n", ret);
			fflush(stdout);
			return -1;
		}
	}

    if (flag) LOG(_FL_, 0, 0, "XecureConnect HandShake..");
	
	if (flag)
	{
		/*
		 * XecureConnect HandShake : KEY 를 송신/응답한다.
		 */
		 
		int iret;
		
		iret = XM2_HandShake(cSock);
		
		if ( iret < 0 )
		{
			if (flag) LOG(_FL_, 0, 0, "XecureConnect HandShake FAIL...");
			return -1;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "SUCCESS : XecureConnect Init...");

	return 0;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : TimeOut과 PollSend 처리를 한다.
 */
/*----------------------------------------------------------------------------*/
int CheckProc(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "CheckProc [START]...");

	if (!flag)
	{
		/* 확인용 */
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (CheckProc) ");
		MSockPrintSockItem(sHdr, csi);
	}

	if (flag)
	{
		/*
		 * SCB01을 위한 기능
		 */

		if (iInitFlag == 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS INITIALIZE..");
			
			int ret = XM2_Init(csi->sock);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 0, 0, "XM2_Init FAIL..");
				if (flag) exit(-1);
			}
			
			iInitFlag = 1;
		}
		else
		{
			if (!flag) LOG(_FL_, 0, 0, "CONTINUE..........");
		}
	}

	if (!flag)
	{
		/*
		 * [차이] = [현재시간(초)] - [최근 수신한 시간(초)]
		 */
		int diff = time(NULL) - csi->lastRecved;

		if (!flag)
		{
			/*
			 * [ TIME_OUT : SERVER / CLIENT ]
			 *
			 * TimeOut 시간이 초과 되었는지 확인한다.
			 * extern int iTimeOutSec  ;
			 *   [차이] > TimeOut(초) 이면 close, 아니면 지나간다.
			if (diff > iTimeOutSec && csi->conntype == CONN_TYPE_ACCEPT)
			 */

			if (diff > iTimeOutSec)
			{
				if (flag) LOG(_FL_, 0, 0, 
					"[%s] [TIME_OUT_CLOSE:%d] SESSION CLOSE....(%s:%d)"
					, sFepid, iTimeOutSec, csi->ipaddr, csi->port);
				return -1;
			}
		}

		if (!flag)
		{
			/*
			 * [ POLL_SEND : CONNECT CLIENT ]
			 *
			 * PollSend 시간이 되었는지 확인한다.
			 * extern int iPollSendSec  ;
			 *   [차이] > iPollSendSec(초) 이면
			 *   Poll 전문을 발생하고 iPollFlag = TRUE로 한다.
			 *
			 * Poll 송신은 CONNECT한 놈만 보내는 것으로 한다..
			 */

			if (diff >= iPollSendSec
			&&  csi->conntype   == CONN_TYPE_CONNECT
			&&  csi->iPollFlag  == FALSE)
			{
				csi->iPollFlag = TRUE;

				if (flag)
				{
					/* POLL REQ 생성 */
					strcpy(sSendLine, PollMsgSendReq());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL REQ 송신 */
					int ret = MSockMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 0, 0, 
							"[%s] [POLL_SEND] SESSION CLOSE BY SEND ERROR..(%s:%d)"
							, sFepid, csi->ipaddr, csi->port);
						return -1;
					}
				}

				if (flag)
				{
					/* POLL REQ 출력 */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_SEND:%d] REQ -> [%d:%s] (%s:%d)"
						, sFepid, diff, iSendLen, sSendLine, csi->ipaddr, csi->port);
				}
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int RecvProc(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "RecvProc [START]...");

	if (!flag)
	{
		/* 확인용 */
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (RecvProc) ");
		MSockPrintSockItem(sHdr, csi);
	}

	if (flag)
	{
		if (flag)
		{
			/*
			 * 전문을 수신한다.
			 */

			iRecvLen = MSockMsgRecv(csi->sock, sRecvLine);
			if (iRecvLen < 0)
			{
				/* 수신 에러 */
				if (flag) LOG(_FL_, 0, 0, 
					"[%s] [MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* 수신길이 0 -> 미처리 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] Length Zero(0).(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return 0;
			}
			else
			{
				/* 정상 수신 */
				if (!flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] [%d:%s].(%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
		}

		if (flag)
		{
			/*
			 * 전문 확인한다.
			 *   1. POLL요청수신 -> POLL응답송신 -> POLL clear
			 *   2. POLL응답수신 -> POLL clear
			 *   3. 일반전문 수신 -> 수신처리
			 */

			if (PollCmpRecvReq(sRecvLine) == 0)
			{
				/*
				 * 1. POLL요청수신 -> POLL응답송신 -> POLL clear
				 */

				if (flag)
				{
					/* 수신 POLL REQ 출력 */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- REQ [%d:%s] (%s:%d)"
						, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
				}

				if (flag)
				{
					/* POLL RES 생성 */
					strcpy(sSendLine, PollMsgSendRes());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL RES 송신 */
					int ret = SocketMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 0, 0, 
							"[%s] [POLL_SEND] SESSION CLOSE BY SEND POLL-RES ERROR..(%s:%d)"
							, sFepid, csi->ipaddr, csi->port);
						return -1;
					}
				}

				if (flag)
				{
					/* 송신 POLL RES 출력 */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_SEND] RES -> [%d:%s] (%s:%d)"
						, sFepid, iSendLen, sSendLine, csi->ipaddr, csi->port);
				}
			}
			else if (PollCmpRecvRes(sRecvLine) == 0)
			{
				/*
				 * 2. POLL응답수신 -> POLL clear
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- RES [%d:%s] (%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
			else
			{
				if (flag)
				{
					/* 데이터 부분을 복호화처리를 한다. */
					XM2_Decrypt();
				}
				
				/*
				 * 3. 일반전문 수신 -> 수신처리( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] <- [%d:%s].(%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);

				if (flag)
				{
					/*
					 * GUID를 생성한다.
					 *
					 *  1. 발생주체(8)
					 *      거래가 처음 시작되는 곳에서 부여
					 *      통합단말과 HTS의 경우 MCI 핸들로 통일함
					 *      다른 채널 및 매매지원시스템의 경우
					 *      채널구분코드(3) 자유롭게 부여한 문자열(5)
					 *  2. 생성년월일시(20)
					 *      형식 : YYYYMMDDhhmmssuuuuuu
					 *      플랫폼에 따라 마이크로 단위초를 구하지 못할
					 *      경우에는 필리 단위로 표시함. 밀리 단위로
					 *      표시할 경우에는 마지막 3자리는 아래 일련번호 필드에 포함됨
					 *  3. 일련번호(4)
					 *      난수로 발생함. or 유일한 일련번호
					 *
					 */
					sprintf(sGUID, "%-8.8s%-20.20s%04d"
						, sServiceid
						, TimeGetDateTime()
						, SeqGetNo(sFepid) % 10000);
				}

				if (flag)
				{
					/*
					 * [ 대외기관 수신용 : oxr, oxa ]
					 * BIZ_HDR + data를 세팅한다.
					 */
					sprintf(sDataLine, "%04d%08d%-32.32s%-20.20s%04d%02d%01d%29.29s%s"
						, sizeof(BIZ_HDR) + iRecvLen - 4   /* sDataLength [ 4] */
						, 0                                /* sDataSeq    [ 8] */
						, sGUID                            /* sGlobalId   [32] */
						, TimeGetDateTime()                /* sDateTime   [20] */
						, 0                                /* sRespCode   [ 4] */
						, 1                                /* sMediaTp    [ 2] */
						, 1                                /* sMediaSeq   [ 1] */
						, ""                               /* sFiller     [29] */
						, sRecvLine);

					iDataLen = strlen(sDataLine);
				}

				if (!flag)
				{
					/* [ 내부 수신용 : oir, ofr ]
					 * FEP_HDR를 제거한다.
					 * BIZ_HDR + data를 구성한다.
					 */
					strcpy(sDataLine, (char*)&pRcvFepBizDat->stBiz);
					iDataLen = strlen(sDataLine);
				}

				if (flag)
				{
					/*
					 * FQ에 기록한다.
					 */
					WriteFQ();
				}
			}

			if (flag)
			{
				/*
				 * 클라이언트로부터 전문이 수신되었으므로
				 * lastRecved 시간을 현재시간으로 세팅한다.
				 * Poll 처리를 위해서 flag를 FALSE로 한다.
				 */
				csi->lastRecved = time(NULL);
				csi->iPollFlag = FALSE;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SendProc(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SendProc [START]...");

	if (!flag)
	{
		/* 확인용 */
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (SendProc) ");
		MSockPrintSockItem(sHdr, csi);
	}

	if (flag)
	{
		/*
		 * 송신전문을 만들어 송신한다.
		 *   1. 4-0 형식 테스트 : SHC01
		 *   2. 4-4 형식 테스트 : TAT01
		 */

		if (flag)
		{
			/*
			 * FQ에서 전문을 읽는다.
			 */

			*sDataLine = 0x00;

			ReadFQ();

			if (*sDataLine == 0x00)
			{
				return 0;
			}
		}

		if (flag)
		{
			/*
			 * [ 대외기관 송신용 : oxr, oxa ]
			 * FQ에서 읽은 자료에서 BIZ_HDR를 제거한다.
			 * DATA 만 전송한다.
			 */
			strcpy(sSendLine, (char*)&pBizDat->sData);
			iSendLen = strlen(sSendLine);
		}
		
		if (flag) LOG(_FL_, 0, 0, "[HDR_제거] -> [%04d:%s] ", iSendLen, sSendLine);

		if (!flag)
		{
			/* [ 내부 송신용 : oir, ofr ]
			 * FEP_HDR를 추가한다.
			 * FEP_HDR + BIZ_HDR + data 전송한다.
			 */
			iSendLen = sizeof(FEP_HDR) + strlen(sDataLine);

			sprintf(sSendLine, "%04d%-6.6s%-4.4s%02d%08d%02d%-20.20s%1.1s%-3.3s%s"
				, iSendLen - 4
				, sFepid
				, TC_DATA
				, 0
				, SeqGetNo(sFepid)  /* 전문일련번호 */
				, 1
				, TimeGetDateTime()
				, "N"
				, ""
				, sDataLine);
		}

		if (flag)
		{
			/* DATA 부분을 암호화 한다. */
			XM2_Encrypt();
		}

		if (flag)
		{
			int ret = SocketMsgSend(csi->sock, sSendLine, iSendLen);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 0, 0, 
					"[%s] [MSG_SEND] SESSION CLOSE BY SEND REQ ERROR..(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
		}

		if (flag)
		{
			/* 정상 송신 */
			if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_SEND] -> [%d:%s].(%s:%d)"
				, sFepid, iSendLen, sSendLine, csi->ipaddr, csi->port);
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
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int MainService(char* strInfo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 0, 0, "[MainService]...strInfo[%s]", strInfo);
		
	SigSetSignal();
    
    CheckAlive(); /* 프로세스 기동 초기화 */

	if (flag)
	{
		if (flag)
		{
			/* 사용할 FQ를 선택한다. */
			if (flag) FQ2Init(SEND_FQ, sServiceid);
			if (flag) FQ2Init(RECV_FQ, sServiceid);
		}

		if (flag)
		{
			/* 소켓정보를 파싱하여 정리한다. */
			MSockParsingSockInfo(strInfo);
		}

		while (TRUE)
		{
			CheckAlive(); /* 프로세스 기동 초기화 */
			
			/* server / accept / connect 소켓을 구성하고 소켓을 생성한다. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect 소켓을 체크한다. TimeOut, Poll 체크 */
			if (flag) MSockCheckHandler(CheckProc);

			/* 소켓의 send 처리를 다루는 handler */
			if (flag) MSockSendHandler(SendProc);

			/* 소켓의 recv 처리를 다루는 handler */
			if (flag) MSockRecvHandler(RecvProc);

			if (flag) sleep(5);
			if (flag) usleep(100 * 1000);
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "\x1b[41m-----%S ProcessExit -----\x1b[0m", sServiceid);

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
			printf("\t CfgGetMaster(ib.version  )  [%s]\n", CfgGetMaster("ib.version"  ));
			printf("\t CfgGetMaster(ib.serialkey)  [%s]\n", CfgGetMaster("ib.serialkey"));
			printf("\t CfgGetMaster(ib.author   )  [%s]\n", CfgGetMaster("ib.author"   ));
			printf("\t CfgGetMaster(ib.company  )  [%s]\n", CfgGetMaster("ib.company"  ));
			printf("\t -------------------------------------------\n");

			CfgOxaPoll();
			CfgOxaLine();
			CfgOxa();

			iLLen        = UtilToInt(CfgGetOxa("LENGTH_LEN"));
			iLPls        = UtilToInt(CfgGetOxa("INC_LEN"   ));
			iTimeOutSec  = UtilToInt(CfgGetOxa("TIME_OUT"  ));
			iPollSendSec = UtilToInt(CfgGetOxa("POLL_TIME" ));

			printf("\t iLLen        = [%d]\n", iLLen);
			printf("\t iLPls        = [%d]\n", iLPls);
			printf("\t iTimeOutSec  = [%d]\n", iTimeOutSec);
			printf("\t iPollSendSec = [%d]\n", iPollSendSec);
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

			sprintf(strFile, "%s/%s", strPath, sServiceid);
			if (!flag) LOG(_FL_, 0, 0, "[%s]\n", strFile);

			if (access(strFile, W_OK) < 0)
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
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOxa("CONN_INFO"));  /* 접속정보 */
}

#endif
