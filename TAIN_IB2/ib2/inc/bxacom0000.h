/*
 *  PROGRAM ID      : bxacom0000.h
 *  PROGRAM NAME    : 대외 배치 공통
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 */

#include <bxa_config.h>     /* Config  Structure */

#ifndef _BXACOM0000_H_
#define _BXACOM0000_H_

FILE    *gpstFile;
int      giSockFd;          /* listen port */
char     gsLogPath[ 99+1];
char     gsLogName[ 20+1];

char     gsFilePath[50+1];
char     gsFileName[30+1];
char     gsFullPath[80+1];

int      giPrevMsgInf;
char     gsPrevBuffer[4200];

static long glTotSize = 0;
static long glSdRvSum = 0;
static int  giSeqNo   = 0;
static int  giBlockNo = 0;
static int  giRelByte = 0; /* 실   Data Byte */

/*
extern   int      giSigCode;
extern   double   gdTotSize;
extern   double   gdGetTime;
extern   char     gsBizTime[ 20+1];
extern   int      giTstCnt;
extern   int      giMisCnt;
extern   char     gsMisBit [100+1];
*/

/*
 *  공통 Utility
 */
extern   int      LoadInfo( char * );
extern   int      GetCommInfo( char *, char * );
extern   int      GetFileInfo( char *, char * );
extern   int      ChkWorkFile( char *, char * );
extern   int      PutWorkStat();
extern   int      GetWorkFile( char *, double * );
extern   int      GetListFile( char *, char * );
extern   int      SetListFile( char *, char *, char *, char * );
extern   char    *CheckFile( char *, char *, long * );
extern   int      SendFtpMain( char *, char *, char * );

/*
 *  공통 Library
 */
extern   char    *SYSDTM();
extern   char    *SYSDTMBFR();
extern   char    *CnvI2A( int iData, int iSize );
extern   int      CnvA2I( char *sData, int iSize );
extern   long     CnvA2L( char *sData, int iSize );
extern   double   CnvA2D( char *sData, int iSize );
extern   void     R_TRIM( char *sTrmVal );
extern   void     InitProc( int, char *[] );
extern   void     ExitProc( char *sProcName );
extern   void     WrtLog( char *sFile, int iLine, char *sLogMsg, ... );
extern   void     WrtErr( char *sFile, int iLine, char *sLogMsg, ... );
extern   void     SignalHandler( int iCode );
extern   void     HandleSignal();
extern   double   GetModifyTime( char * );
extern   long     GetFileSize( char * );
extern   int      deleteCrLf( char *, int );
extern   int      appendCrLf( char *, int );

extern   long     repString( char *, char *, char * );
extern   char    *getInfoValue( char *, char *, char * );

#endif
