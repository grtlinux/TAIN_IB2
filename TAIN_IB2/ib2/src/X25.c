/*
 * PROGRAM ID   : X25.c
 * PROGRAM NAME : X25 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : X25 모듈
 * HISTORY      : 2013.10.10 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define ALARM_SIGNAL_TIME  30

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 Handler
 */
/*----------------------------------------------------------------------------*/
void X25AlarmHandler(int signo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "프로그램을 종료합니다...(%d)", signo);
		if (flag) exit(0);
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 세팅 함수
 */
/*----------------------------------------------------------------------------*/
void X25AlarmSet(int sec)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (sec > 0)
		{
			/*
			 * alarm signal handler를 세팅한다.
			 */
			 
			signal(SIGALRM, X25AlarmHandler);
			alarm(sec);
		}
		else
		{
			alarm(sec);
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
int X25Send(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
        BOOL flag = TRUE;

	int nleft, nwritten, err;
	char *buf;

	buf = (char*) msg;
	nleft = len;

	while (nleft > 0) {
		nwritten = send(sock, buf, nleft, 0);
		if (nwritten < 0) {

            /* DATE.2014.03.07 : (JUN) errno 출력 추가,  EAGAIN 조건추가 */
            if (flag) LOG(_FL_, 0, 0, "ERROR : SEND RETURN=[%d], errno=[%d:%s]", nwritten, errno, strerror(errno));

			/* unix */
			if (errno == EINTR || errno == EAGAIN) {
				nwritten = 0;
			} else {
				return -2;
			}
		} else if (nwritten == 0) {
			/* DATE.2014.03.07 : (JUN) errno 출력 테스트 */
	        if (flag) LOG(_FL_, 0, 0, "ERROR : RECV RETURN 0 by close() (fd:%d)", sock);
	        if (flag) return -1;
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
int X25Recv(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	unsigned char* buf = (unsigned char*) msg;
	int nread, ret;
	int remain;
	
	nread = 0;
	remain = len;
	
	while ( remain ) {
		
		errno = 0; /* errno 초기화 */
		
		if (flag) LOG(_FL_, 0, 0, "[X25Recv] ALARM SET = [%d] sec (fd:%d)", ALARM_SIGNAL_TIME, sock);
		X25AlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set */
		ret = recv(sock, buf + nread, remain, 0);
		X25AlarmSet(0);   /* alarm signal clear */
		
		if (ret < 0) {
            /* DATE.2014.03.07 : (JUN) errno 출력 추가 */
            if (flag) LOG(_FL_, 0, 0, "ERROR : RECV RETURN=[%d], errno=[%d:%s] (fd:%d)", ret, errno, strerror(errno), sock);

			if (!flag) LOG(_FL_, 0, 0,  "@@@@@@@@@ ERROR : recv return -1\n");

			/* unix */
			if (errno == EINTR)
				ret = 0;
			else if (errno == EWOULDBLOCK)
				return -3;
			else
				return -4;
		}
		else if (ret == 0) {
            if (flag) LOG(_FL_, 0, 0, "ERROR : RECV RETURN 0 by close() (fd:%d)", sock);

			if (!flag) LOG(_FL_, 0, 0,  "@@@@@@@@@ ERROR : recv return 0\n");
			if (!flag) break;
			if (flag) return -1;
		}

		nread += ret;
		remain -= ret;
	}

	return nread;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int X25RecvFFEF(int sock, char* msg, int maxLen)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "X25RecvFFEF [START]...");

	unsigned char* buf = (unsigned char*) msg;
	int nread, ret;
	int remain;
	
	nread = 0;
	remain = maxLen;
	
	while ( remain ) {
		
		if (!flag) LOG(_FL_, 0, 0, "[X25RecvFFEF] [sock=%d] [nread=%d][remain=%d]", sock, nread, remain);
		if (!flag) LOG(_FL_, 0, 0, "[X25RecvFFEF] ALARM SET = [%d] sec (fd:%d)", ALARM_SIGNAL_TIME, sock);
		X25AlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set */
		ret = recv(sock, buf + nread, 1, 0);
		X25AlarmSet(0);   /* alarm signal clear */

		if (!flag) LOG(_FL_, 0, 0,  "[X25RecvFFEF] [sock=%d] [nread=%d][remain=%d][ret=%d]", sock, nread, remain, ret);

		if (ret < 0) {

            /* DATE.2014.03.07 : (JUN) errno 출력 추가 */
	        if (flag) LOG(_FL_, 0, 0,  "ERROR : RECV RETURN=[%d], errno=[%d:%s]", ret, errno, strerror(errno));

			if (flag) LOG(_FL_, 0, 0,  "@@@@@@@@@ ERROR : recv return -1\n");

			/* unix */
			if (errno == EINTR)
				ret = 0;
			else if (errno == EWOULDBLOCK)
				return -3;
			else
				return -4;
		}
		else if (ret == 0) {
            /* DATE.2014.03.07 : (JUN) errno 출력 추가 */
			if (flag) LOG(_FL_, 0, 0,  "ERROR : RECV RETURN 0 by close() , errno=[%d]\n", errno);

			/* TODO DATE.20131011 : 고민 필요함 */
			if (!flag) LOG(_FL_, 0, 0,  "@@@@@@@@@ ERROR : recv return 0\n");
			if (!flag) break;
			if (!flag) return -1;
		}

		nread += ret;
		remain -= ret;
		
		if (nread >= 2 && buf[nread - 2] == 0xFF && buf[nread - 1] == 0xEF) {
			/* MegaBox / MegaPac의 자료의 마지막을 나타내는 코드값 */
			if (!flag) LOG(_FL_, 0, 0,  "[X25RecvFFEF] break. [sock=%d] [nread=%d][remain=%d]", sock, nread, remain);
			break;
		}
	}

	return nread;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int X25MsgSend(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	int ret = 0;

	if (!flag) LOG(_FL_, 0, 0, "X25MsgSend [START]...");

	if (flag)
	{
		ret = X25Send(sock, msg, len);
	}
	
	return ret;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int X25MsgRecv(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	int len = 0;

	if (!flag) LOG(_FL_, 0, 0, "X25MsgRecv [START]...");

	if (flag)
	{
		len = X25Recv(sock, msg, 8192);
		msg[len] = 0x00;

		if (!flag) LOG(_FL_, 0, 0,  "[X25MsgRecv]... [%d:%s]", len, msg);
	}
	
	return len;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int X25MsgSendFFEF(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	int ret = 0;

	if (!flag) LOG(_FL_, 0, 0, "X25MsgSendFFEF [START]...");

	if (flag)
	{
		unsigned char sTail[3] = { 0xFF, 0xEF, 0x00 };
		
		if (!flag) LOG(_FL_, 0, 0,  "[X25MsgSendFFEF]...1 [len=%d][%d:%s]", len, strlen(msg), msg);
		
		strcat(msg, sTail);
		len += 2;
		
		if (!flag) LOG(_FL_, 0, 0,  "[X25MsgSendFFEF]...2 [len=%d][%d:%s]", len, strlen(msg), msg);

		ret = X25Send(sock, msg, len);
	}
	
	return ret;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
int X25MsgRecvFFEF(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	int len = 0;

    if (!flag) LOG(_FL_, 0, 0, "X25MsgRecvFFEF [START]...");

	if (flag)
	{
		len = X25RecvFFEF(sock, msg, 8192);

		len -= 2;         /* FFEF를 제거한다. */
		msg[len] = 0x00;

		/* ffef Tail 제거를 확인한다. */
		if (!flag) LOG(_FL_, 0, 0,  "[X25MsgRecvFFEF]....[%d:%s]", len, msg);
	}
	
	return len;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : InitMegaEOR
 */
/*----------------------------------------------------------------------------*/
int X25Init(int sock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0,  "[X25Init] START");

	if (flag)
	{
	    char sWait[12] = { 0xFF, 0xFB, 0x00, 0xFF, 0xFD, 0x00, 0xFF, 0xFB, 0x19, 0xFF, 0xFD, 0x19 };
	    char sRecv[12] = { 0x00, };
	    char sSend[ 3] = { 0xFF, 0xFD, 0x19 };
	    
	    int  ret = 0;
	
		if (flag)
		{
			if (!flag) LOG(_FL_, 0, 0,  "1.Negotiation 요청수신");
			/*
			 * Negotiation 요청전문을 읽는다.
			 */
			ret = X25Recv(sock, sRecv, sizeof(sRecv));
			if (ret <= 0)
			{
				if (flag) LOG(_FL_, 0, 0,  "ERROR : SockRecv..[ret=%d]", ret);
				return -1;
			}
			
			if (ret != sizeof(sRecv))
			{
				if (flag) LOG(_FL_, 0, 0,  "ERROR : ret(%d) != sizeof(sRecv)(%d) .."
					, ret, sizeof(sRecv));
				return -1;
			}
		}
		
		if (flag)
		{
			if (!flag) LOG(_FL_, 0, 0,  "2.Negotiation 비교");
			/*
			 * 수신한 전문과 sWait전문을 비교한다.
			 */
			if (memcmp(sWait, sRecv, sizeof(sWait)) != 0)
			{
				if (flag) LOG(_FL_, 0, 0,  "ERROR : compare error..");
				return -1;
			}
		}
		
		if (flag)
		{
			if (!flag) LOG(_FL_, 0, 0,  "3.Negotiation 응답송신");
			/*
			 * Negotiation 응답전문을 송신한다.
			 */
			int ret = X25Send(sock, sSend, sizeof(sSend));
			if (ret <= 0)
			{
				if (flag) LOG(_FL_, 0, 0,  "ERROR : SockSend..");
				return -1;
			}
		}
	}

	if (flag) LOG(_FL_, 0, 0,  "[X25Init] END");

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
	
	sProgName = argv[0];
	
	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	if (flag)
	{
	}
}

#endif
