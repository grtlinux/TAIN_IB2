/*
 * PROGRAM ID   : Sig.c
 * PROGRAM NAME : Signal 처리를 위해 사용하는 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Signal 처리를 위해 사용하는 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#include <Common.h>

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


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#define   MAX_LINE_SIZE      8192

typedef struct _LOAD_ALIVE_
{
	char sFepId      [10];
	char sDate       [ 8];
	char sTime       [ 6];
	char sAlive      [ 7];
	char sSec        [10];
	char sLF         [ 1];
} LOAD_ALIVE;

#define  _DEF_EXIT        1     /* exit */
#define  _DEF_IGNORE      2     /* ignore */
#define  _DEF_IGNORE_NPR  3     /* ignore and no print */

char sSMS           [100];      /* Global SMS */

void SigDefHandler(int signum);

struct
{
	int  bFlag;
	int  iSigNo;
	int  iProcMode;
	char *sSigName;
	void (*handler) (int);
}
	arSig[] =
{
	 { 1, SIGHUP     , _DEF_IGNORE    , "SIGHUP"     , SigDefHandler }, /*  1 hangup */
	 { 1, SIGINT     , _DEF_IGNORE    , "SIGINT"     , SigDefHandler }, /*  2 Interrupt */
	 { 1, SIGQUIT    , _DEF_EXIT      , "SIGQUIT"    , SigDefHandler }, /*  3 quit */
	 { 1, SIGILL     , _DEF_EXIT      , "SIGILL"     , SigDefHandler }, /*  4 Illegal instruction (not reset when caught) */
	 { 1, SIGTRAP    , _DEF_EXIT      , "SIGTRAP"    , SigDefHandler }, /*  5 trace trap (not reset when caught) */
	 { 1, SIGIOT     , _DEF_IGNORE    , "SIGIOT"     , SigDefHandler }, /*  6 IOT instruction      */
	 { 1, SIGABRT    , _DEF_EXIT      , "SIGABRT"    , SigDefHandler }, /*  6 Process abort signal */
	 { 1, SIGBUS     , _DEF_EXIT      , "SIGBUS"     , SigDefHandler }, /*  7 bus error */
	 { 1, SIGFPE     , _DEF_IGNORE    , "SIGFPE"     , SigDefHandler }, /*  8 Floating point exception */
	 { 0, SIGKILL    , _DEF_EXIT      , "SIGKILL"    , SigDefHandler }, /*  9 kill (cannot be caught of ignored) */
	 { 1, SIGUSR1    , _DEF_IGNORE    , "SIGUSR1"    , SigDefHandler }, /* 10 user defined signal 1 */
	 { 1, SIGSEGV    , _DEF_EXIT      , "SIGSEGV"    , SigDefHandler }, /* 11 Segmentation violation */
	 { 1, SIGUSR2    , _DEF_IGNORE    , "SIGUSR2"    , SigDefHandler }, /* 12 user defined signal 2 */
	 { 1, SIGPIPE    , _DEF_IGNORE_NPR, "SIGPIPE"    , SigDefHandler }, /* 13 write on a pipe with no one to read it */
#if 0
	 { 1, SIGALRM    , _DEF_IGNORE_NPR, "SIGALRM"    , SigDefHandler }, /* 14 alarm clock */
#else
	 { 1, SIGALRM    , _DEF_IGNORE    , "SIGALRM"    , SigDefHandler }, /* 14 alarm clock */
#endif
	 { 1, SIGTERM    , _DEF_EXIT      , "SIGTERM"    , SigDefHandler }, /* 15 Software termination signal from kill */
	 { 1, SIGCHLD    , _DEF_IGNORE_NPR, "SIGCHLD"    , SigDefHandler }, /* 17 Child process terminated or stopped */
	 { 1, SIGCONT    , _DEF_IGNORE_NPR, "SIGCONT"    , SigDefHandler }, /* 18 stopped process has been continued */
	 { 0, SIGSTOP    , _DEF_EXIT      , "SIGSTOP"    , SigDefHandler }, /* 19 stop (cannot be caught or ignored) */
	 { 1, SIGTSTP    , _DEF_EXIT      , "SIGTSTP"    , SigDefHandler }, /* 20 user stop requested from tty */
	 { 1, SIGTTIN    , _DEF_EXIT      , "SIGTTIN"    , SigDefHandler }, /* 21 background tty read attempted */
	 { 1, SIGTTOU    , _DEF_EXIT      , "SIGTTOU"    , SigDefHandler }, /* 22 background tty write attempted */
	 { 1, SIGURG     , _DEF_IGNORE_NPR, "SIGURG"     , SigDefHandler }, /* 23 urgent socket condition */
	 { 1, SIGXCPU    , _DEF_IGNORE    , "SIGXCPU"    , SigDefHandler }, /* 24 exceeded cpu limit */
	 { 1, SIGXFSZ    , _DEF_IGNORE    , "SIGXFSZ"    , SigDefHandler }, /* 25 exceeded file size limit */
	 { 1, SIGVTALRM  , _DEF_IGNORE    , "SIGVTALRM"  , SigDefHandler }, /* 26 virtual timer expired */
	 { 1, SIGPROF    , _DEF_IGNORE    , "SIGPROF"    , SigDefHandler }, /* 27 profiling timer expired */
	 { 1, SIGWINCH   , _DEF_IGNORE    , "SIGWINCH"   , SigDefHandler }, /* 28 window size change */
	 { 1, SIGPOLL    , _DEF_EXIT      , "SIGPOLL"    , SigDefHandler }, /* 29 pollable event occured */
	 { 1, SIGIO      , _DEF_EXIT      , "SIGIO"      , SigDefHandler }, /* SIGPOLL socket I/O possible (SIGPOLL alias) */
	 { 1, SIGPWR     , _DEF_IGNORE    , "SIGPWR"     , SigDefHandler }, /* 30 power state indication */
	 { 1, SIGSYS     , _DEF_EXIT      , "SIGSYS"     , SigDefHandler }, /* 31 bad argument to system call */

#if 0
	 { 1, SIGSTKFLT  , _DEF_EXIT      , "SIGSTKFLT"  , SigDefHandler }, /* 16 Stack fault.  */
	 { 1, __SIGRTMIN , _DEF_IGNORE    , "__SIGRTMIN" , SigDefHandler }, /* 32 first (highest-priority) realtime signal */
	 { 1, __SIGRTMAX , _DEF_IGNORE    , "__SIGRTMAX" , SigDefHandler }, /* 64 last (lowest-priority) realtime signal */
	 { 1, _NSIG      , _DEF_IGNORE    , "_NSIG"      , SigDefHandler }, /* 65 Biggest signal number + 1 (including real-time signals).  */
#endif

#if 0
	 { 1, SIGWAITING , _DEF_IGNORE    , "SIGWAITING" , SigDefHandler }, /* 32 reserved signal no longer used by threading code */
	 { 1, SIGLWP     , _DEF_IGNORE    , "SIGLWP"     , SigDefHandler }, /* 33 reserved signal no longer used by threading code */
	 { 1, SIGCANCEL  , _DEF_IGNORE    , "SIGCANCEL"  , SigDefHandler }, /* 36 reserved signal for thread cancellation */
	 { 1, SIGEMT     , _DEF_IGNORE    , "SIGEMT"     , SigDefHandler }, /*  7 EMT instruction */
	 { 1, SIGFREEZE  , _DEF_IGNORE    , "SIGFREEZE"  , SigDefHandler }, /* 34 special signal used by CPR */
	 { 1, SIGTHAW    , _DEF_IGNORE    , "SIGTHAW"    , SigDefHandler }, /* 35 special signal used by CPR */
	 { 1, SIGLOST    , _DEF_IGNORE    , "SIGLOST"    , SigDefHandler }, /* 37 resource lost (eg, record-lock lost) */
#endif

#if 0
	 { 1, SIGXRES    , _DEF_IGNORE    , "SIGXRES"    , SigDefHandler }, /* 38 resource control exceeded */
	 { 1, SIGJVM1    , _DEF_IGNORE    , "SIGJVM1"    , SigDefHandler }, /* 39 reserved signal for Java Virtual Machine */
	 { 1, SIGJVM2    , _DEF_IGNORE    , "SIGJVM2"    , SigDefHandler }, /* 40 reserved signal for Java Virtual Machine */
	 { 1, _SIGRTMIN  , _DEF_IGNORE    , "_SIGRTMIN"  , SigDefHandler }, /* 41 first (highest-priority) realtime signal */
	 { 1, _SIGRTMAX  , _DEF_IGNORE    , "_SIGRTMAX"  , SigDefHandler }, /* 48 last (lowest-priority) realtime signal */
#endif

#if 0
	 { 1, SIGVTALRM  , _DEF_IGNORE    , "SIGVTALRM"  , SigDefHandler }, /* 20 virtual timer alarm */
	 { 1, SIGPROF    , _DEF_IGNORE    , "SIGPROF"    , SigDefHandler }, /* 21 profiling timer alarm */
	 { 1, SIGIO      , _DEF_EXIT      , "SIGIO"      , SigDefHandler }, /* 22 asynchronous I/O */
	 { 1, SIGWINCH   , _DEF_IGNORE    , "SIGWINCH"   , SigDefHandler }, /* 23 window size change signal */
	 { 1, SIGSTOP    , _DEF_EXIT      , "SIGSTOP"    , SigDefHandler }, /* 24 Stop signal (cannot be caught or ignored) */
	 { 1, SIGTSTP    , _DEF_EXIT      , "SIGTSTP"    , SigDefHandler }, /* 25 Interactive stop signal */

	 { 1, SIGURG     , _DEF_IGNORE_NPR, "SIGURG"     , SigDefHandler }, /* 29 urgent condition on IO channel */
	 { 1, SIGCONT    , _DEF_IGNORE_NPR, "SIGCONT"    , SigDefHandler }, /* 26 Continue if stopped */
	 { 1, SIGTTIN    , _DEF_EXIT      , "SIGTTIN"    , SigDefHandler }, /* 27 Read from control terminal attempted by a member of a background process group */
	 { 1, SIGTTOU    , _DEF_EXIT      , "SIGTTOU"    , SigDefHandler }, /* 28 Write to control terminal attempted by a member of a background process group */
	 { 1, SIGXCPU    , _DEF_IGNORE    , "SIGXCPU"    , SigDefHandler }, /* 33 CPU time limit exceeded (setrlimit)  */
	 { 1, SIGXFSZ    , _DEF_IGNORE    , "SIGXFSZ"    , SigDefHandler }, /* 34 CPU file size limit exceeded (setrlimit)  */
	 { 1, SIGMSG     , _DEF_IGNORE    , "SIGMSG"     , SigDefHandler }, /* 27 input data is in the ring buffer             */
	 { 1, SIGDANGER  , _DEF_IGNORE    , "SIGDANGER"  , SigDefHandler }, /* 33 system crash imminent; free up some page space  */
	 { 1, SIGMIGRATE , _DEF_IGNORE    , "SIGMIGRATE" , SigDefHandler }, /* 35 migrate process                                 */
	 { 1, SIGPRE     , _DEF_IGNORE    , "SIGPRE"     , SigDefHandler }, /* 36 programming exception                           */
	 { 1, SIGVIRT    , _DEF_IGNORE    , "SIGVIRT"    , SigDefHandler }, /* 37 AIX virtual time alarm                          */
	 { 1, SIGALRM1   , _DEF_IGNORE    , "SIGALRM1"   , SigDefHandler }, /* 38 m:n condition variables - RESERVED - DON'T USE  */
	 { 1, SIGWAITING , _DEF_IGNORE    , "SIGWAITING" , SigDefHandler }, /* 39 process's lwps are blocked                      */
	 { 1, SIGRECONFIG, _DEF_IGNORE    , "SIGRECONFIG", SigDefHandler }, /* 58 Reserved for Dynamic Reconfiguration Operations */
	 { 1, SIGCPUFAIL , _DEF_IGNORE    , "SIGCPUFAIL" , SigDefHandler }, /* 59 Predictive De-configuration of Processors -     */
	 { 1, SIGKAP     , _DEF_IGNORE    , "SIGKAP"     , SigDefHandler }, /* 60 keep alive poll from native keyboard            */
	 { 1, SIGRETRACT , _DEF_IGNORE    , "SIGRETRACT" , SigDefHandler }, /* 61 monitor mode should be relinguished             */
	 { 1, SIGSOUND   , _DEF_IGNORE    , "SIGSOUND"   , SigDefHandler }, /* 62 sound control has completed                     */
	 { 1, SIGSAK     , _DEF_IGNORE    , "SIGSAK"     , SigDefHandler }, /* 63 secure attention key                            */
#endif
	 { 0, 0          , 0              , NULL         , NULL          }
};

