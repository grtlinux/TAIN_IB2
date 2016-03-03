/*
 *  PROGRAM ID   : fepstat.c
 *  PROGRAM NAME : TCP/IP Server Check
 *  DATE         : 2013-05-04
 *  REMAKE       : print the information of FEP state
 *  PROPERTIES   : create by Kang Seok.....
 */

/*
 *  Header File
 */
#include <Common.h>
/*
#include <oxacom0000.h>
*/

/*
 *  DEFINE
 */
#define _DEBUG_FLAG_   _DEBFLG   /* compile option : -D_DEBFLG=1/0 */

#define STR_COLOR_RED           "\x1b[41m"
#define STR_COLOR_GREEN         "\x1b[42m"
#define STR_COLOR_BLUE          "\x1b[44m"
#define STR_COLOR_RESET         "\x1b[0m"

#define FILE_FEPSTAT_PS         "/tmp/fepstat_ps.lst"
#define FILE_FEPSTAT_NETSTAT    "/tmp/fepstat_netstat.lst"

#define STR_OFS                 "OFS%5.5s%2.2s"
#define STR_OFR                 "OFR%5.5s%2.2s"
#define STR_OIS                 "OIS%5.5s%2.2s"
#define STR_OIR                 "OIR%5.5s%2.2s"
#define STR_OXS                 "OXS%5.5s%2.2s"
#define STR_OXR                 "OXR%5.5s%2.2s"
#define STR_OSA                 "OSA%5.5s%2.2s"
#define STR_OXA                 "OXA%5.5s%2.2s"
#define STR_BXS                 "BXS%5.5s%2.2s"
#define STR_BXR                 "BXR%5.5s%2.2s"
#define STR_BSA                 "BSA%5.5s%2.2s"
#define STR_BXA                 "BXA%5.5s%2.2s"

#define AP01                    "01"
#define AP02                    "02"

#define ARGNO                   3
#define ARGNO_PROGNAME          0
#define ARGNO_FEPID             1   /* FEPID */
#define ARGNO_COMMENT           2   /* FEP COMMENT */

#define SIZ_LINE                1024

/*
 * Global Variables
 */

FILE* gstPs;    /* File Descriptor of FILE_FEPSTAT_PS */
FILE* gstNs;    /* File Descriptor of FILE_FEPSTAT_NETSTAT */

char  gsFepId        [128];
char  gsApNo         [128];

char  gsName         [128];

char  gsPid          [128];
char  gsProgName     [128];
char  gsSvcName      [128];

char  gsLLocal       [128];
char  gsLRemote      [128];
char  gsLState       [128];
char  gsLPid         [128];
char  gsLProgName    [128];
char  gsLPort        [128];

char  gsCLocal       [128];
char  gsCRemote      [128];
char  gsCState       [128];
char  gsCPid         [128];
char  gsCProgName    [128];

char  gsMegaBox      [128];
char  gsComment      [128];

char* gsDispStr            = "%s%5.5s %2.2s %-10.10s %6s %5s %-21s %-3s %-21s %-12s %4s %-30s%s\n";
/* char* gsDispStr            = "%5.5s %2.2s %-10.10s %6s %5s %-21s %-3s %-21s %-12s %4s %-30s %s\n"; */
int   giDispCnt           ;

/*============================================================================*/
/*
 */

int   giListenPort   [4096];

int SetListenPort(char* sPort)
{
	int i;
	for (i=0; i < 4096; i++)
	{
		if (giListenPort[i] == 0)
		{
			giListenPort[i] = atoi(sPort);
			return giListenPort[i];
		}
	}
	
	return 0;
}

int SearchListenPort(char* sPort)
{
	int iPort = atoi(sPort);
	int i;
	for (i=0; i < 4096; i++)
	{
		if (giListenPort[i] == iPort)
		{
			return giListenPort[i];
		}
	}
	
	return 0;
}

/*============================================================================*/
/*
 *  Function     : ValidateArgument
 *  Arguments    : int argc, char *argv[]
 *  Return Type  : int
 *  Return Value :
 *  Description  : Validate Arguements
 */
int ValidateArgument(int argc, char* argv[])
{
	if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> [argc=%d]\n", __FUNCTION__, __LINE__, argc);

	/* check FEPID */
	{
		char* fepid = argv[ARGNO_FEPID];

		if (strlen(fepid) != 5)
		{
			if (1) printf("ERROR : FEPID size is 5 bytes. your's [%s]\n", argv[ARGNO_FEPID]);
			return -1;
		}
	}

	if (0 && _DEBUG_FLAG_) printf("SUCCESS : Check for validation of arguments.\n");

	return 0;
}

