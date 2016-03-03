/*
 * PROGRAM ID   : Poll.c
 * PROGRAM NAME : Poll 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Poll 모듈
 * HISTORY      : 2013.09.30 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define   MAX_LINE_SIZE      1024
#define   MAX_ITEM             20

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

char      strLine          [MAX_LINE_SIZE];
char*     arItem                [MAX_ITEM];

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* PollSetMMDDHHMMSS(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sDate = TimeGetDateTime();   /* YYYYMMDDhhmmssSSSSSS */
		char* p = strstr(str, "MMDDHHMMSS");

		if (p != NULL)
		{
			memcpy(p, sDate + 4, 10);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* PollSetYYMMDDHHMMSS(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sDate = TimeGetDateTime();   /* YYYYMMDDhhmmssSSSSSS */
		char* p = strstr(str, "YYMMDDHHMMSS");

		if (p != NULL)
		{
			memcpy(p, sDate + 2, 12);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* PollSetYYYYMMDDHHMMSS(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sDate = TimeGetDateTime();   /* YYYYMMDDhhmmssSSSSSS */
		char* p = strstr(str, "YYYYMMDDHHMMSS");

		if (p != NULL)
		{
			memcpy(p, sDate, 14);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* PollSetYYYYMMDD(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sDate = TimeGetDateTime();   /* YYYYMMDDhhmmssSSSSSS */
		char* p = strstr(str, "YYYYMMDD");

		if (p != NULL)
		{
			memcpy(p, sDate, 8);
		}
	}

	return str;
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
int Poll2GetPollTime()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static int iPollTime = 0;
	
	if (flag)
	{
		if (iPollTime == 0)
		{
			strcpy(strLine, CfgGetPoll("SEND_REQ"));
			UtilStrSplit(strLine, ':', arItem);
			
			iPollTime = UtilToInt(UtilTrim(arItem[0]));
		}
	}
	
	return iPollTime;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* Poll2MsgSendReq()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static char msg[MAX_LINE_SIZE] = { 0x00, };
	
	if (flag)
	{
		if (*msg == 0x00)
		{
			strcpy(strLine, CfgGetPoll("SEND_REQ"));
			UtilStrSplit(strLine, ':', arItem);

			UtilTrim(arItem[1]);
			
			if (arItem[1][0] == 'M')  /* Make */
			{
				char* p = rindex(arItem[1], ']');
				*p = 0x00;
				p = index(arItem[1], '[') + 1;
				
				strcpy(msg, p);
				
				PollSetYYYYMMDDHHMMSS(msg);
				PollSetYYMMDDHHMMSS(msg);
				PollSetMMDDHHMMSS(msg);
				PollSetYYYYMMDD(msg);
			}
			else if (arItem[1][0] == 'R')  /* Replace */
			{
			}
		}
	}
	
	return msg;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* Poll2MsgSendRes()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static char msg[MAX_LINE_SIZE] = { 0x00, };
	
	if (flag)
	{
		if (*msg == 0x00)
		{
			strcpy(strLine, CfgGetPoll("SEND_RES"));
			UtilStrSplit(strLine, ':', arItem);

			UtilTrim(arItem[1]);
			
			if (arItem[1][0] == 'M')  /* Make */
			{
				char* p = rindex(arItem[1], ']');
				*p = 0x00;
				p = index(arItem[1], '[') + 1;
				
				strcpy(msg, p);

				PollSetYYYYMMDDHHMMSS(msg);
				PollSetYYMMDDHHMMSS(msg);
				PollSetMMDDHHMMSS(msg);
				PollSetYYYYMMDD(msg);
			}
			else if (arItem[1][0] == 'R')  /* Replace */
			{
			}
		}
	}
	
	return msg;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int Poll2CmpRecvReq(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static int iPos, iLen;
	static char strCmp[50] = { 0x00, };
	
	if (flag)
	{
		if (*strCmp == 0x00)
		{
			strcpy(strLine, CfgGetPoll("RECV_REQ"));
			UtilStrSplit(strLine, ',', arItem);
			
			iPos = UtilToInt(UtilTrim(arItem[1]));
			iLen = UtilToInt(UtilTrim(arItem[2]));
			strcpy(strCmp, UtilTrim(arItem[3]));
		}
	}
	
	return memcmp(str + iPos, strCmp, iLen);
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int Poll2CmpRecvRes(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static int iPos, iLen;
	static char strCmp[50] = { 0x00, };
	
	if (flag)
	{
		if (*strCmp == 0x00)
		{
			strcpy(strLine, CfgGetPoll("RECV_RES"));
			UtilStrSplit(strLine, ',', arItem);
			
			iPos = UtilToInt(UtilTrim(arItem[1]));
			iLen = UtilToInt(UtilTrim(arItem[2]));
			strcpy(strCmp, UtilTrim(arItem[3]));
		}
	}
	
	return memcmp(str + iPos, strCmp, iLen);
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
int PollGetPollTime()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static int iPollTime = 0;
	
	if (flag)
	{
		if (iPollTime == 0)
		{
			strcpy(strLine, CfgGetOxaPoll("SEND_REQ"));
			UtilStrSplit(strLine, ':', arItem);
			
			iPollTime = UtilToInt(UtilTrim(arItem[0]));
		}
	}
	
	return iPollTime;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* PollMsgSendReq()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static char msg[MAX_LINE_SIZE] = { 0x00, };
	
	if (flag)
	{
		if (*msg == 0x00)
		{
			strcpy(strLine, CfgGetOxaPoll("SEND_REQ"));
			UtilStrSplit(strLine, ':', arItem);

			UtilTrim(arItem[1]);
			
			if (arItem[1][0] == 'M')  /* Make */
			{
				char* p = rindex(arItem[1], ']');
				*p = 0x00;
				p = index(arItem[1], '[') + 1;
				
				strcpy(msg, p);
				
				PollSetYYYYMMDDHHMMSS(msg);
				PollSetYYMMDDHHMMSS(msg);
				PollSetMMDDHHMMSS(msg);
				PollSetYYYYMMDD(msg);
			}
			else if (arItem[1][0] == 'R')  /* Replace */
			{
			}
		}
	}
	
	return msg;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* PollMsgSendRes()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static char msg[MAX_LINE_SIZE] = { 0x00, };
	
	if (flag)
	{
		if (*msg == 0x00)
		{
			strcpy(strLine, CfgGetOxaPoll("SEND_RES"));
			UtilStrSplit(strLine, ':', arItem);

			UtilTrim(arItem[1]);
			
			if (arItem[1][0] == 'M')  /* Make */
			{
				char* p = rindex(arItem[1], ']');
				*p = 0x00;
				p = index(arItem[1], '[') + 1;
				
				strcpy(msg, p);

				PollSetYYYYMMDDHHMMSS(msg);
				PollSetYYMMDDHHMMSS(msg);
				PollSetMMDDHHMMSS(msg);
				PollSetYYYYMMDD(msg);
			}
			else if (arItem[1][0] == 'R')  /* Replace */
			{
			}
		}
	}
	
	return msg;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int PollCmpRecvReq(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static int iPos, iLen;
	static char strCmp[50] = { 0x00, };
	
	if (flag)
	{
		if (*strCmp == 0x00)
		{
			strcpy(strLine, CfgGetOxaPoll("RECV_REQ"));
			UtilStrSplit(strLine, ',', arItem);
			
			iPos = UtilToInt(UtilTrim(arItem[1]));
			iLen = UtilToInt(UtilTrim(arItem[2]));
			strcpy(strCmp, UtilTrim(arItem[3]));
		}
	}
	
	return memcmp(str + iPos, strCmp, iLen);
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int PollCmpRecvRes(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static int iPos, iLen;
	static char strCmp[50] = { 0x00, };
	
	if (flag)
	{
		if (*strCmp == 0x00)
		{
			strcpy(strLine, CfgGetOxaPoll("RECV_RES"));
			UtilStrSplit(strLine, ',', arItem);
			
			iPos = UtilToInt(UtilTrim(arItem[1]));
			iLen = UtilToInt(UtilTrim(arItem[2]));
			strcpy(strCmp, UtilTrim(arItem[3]));
		}
	}
	
	return memcmp(str + iPos, strCmp, iLen);
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
	
	sProgName = argv[0];
	
	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	if (flag)
	{
		/*
		 * master 환경파일을 읽는다. biz.properties 파일도 읽는다.
		 */
		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}
	}
	
	if (!flag)
	{
		if (flag) strcpy(sServiceid, "OXASHC0101");

		CfgOxaPoll();
		
		if (flag)
		{
			LOG(_FL_, 0, 0,  "[SEND_REQ] = [%s]", CfgGetOxaPoll("SEND_REQ"));
			LOG(_FL_, 0, 0,  "[RECV_RES] = [%s]", CfgGetOxaPoll("RECV_RES"));
			LOG(_FL_, 0, 0,  "[RECV_REQ] = [%s]", CfgGetOxaPoll("RECV_REQ"));
			LOG(_FL_, 0, 0,  "[SEND_RES] = [%s]", CfgGetOxaPoll("SEND_RES"));
		}
		
		if (flag)
		{
			printf("[SEND_REQ:%s]\n", PollMsgSendReq());
			printf("[SEND_RES:%s]\n", PollMsgSendRes());
		}
		
		if (flag)
		{
			char* p;

			if (flag)
			{
				p = "0020HDRREQPOLL0930123456";
				
				if (PollCmpRecvReq(p) == 0)
					printf("RECV_REQ == [%s]\n", p);
				else if (PollCmpRecvRes(p) == 0)
					printf("RECV_RES == [%s]\n", p);
				else
					printf("Not Poll Msg [%s]\n", p);
			}
			
			if (flag)
			{
				p = "0020HDRRESPOLL0930123456";
				
				if (PollCmpRecvReq(p) == 0)
					printf("RECV_REQ == [%s]\n", p);
				else if (PollCmpRecvRes(p) == 0)
					printf("RECV_RES == [%s]\n", p);
				else
					printf("Not Poll Msg [%s]\n", p);
			}			

			if (flag)
			{
				p = "0021HDRAAAPOLL0930123456";
				
				if (PollCmpRecvReq(p) == 0)
					printf("RECV_REQ == [%s]\n", p);
				else if (PollCmpRecvRes(p) == 0)
					printf("RECV_RES == [%s]\n", p);
				else
					printf("Not Poll Msg [%s]\n", p);
			}			
		}
	}

	if (flag)
	{
		if (flag) strcpy(sServiceid, "OXATAT0101");

		CfgOxaPoll();
		
		if (flag)
		{
			LOG(_FL_, 0, 0,  "[SEND_REQ] = [%s]", CfgGetOxaPoll("SEND_REQ"));
			LOG(_FL_, 0, 0,  "[RECV_RES] = [%s]", CfgGetOxaPoll("RECV_RES"));
			LOG(_FL_, 0, 0,  "[RECV_REQ] = [%s]", CfgGetOxaPoll("RECV_REQ"));
			LOG(_FL_, 0, 0,  "[SEND_RES] = [%s]", CfgGetOxaPoll("SEND_RES"));
		}
		
		if (flag)
		{
			printf("[SEND_REQ:%s]\n", PollMsgSendReq());
			printf("[SEND_RES:%s]\n", PollMsgSendRes());
		}
		
		if (flag)
		{
			char* p;

			if (flag)
			{
				p = "0031TQ01                  99999";
				
				if (PollCmpRecvReq(p) == 0)
					printf("RECV_REQ == [%s]\n", p);
				else if (PollCmpRecvRes(p) == 0)
					printf("RECV_RES == [%s]\n", p);
				else
					printf("Not Poll Msg [%s]\n", p);
			}
			
			if (flag)
			{
				p = "0027TR01                  0";
				
				if (PollCmpRecvReq(p) == 0)
					printf("RECV_REQ == [%s]\n", p);
				else if (PollCmpRecvRes(p) == 0)
					printf("RECV_RES == [%s]\n", p);
				else
					printf("Not Poll Msg [%s]\n", p);
			}			

			if (flag)
			{
				p = "0027CQ01                  0";
				
				if (PollCmpRecvReq(p) == 0)
					printf("RECV_REQ == [%s]\n", p);
				else if (PollCmpRecvRes(p) == 0)
					printf("RECV_RES == [%s]\n", p);
				else
					printf("Not Poll Msg [%s]\n", p);
			}			
		}
	}
}

#endif