/*============================================================================*/
/*
 * 내용 : Signal 명칭을 얻는 함수
 */
/*----------------------------------------------------------------------------*/
char* SigGetName(int signum)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; arSig[i].sSigName; i++)
		{
			if (arSig[i].iSigNo == signum)
			{
				return arSig[i].sSigName;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : Signal 명칭을 얻는 함수
 */
/*----------------------------------------------------------------------------*/
char* SigGetMode(int mode)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		switch (mode)
		{
			case _DEF_EXIT       : return "_DEF_EXIT";
			case _DEF_IGNORE     : return "_DEF_IGNORE";
			case _DEF_IGNORE_NPR : return "_DEF_IGNORE_NPR";
		}
	}

	return "???";
}

/*============================================================================*/
/*
 * 내용 : Signal 수신치 처리하는 함수
 */
/*----------------------------------------------------------------------------*/
void SigDefHandler(int signum)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		int i;
		for (i=0; arSig[i].sSigName; i++)
		{
			if (arSig[i].iSigNo != signum)
				continue;

			switch (arSig[i].iProcMode)
			{
				case _DEF_EXIT:    /* exit signal */
					if (flag)
					{
#if 0
						if (flag) LOG(_FL_, 9, 0, "SIGNAL : [(%2d)%-10.10s] -> [%-15.15s]"
							" signal received... process EXIT."
							, arSig[i].iSigNo, arSig[i].sSigName, SigGetMode(_DEF_EXIT));
#else 
                        if (flag) LOG(_FL_, 9, 0, "\x1b[41m----------(%2d)%s signal received... process EXIT (%s) ----------\x1b[0m"
                        	, arSig[i].iSigNo, arSig[i].sSigName, sServiceid);
                        	
                        /* 문자메시지 전송 :  SIGTERM 이 아닌경우 문자전송*/
                        if ( arSig[i].iSigNo != 15 ) {                        	
	                        
						    sprintf(sSMS, "%s (%2d)%s signal received... process EXIT" , sServiceid, arSig[i].iSigNo, arSig[i].sSigName);
	                        	
					        if (UmsMsgSend(sSMS) < 0)
							{
								if (flag) LOG(_FL_, 9, 0, "ERROR : UMS SEND....");
							}
						}
#endif

						exit(0);
					}
					break;

				case _DEF_IGNORE:    /* ignore signal */
					if (flag)
					{
						if (flag) LOG(_FL_, 9, 0, "SIGNAL : [(%2d)%-10.10s] -> [%-15.15s]"
							" signal received..."
							, arSig[i].iSigNo, arSig[i].sSigName, SigGetMode(_DEF_IGNORE));

						if (signum != SIGALRM)
						{
							if (flag && signum == SIGCHLD)
							{
								int stat;
								pid_t pid_child;

								/* process dead child */
								do
								{
									pid_child = waitpid(-1, &stat, WNOHANG);
								} while (pid_child != -1);
							}
						}

						signal(arSig[i].iSigNo, arSig[i].handler);
						return;
					}
					break;

				case _DEF_IGNORE_NPR:    /* ignore signal and no print */
					if (flag)
					{
						signal(arSig[i].iSigNo, arSig[i].handler);
						return;
					}
					break;
			}
		}
	}
}

