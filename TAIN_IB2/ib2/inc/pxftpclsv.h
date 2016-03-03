/* Global Variable Declare */
pthread_mutex_t   gstTrdMtx;
pthread_mutex_t   gstSokMtx;

char  gsProcName[20+1];
char  gsWorkDate[ 8+1];
char  gsIpAddr  [15+1];
char  gsIpAddr  [15+1];
int   giPort;
char  gcSubDir   = 'Y';
int   giMaxCon   =  1 ;
int   giWaitTime = 10 ;
char  gcLogLevel = 'I';
char *gsLogHeader = "%h:%m:%s.%S";
char *gsFileDir;
char *gsSuccDir;
char *gsSuccList;
char *gsLogFile;
char *gssCommand[10];
FILE *gfpSucc;
FILE *gfpLog;
WORK_INFO *gstWork;
int        giWorkCnt;

