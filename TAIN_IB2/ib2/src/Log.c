/*
 * PROGRAM ID   : Log.c
 * PROGRAM NAME : 로그처리 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : 로그처리 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   MAX_MSG_SIZE      10240

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

void (*LOG) (const char* sLogFile, const char* sFile, const char* sFunc, int iLine, int iLevel, int iCode, const char* pMsg, ... ) = LOG_DEFAULT;

/*============================================================================*/
/*
 * 내용 : LOG 를 func으로 세팅한다.
 */
/*----------------------------------------------------------------------------*/
void SETLOG( void (*func) (const char* sLogFile, const char* sFile, const char* sFunc, int iLine, int iLevel, int iCode, const char* pMsg, ... ) )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		LOG = func;
	}
}

/*============================================================================*/
/*
 * 내용 : 순차적인 값을 리턴한다.
 */
/*----------------------------------------------------------------------------*/
void LOG_DEFAULT(const char* sLogFile, const char* sFile, const char* sFunc, int iLine, int iLevel, int iCode, const char* pMsg, ... )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	char sBuf[MAX_MSG_SIZE];
	char sLogLevel[2];
	int iLogLevel = LOG_LEVEL_DEBUG;  /* Log Level */
	
	if (flag)
	{
		FILE * fp = stdout;
		
		if (flag)
		{
			/*
			 * VArg 메시지처리를 한다.
			 */
			 
			va_list VaList;
			va_start(VaList, pMsg);
			vsprintf(sBuf, pMsg, VaList);
			va_end(VaList);
			
			if (!flag) LOG(_FL_, 0, 0, "[%s]", sBuf);
		}
            			
		if (flag)
		{
		    /*
             *  LOG_LEVEL_DEBUG   0 (Biz 에서 요청하는 모든 것을 출력)
             *  LOG_LEVEL_INFO    5 (Biz 의 정보~에러만 출력한다     )
             *  LOG_LEVEL_WARNING 8 (Biz 의 경고~에러만 출력한다     )
             *  LOG_LEVEL_ERROR   9 (Biz 의 에러만 출력한다          )
             */
		    if (strcmp("DEBUG", CfgGetBiz("LOG_LEVEL")) == 0)
		    {
		    	iLogLevel = 0;
			} else if (strcmp("INFO", CfgGetBiz("LOG_LEVEL")) == 0) {
				iLogLevel = 5;
            } else if (strcmp("WARNING", CfgGetBiz("LOG_LEVEL")) == 0) {
				iLogLevel = 8;
			} else if (strcmp("ERROR", CfgGetBiz("LOG_LEVEL")) == 0) {
				iLogLevel = 9;
			}				
	    }

		if (flag)
		{
            switch( iLogLevel )
            {
                case LOG_LEVEL_ERROR   : if( iLevel < 9 ) return; break;
                case LOG_LEVEL_WARNING : if( iLevel < 8 ) return; break;
                case LOG_LEVEL_INFO    : if( iLevel < 5 ) return; break;
            	default: /*  DEBUG인 경우 모두 출력한다. */       break;
            }
        }
        
        if (flag)
		{
            switch( iLevel )
            {
                case LOG_LEVEL_ERROR   : sprintf(sLogLevel, "%s", "ER"); break;
                case LOG_LEVEL_WARNING : sprintf(sLogLevel, "%s", "WA"); break;
                case LOG_LEVEL_INFO    : sprintf(sLogLevel, "%s", "IN"); break;
            	default:                 sprintf(sLogLevel, "%s", "DE"); break;
            }
        }
    
		
		if (flag)
		{
			/*
			 * 로그를 출력할 stream을 세팅한다.
			 */
			 
			if (sFile != NULL)
			{
				/* 파일 오픈 */
			}
			
			if (flag)
			{
#if 0
				fprintf(fp, "%s [%s:%s:%d:%d](%d) %s\n", TimeGetDateTime2(), sFile, sFunc, iLine, iLevel, iCode, sBuf);
#else
                /* DATE.2014.04.04 : (JUN) 서비스ID 출력 */
				fprintf(fp, "%s %s [%-10.10s:%-20.20s:%4d:%2.2s](%d) %s\n", TimeGetDateTime2(), sServiceid, sFile, sFunc, iLine, sLogLevel, iCode, sBuf);
#endif
				fflush(fp);
			}
			
			if (sFile != NULL)
			{
				/* 파일 클로우즈 */
			}
		}
	}
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
		LOG(_FL_, 0, 0, ": this is the program for the testing<%d:%s>", 10, "TEST");
		LOG(_FL_, 0, 0, ": this is the program for the testing<%d:%s>", 20, "TEST");
		LOG(_FL_, 0, 0, ": this is the program for the testing<%d:%s>", 30, "TEST");
		LOG(_FL_, 0, 0, ": this is the program for the testing<%d:%s>", 40, "TEST");
		}

}

#endif