/*============================================================================*/
/*
 * 내용 : Signal
 */
/*----------------------------------------------------------------------------*/
int SigSetSignal()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 0, 0, "PID = %d", getpid());
	
	if (flag)
	{
		int i;
		for (i=0; arSig[i].sSigName; i++)
		{
			if (arSig[i].bFlag)
			{
				if (!flag) LOG(_FL_, 9, 0, "SIGNAL : [(%2d)%-10.10s] -> [%-15.15s]"
					" set signal processing"
					, arSig[i].iSigNo, arSig[i].sSigName, SigGetMode(arSig[i].iProcMode));

				signal(arSig[i].iSigNo, arSig[i].handler);
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : AlarmHandler
 */
/*----------------------------------------------------------------------------*/
void SigAlarmHandler(int signum)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag) LOG(_FL_, 9, 0, "SIGNAL : [(%2d)%-10.10s]"
			" signal received, and EXIT..."
			, signum, SigGetName(signum));
		
		exit(0);
	}
}

/*============================================================================*/
/*
 * 내용 : Set AlarmHandler
 */
/*----------------------------------------------------------------------------*/
int SigSetAlarmHandler()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) signal(SIGALRM, SigAlarmHandler);
	
	return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : SIGUSR1 시스널 처리 함수
 */