/*============================================================================*/
/*
 *  Function     : OpenFepStatFile
 *  Arguments    :
 *  Return Type  : int
 *  Return Value :
 *  Description  : Service Function
 */
int OpenFepStatFile()
{
	if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> [FILE_FEPSTAT_PS=%s]\n", __FUNCTION__, __LINE__, FILE_FEPSTAT_PS);
	if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> [FILE_FEPSTAT_NETSTAT=%s]\n", __FUNCTION__, __LINE__, FILE_FEPSTAT_NETSTAT);

	gstPs = fopen(FILE_FEPSTAT_PS, "r");
	if (gstPs == NULL)
	{
		if (1) printf("ERROR : couldn't open the file [%s] errno=[%d:%s]\n", FILE_FEPSTAT_PS, errno, strerror(errno));
		return -1;
	}

	gstNs = fopen(FILE_FEPSTAT_NETSTAT, "r");
	if (gstNs == NULL)
	{
		if (1) printf("ERROR : couldn't open the file [%s] errno=[%d:%s]\n", FILE_FEPSTAT_NETSTAT, errno, strerror(errno));
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*
 *  Function     : CloseFepStatFile
 *  Arguments    :
 *  Return Type  : int
 *  Return Value :
 *  Description  : Service Function
 */
int CloseFepStatFile()
{
	fclose(gstPs);
	fclose(gstNs);

	return 0;
}

/*============================================================================*/
/*
 *  Function     : GetNPrintFepInfo
 *  Arguments    : char* name, char* fepid, char* apno, int flag
 *  Return Type  : int
 *  Return Value :
 *  Description  : Get the information of the Service
 */
int GetNPrintFepInfo(char* name, char* fepid, char* apno)
{
	if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> [FEPID=%s]\n", __FUNCTION__, __LINE__, fepid);

	char sLine[SIZ_LINE+1];

	int   iFlag      = 0;

	int   iCnt       = 0;

	strcpy(gsFepId, fepid);
	strcpy(gsApNo , apno );
	sprintf(gsName, name, fepid, apno);

	gsPid[0]       = 0x00;
	gsProgName[0]  = 0x00;
	gsSvcName[0]   = 0x00;

	gsLLocal[0]    = 0x00;
	gsLRemote[0]   = 0x00;
	gsLState[0]    = 0x00;
	gsLPid[0]      = 0x00;
	gsLProgName[0] = 0x00;
	gsLPort[0]     = 0x00;

	gsCLocal[0]    = 0x00;
	gsCRemote[0]   = 0x00;
	gsCState[0]    = 0x00;
	gsCPid[0]      = 0x00;
	gsCProgName[0] = 0x00;

	gsMegaBox[0]   = 0x00;

	/* FILE_FEPSTAT_PS */
	{
		rewind(gstPs);

		iFlag = 0;
		while (fgets(sLine, SIZ_LINE, gstPs))
		{
			char* saveptr;
			char* pid        = NULL;
			char* progname   = NULL;
			char* svcname    = NULL;

			if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> [%s]\n", __FUNCTION__, __LINE__, sLine);
			
			{
				char* p = strstr(sLine, "ps_man");
				if (p != NULL)
					continue;
			}

			pid      = strtok_r(sLine, " \n", &saveptr);
			progname = strtok_r(NULL , " \n", &saveptr);
			svcname  = strtok_r(NULL , " \n", &saveptr);
			
			if (svcname == NULL )
			{
				break;  /* 20140521 coredump로 추가 */
			}

			if (strcmp(gsName, svcname) == 0)
			{
				strcpy(gsPid     , pid     );
				strcpy(gsProgName, progname);
				strcpy(gsSvcName , svcname );

				if (0 && _DEBUG_FLAG_) printf(">>>> [%s][%s][%s]\n", gsPid, gsProgName, gsSvcName);

				iFlag = 1;
				break;
			}
		}

		if (iFlag == 0)
			return -1;
	}

	/* FILE_FEPSTAT_NETSTAT */
	{
		rewind(gstNs);

		iFlag = 0;
		while (fgets(sLine, SIZ_LINE, gstNs))
		{
			char* saveptr;
			char* local      = NULL;
			char* remote     = NULL;
			char* state      = NULL;
			char* pid        = NULL;
			char* progname   = NULL;

			if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> [%s]\n", __FUNCTION__, __LINE__, sLine);

			local    = strtok_r(sLine, " /\n", &saveptr);
			remote   = strtok_r(NULL , " /\n", &saveptr);
			state    = strtok_r(NULL , " /\n", &saveptr);
			pid      = strtok_r(NULL , " /\n", &saveptr);
			progname = strtok_r(NULL , " /\n", &saveptr);

            if (pid == NULL )
			{
				return -1;  /* 20140521 coredump로 추가 */
			}

			if (strcmp(gsPid, pid) == 0)
			{
				if (strcmp(state, "LISTEN") == 0)
				{
					strcpy(gsLLocal    , local   );
					strcpy(gsLRemote   , remote  );
					strcpy(gsLState    , state   );
					strcpy(gsLPid      , pid     );
					strcpy(gsLProgName , progname);

					strtok_r(local, ":", &saveptr);
					strcpy(gsLPort, strtok_r(NULL, ":", &saveptr));
					
					SetListenPort(gsLPort);

					if (0 && _DEBUG_FLAG_) printf(">>>> [%s][%s][%s][%s][%s][%s]\n", gsLLocal, gsLRemote, gsLState, gsLPid, gsLProgName, gsLPort);
				}
				else
				{
					char* sMegaBox1 = "172.30.220.204";
					char* sMegaBox2 = "172.30.220.182";
					int len1 = strlen(sMegaBox1);
					int len2 = strlen(sMegaBox2);

					strcpy(gsCLocal    , local   );
					strcpy(gsCRemote   , remote  );
					strcpy(gsCState    , state   );
					strcpy(gsCPid      , pid     );
					strcpy(gsCProgName , progname);

					if (1)
					{
						strtok_r(local, ":", &saveptr);
						strcpy(gsLPort, strtok_r(NULL, ":", &saveptr));
					}

					if (strncmp(gsCRemote, sMegaBox1, len1) == 0 || strncmp(gsCRemote, sMegaBox2, len2) == 0)
					{
						strcpy(gsMegaBox, "MBOX");
					}

					if (0 && _DEBUG_FLAG_) printf(">>>> [%s][%s][%s][%s][%s]\n", gsCLocal, gsCRemote, gsCState, gsCPid, gsCProgName);

					/* Print the information of the FEP Service State */
					/* "FEPID AP  SvcName    PID   LPort    Local IP:Port            Remote IP:Port        State     MBOX    Comment"  */
					/* "----- -- ---------- ------ ----- --------------------- --- --------------------- ------------ ---- ------------------------------" */
					if (1) {
						static int flag = 0;

						char progname[1024];
						sprintf(progname, "[%s:%s:%s]", gsProgName, gsLProgName, gsCProgName);

						if (1) printf(gsDispStr
							, gsCState[0] == 'E' || gsSvcName[1] == 'S' ? "" : STR_COLOR_RED
							, flag ? "" : gsFepId
							, flag == atoi(apno) ? "" : gsApNo
							, gsSvcName
							, gsPid
							, SearchListenPort(gsLPort) ? gsLPort : ""
							, SearchListenPort(gsLPort) ? "" : gsCLocal
							, name[1] != 'X' ? "" : name[2] == 'A' ? "<->" : name[2] == 'S' ? " ->" : "<- "
							, gsCRemote
							, gsCState
							, gsMegaBox
							, gsComment
							, STR_COLOR_RESET
							, progname
							);

						gsComment[0]   = 0x00;

						flag = atoi(apno);

						giDispCnt ++;
						iCnt ++;
					}
				}

				iFlag = 1;
			}
		}

		if (0 && iFlag == 0)
			return -1;
	}

	/* Print the information of the FEP Service State */
	/* "FEPID AP  SvcName    PID   LPort    Local IP:Port            Remote IP:Port        State     MBOX    Comment"  */
	/* "----- -- ---------- ------ ----- --------------------- --- --------------------- ------------ ---- ------------------------------" */
	if (1 && iCnt == 0) {
		static int flag = 0;

		char progname[1024];
		sprintf(progname, "[%s:%s:%s]", gsProgName, gsLProgName, gsCProgName);

		if (1) printf(gsDispStr
			, gsCState[0] == 'E' || gsSvcName[1] == 'S' ? "" : STR_COLOR_RED
			, flag ? "" : gsFepId
			, flag == atoi(apno) ? "" : gsApNo
			, gsSvcName
			, gsPid
			, gsLPort
			, gsLPort[0] ? "" : gsCLocal
			, name[1] != 'X' ? "" : name[2] == 'A' ? "<->" : name[2] == 'S' ? " ->" : "<- "
			, gsCRemote
			, gsCState
			, gsMegaBox
			, gsComment
			, STR_COLOR_RESET
			, progname
			);

		gsComment[0]   = 0x00;

		flag = atoi(apno);

		giDispCnt ++;
	}

	return 0;
}

/*============================================================================*/
/*
 *  Function     : Service
 *  Arguments    : int argc, char *argv[]
 *  Return Type  : int
 *  Return Value :
 *  Description  : Service Function
 */
int Service(int argc, char* argv[])
{
	char* sFepId = argv[ARGNO_FEPID];

	if (0 && _DEBUG_FLAG_) printf("[%s:%d] >>>> FEPID = [%s]\n", __FUNCTION__, __LINE__, sFepId);

	strcpy(gsComment, argv[ARGNO_COMMENT]);

	giDispCnt = 0;

	GetNPrintFepInfo(STR_OFS, sFepId, AP01);
	GetNPrintFepInfo(STR_OFR, sFepId, AP01);
	GetNPrintFepInfo(STR_OIS, sFepId, AP01);
	GetNPrintFepInfo(STR_OIR, sFepId, AP01);
	GetNPrintFepInfo(STR_OXS, sFepId, AP01);
	GetNPrintFepInfo(STR_OXR, sFepId, AP01);
	GetNPrintFepInfo(STR_OXA, sFepId, AP01);
	GetNPrintFepInfo(STR_OSA, sFepId, AP01);
	GetNPrintFepInfo(STR_BXS, sFepId, AP01);
	GetNPrintFepInfo(STR_BXR, sFepId, AP01);
	GetNPrintFepInfo(STR_BXA, sFepId, AP01);
	GetNPrintFepInfo(STR_BSA, sFepId, AP01);

	GetNPrintFepInfo(STR_OFS, sFepId, AP02);
	GetNPrintFepInfo(STR_OFR, sFepId, AP02);
	GetNPrintFepInfo(STR_OIS, sFepId, AP02);
	GetNPrintFepInfo(STR_OIR, sFepId, AP02);
	GetNPrintFepInfo(STR_OXS, sFepId, AP02);
	GetNPrintFepInfo(STR_OXR, sFepId, AP02);
	GetNPrintFepInfo(STR_OXA, sFepId, AP02);
	GetNPrintFepInfo(STR_OSA, sFepId, AP02);
	GetNPrintFepInfo(STR_BXS, sFepId, AP02);
	GetNPrintFepInfo(STR_BXR, sFepId, AP02);
	GetNPrintFepInfo(STR_BXA, sFepId, AP02);
	GetNPrintFepInfo(STR_BSA, sFepId, AP02);

	if (giDispCnt == 0)
	{
		if (1) printf(gsDispStr
			, ""
			, gsFepId
			, "-"
			, "-"
			, "-"
			, "-"
			, "-"
			, "-"
			, "-"
			, "-"
			, ""
			, gsComment
			, ""
			, ""
			);
	}

	return 0;
}

/*============================================================================*/
/*
 *  Function     : main
 *  Arguments    : int argc, char *argv[]
 *  Return Type  : int
 *  Return Value :
 *  Description  : Main Function
 */
int main(int argc, char* argv[])
{
	int iRet = 0;

	if(argc < ARGNO)
	{
		if (1) printf("USAGE : %s [FEPID] [COMMENT]\n"
			"\tFEPID    : FEPID defined by FEP Server\n"
			"\tCOMMENT  : FEPID information\n"
			"\tex)\n"
			"\t\t%s KBB01\n\n"
			, argv[ARGNO_PROGNAME]
			, argv[ARGNO_PROGNAME]);

		exit(-1);
	}

	/* Validation Check for arguments */
	iRet = ValidateArgument(argc, argv);
	if (iRet < 0) return iRet;

	/* Open the files of FEP state */
	iRet = OpenFepStatFile();
	if (iRet < 0) return iRet;

	/* Process to get the information of the FEPID */
	iRet = Service(argc, argv);
	if (iRet < 0) return iRet;

	/* Close the files of FEP stat */
	iRet = CloseFepStatFile();
	if (iRet < 0) return iRet;

	return 0;
}   /* end of main() */

