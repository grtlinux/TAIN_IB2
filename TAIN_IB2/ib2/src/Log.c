/*
 * PROGRAM ID   : Log.c
 * PROGRAM NAME : �α�ó�� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : �α�ó�� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
 * ���� : LOG �� func���� �����Ѵ�.
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
 * ���� : �������� ���� �����Ѵ�.
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
			 * VArg �޽���ó���� �Ѵ�.
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
             *  LOG_LEVEL_DEBUG   0 (Biz ���� ��û�ϴ� ��� ���� ���)
             *  LOG_LEVEL_INFO    5 (Biz �� ����~������ ����Ѵ�     )
             *  LOG_LEVEL_WARNING 8 (Biz �� ���~������ ����Ѵ�     )
             *  LOG_LEVEL_ERROR   9 (Biz �� ������ ����Ѵ�          )
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
            	default: /*  DEBUG�� ��� ��� ����Ѵ�. */       break;
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
			 * �α׸� ����� stream�� �����Ѵ�.
			 */
			 
			if (sFile != NULL)
			{
				/* ���� ���� */
			}
			
			if (flag)
			{
#if 0
				fprintf(fp, "%s [%s:%s:%d:%d](%d) %s\n", TimeGetDateTime2(), sFile, sFunc, iLine, iLevel, iCode, sBuf);
#else
                /* DATE.2014.04.04 : (JUN) ����ID ��� */
				fprintf(fp, "%s %s [%-10.10s:%-20.20s:%4d:%2.2s](%d) %s\n", TimeGetDateTime2(), sServiceid, sFile, sFunc, iLine, sLogLevel, iCode, sBuf);
#endif
				fflush(fp);
			}
			
			if (sFile != NULL)
			{
				/* ���� Ŭ�ο��� */
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
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