/*----------------------------------------------------------------------------*/
void SigAct_SIGUSR1( int signum )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGUSR1, SigAct_SIGUSR1);
	}

	if (flag)
	{
		LOG(_FL_, 0, 0, "SIGNAL(USR1) : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(USR1) : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(USR1) : 메시지를 출력합니다....");
	}
}

/*============================================================================*/
/*
 * 내용 : SIGUSR1 시스널 세팅 함수
 */
/*----------------------------------------------------------------------------*/
void SigSet_SIGUSR1( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGUSR1, SigAct_SIGUSR1);
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 처리 함수
 */
/*----------------------------------------------------------------------------*/
void SigAct_SIGALRM( int signum )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, SigAct_SIGALRM);
	}

	if (flag)
	{
		LOG(_FL_, 0, 0, "SIGNAL(ALRM) : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM) : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM) : 메시지를 출력합니다....");
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 처리 함수-1
 */
/*----------------------------------------------------------------------------*/
void SigAct_SIGALRM_1( int signum )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, SigAct_SIGALRM_1);
	}

	if (flag)
	{
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-1 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-1 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-1 : 메시지를 출력합니다....");
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 처리 함수-2
 */
/*----------------------------------------------------------------------------*/
void SigAct_SIGALRM_2( int signum )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, SigAct_SIGALRM_2);
	}

	if (flag)
	{
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 세팅 함수
 */
/*----------------------------------------------------------------------------*/
void SigSet_SIGALRM( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, SigAct_SIGALRM);
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 세팅 함수 - 전달형
 */
/*----------------------------------------------------------------------------*/
void SigSet_SIGALRM_Param( void (*sigfunc) (int) )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, *sigfunc);
	}
}

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
void SigAlarmHandler_old(int signo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "SIGNAL(%d) : 메시지를 출력합니다....", signo);
		if (flag) printf("프로그램을 종료합니다.\n");
		if (flag) exit(0);
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 세팅 함수
 */
/*----------------------------------------------------------------------------*/
void SigAlarmSet_old(int sec)
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
			 
			signal(SIGALRM, SigAlarmHandler_old);
			alarm(sec);
		}
		else
		{
			alarm(sec);
		}
	}
}


/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SigAliveWrite(char* sProcessId)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	LOG(_FL_, 0, 0, "Process_Alive..[%s]", sProcessId);
	
	int iProcess = 1;
	
	if (flag)
	{
		/*
		 * DAT 파일에 메시지를 쓴다.
		 */

		FILE* fp;
		FILE* fp2;
		FILE* fp3;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

        char* p = CfgGetMst("ib.path.file.ps_alive");
        
		if (p == NULL)
		{
			LOG(_FL_, 9, 0, "ERROR : PS_ALIVE 파일 미정의");
			return -1;
		}

		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);

		if (flag)
		{
			if (flag)
			{
				/* 폴더를 확인한다. 없으면 만든다. */
				char sPath[128];
				strcpy(sPath, sFileName);

				char* p = rindex(sPath, '/');
				*p = 0x00;
				if (!flag) LOG(_FL_, 0, 0, "[sPath:%s]", sPath);

				if (access(sPath, R_OK) < 0)
				{
					/* 폴더가 없으면 생성한다. */
					mkdir(sPath, 0750);
					
					LOG(_FL_, 9, 0, "폴더 생성=[%s]", sPath);
				}
			}

			if (flag)
			{
				/* PS_ALIVE 파일을 확인하여 없으면 생성한다. */
				if (access(sFileName, W_OK) < 0)
				{
					/* 파일이 없으면 생성한다. */
					fp = fopen(sFileName, "w");
					
					fclose(fp);
					
					LOG(_FL_, 9, 0, "파일 생성=[%s]", sFileName);
				}
			}

			if (flag)  /* 변경 */
			{
			    /* DAT 파일을 오픈한다. */
				fp2 = fopen(sFileName, "r+");
				if (fp2 == NULL)
				{
					LOG(_FL_, 9, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
					
					return -1;
				}
				
				while ( 1 )
				{
					strLine[0] = 0x00;
		
					long pos = ftell(fp2);
					char* p = fgets(strLine, MAX_LINE_SIZE, fp2);
					if (p == NULL)
						break;
		
					LOAD_ALIVE* pDat = (LOAD_ALIVE*) &strLine;
					
					if (strncmp(pDat->sFepId, sProcessId, 10) == 0)
					{
						
						memcpy(pDat->sAlive, "_ALIVE_", 7);
						memcpy(pDat->sTime, TimeGetTime(), 6);
						sprintf(pDat->sSec, "%10.10d", time(NULL) );
		                pDat->sLF[0] = 0x00;
		
						fseek(fp2, pos, SEEK_SET);
						fputs(strLine, fp2);
		
		                iProcess = 0;
					}
				}
		
				/* 처리된 파일을 닫는다. */
				fclose(fp2);
			}
			
			
			if (flag)  /* 시간체크 */
			{
			    /* DAT 파일을 오픈한다. */
				fp3 = fopen(sFileName, "r+");
				if (fp3 == NULL)
				{
					LOG(_FL_, 9, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
					
					return -1;
				}
		
				while ( 1 )
				{
					strLine[0] = 0x00;
		
					long pos = ftell(fp3);
					char* p = fgets(strLine, MAX_LINE_SIZE, fp3);
					if (p == NULL)
						break;
		
					LOAD_ALIVE* pDat = (LOAD_ALIVE*) &strLine;
					
					int iDiff = time(NULL) - atoi(pDat->sSec);
					
					if (iDiff >  20 && strncmp(pDat->sAlive, "_ALIVE_", 7) == 0 && strncmp(pDat->sFepId, "MAACOM0000", 10) == 0)
					{
						LOG(_FL_, 9, 0, "[%s]프로세스 30초 경과 -> CHECK 변경", pDat->sFepId);
						memcpy(pDat->sAlive, "_CHECK_", 7);
						memcpy(pDat->sTime, TimeGetTime(), 6);
		                pDat->sLF[0] = 0x00;
		
						fseek(fp2, pos, SEEK_SET);
						fputs(strLine, fp2);
		
		                iProcess = 0;
					}
					
					if (iDiff > 120 && strncmp(pDat->sAlive, "_ALIVE_", 7) == 0)
					{
						LOG(_FL_, 9, 0, "120초 경과 -> CHECK 변경");
						memcpy(pDat->sAlive, "_CHECK_", 7);
						memcpy(pDat->sTime, TimeGetTime(), 6);
		                pDat->sLF[0] = 0x00;
		
						fseek(fp2, pos, SEEK_SET);
						fputs(strLine, fp2);
		
		                iProcess = 0;
					}
				}
		
				/* 처리된 파일을 닫는다. */
				fclose(fp3);
			}
	
			
		}
		

		if (flag)  /* 신규 등록 */
		{
	        if ( iProcess == 1 ) {
	        	
				/* DAT 파일을 오픈한다. */
				fp = fopen(sFileName, "a");
				if (fp == NULL)
				{
					LOG(_FL_, 9, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
					if (flag) exit(-1);
					return -1;            /* TODO DATE.2013.11.07 */
				}
    
				fprintf(fp, "%10.10s%8.8s%6.6s%7.7s%10.10d\n"
					, sProcessId
					, TimeGetDate()
		            , TimeGetTime()
		            , "_ALIVE_"
		            , time(NULL) );
		
				fflush(fp);
	
				/* 처리된 파일을 닫는다. */
				fclose(fp);
				}
		}
	}

	return;
}



/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SigAliveSec(char* sProcessId)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	int iDiff = 0;
	
	if (flag)
	{
		/*
		 * DAT 파일에 메시지를 쓴다.
		 */

        FILE* fp;
		FILE* fp2;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];
		
		char* p = CfgGetMst("ib.path.file.ps_alive");
        
		if (p == NULL)
		{
			LOG(_FL_, 9, 0, "ERROR : PS_ALIVE 파일 미정의");
			return -1;
		}

		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);

		if (flag)
		{
			if (flag)
			{
				/* 폴더를 확인한다. 없으면 만든다. */
				char sPath[128];
				strcpy(sPath, sFileName);

				char* p = rindex(sPath, '/');
				*p = 0x00;
				if (!flag) LOG(_FL_, 9, 0, "[sPath:%s]", sPath);

				if (access(sPath, R_OK) < 0)
				{
					/* 폴더가 없으면 생성한다. */
					mkdir(sPath, 0750);
					
					LOG(_FL_, 9, 0, "폴더 생성=[%s]", sPath);
				}
			}

			if (flag)
			{
				/* PS_ALIVE 파일을 확인하여 없으면 생성한다. */
				if (access(sFileName, W_OK) < 0)
				{
					/* 파일이 없으면 생성한다. */
					fp = fopen(sFileName, "w");
					
					fclose(fp);
					
					LOG(_FL_, 9, 0, "파일 생성=[%s]", sFileName);
				}
			}
			
			if (flag)  /* 변경 */
			{
			    /* DAT 파일을 오픈한다. */
				fp2 = fopen(sFileName, "r+");
				if (fp2 == NULL)
				{
					LOG(_FL_, 9, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
					
					return -1;
				}
		
				while ( 1 )
				{
					strLine[0] = 0x00;
		
					long pos = ftell(fp2);
					char* p = fgets(strLine, MAX_LINE_SIZE, fp2);
					if (p == NULL)
						break;
		
					LOAD_ALIVE* pDat = (LOAD_ALIVE*) &strLine;

					if (strncmp(pDat->sFepId, sProcessId, 10) == 0 )
					{
						iDiff = time(NULL) - atoi(pDat->sSec);
					}
				}
		
				/* 처리된 파일을 닫는다. */
				fclose(fp2);
			}
		}
	}

	return iDiff;
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

	if (!flag)
	{
		SigSet_SIGALRM();

		if (flag) {
			alarm(10);
			LOG(_FL_, 0, 0, "alarm signal 10초후 발생..");
		}

		sleep(20);  /* signal interrupt 발생으로 sleep시간 모두를 채우지 않는다. */

		LOG(_FL_, 0, 0, "종료");

		sleep(2);
	}

	if (!flag)
	{
		switch (2) {
			case 1: SigSet_SIGALRM_Param( SigAct_SIGALRM_1 ); break;
			case 2: SigSet_SIGALRM_Param( SigAct_SIGALRM_2 ); break;
			default : break;
		}

		if (flag) {
			int alarmSec = 5;
			alarm(alarmSec);
			LOG(_FL_, 0, 0, "alarm signal %d 초후 발생..", alarmSec);
		}

		sleep(20);  /* signal interrupt 발생으로 sleep시간 모두를 채우지 않는다. */

		LOG(_FL_, 0, 0, "종료");

		sleep(1);
	}

	if (!flag)
	{
		SigSetSignal();

		sleep(1);

		if (!flag) kill(getpid(), SIGHUP     );
		if (!flag) kill(getpid(), SIGINT     );
		if (flag) kill(getpid(), SIGQUIT    );
		if (!flag) kill(getpid(), SIGILL     );
		if (!flag) kill(getpid(), SIGTRAP    );
		if (!flag) kill(getpid(), SIGIOT     );
		if (!flag) kill(getpid(), SIGABRT    );
		if (!flag) kill(getpid(), SIGBUS     );
		if (!flag) kill(getpid(), SIGFPE     );
		if (!flag) kill(getpid(), SIGKILL    );
		if (!flag) kill(getpid(), SIGUSR1    );
		if (!flag) kill(getpid(), SIGSEGV    );
		if (!flag) kill(getpid(), SIGUSR2    );
		if (!flag) kill(getpid(), SIGPIPE    );
		if (!flag) kill(getpid(), SIGALRM    );
		if (!flag) kill(getpid(), SIGTERM    );
		if (!flag) kill(getpid(), SIGCHLD    );
		if (!flag) kill(getpid(), SIGCONT    );
		if (!flag) kill(getpid(), SIGSTOP    );
		if (!flag) kill(getpid(), SIGTSTP    );
		if (!flag) kill(getpid(), SIGTTIN    );
		if (!flag) kill(getpid(), SIGTTOU    );
		if (!flag) kill(getpid(), SIGURG     );
		if (!flag) kill(getpid(), SIGXCPU    );
		if (!flag) kill(getpid(), SIGXFSZ    );
		if (!flag) kill(getpid(), SIGVTALRM  );
		if (!flag) kill(getpid(), SIGPROF    );
		if (!flag) kill(getpid(), SIGWINCH   );
		if (!flag) kill(getpid(), SIGPOLL    );
		if (!flag) kill(getpid(), SIGIO      );
		if (!flag) kill(getpid(), SIGPWR     );
		if (!flag) kill(getpid(), SIGSYS     );

		sleep(1);
	}
	
	if (!flag)
	{
		SigAlarmSet_old(5);
		if (flag) printf("0. Alarm Setting...\n");
		
		if (flag) sleep(4);
		
		SigAlarmSet_old(0);
		if (flag) printf("1. Alarm Clear...\n");
	}

	if (flag)
	{
		SigSetSignal();

		if (flag) SigSetAlarmHandler();

		while ( 1 )
		{
			printf("signal program is running.....\n");
			sleep(10);
		}
	}
}

#endif
