/*
 * PROGRAM ID   : Time.c
 * PROGRAM NAME : �ð��� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : �ð��� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 *     �����Լ� : asctime, ctime, gmtime, localtime, mktime
 *                asctime_r, ctime_r, gmtime_r, localtime_r
 *                date(1), gettimeofday(2), time(2), utime(2), clock(3), 
 *                difftime(3), strftime(3), strptime(3), timegm(3), tzset(3),
 *                time(7)
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define MAX_STRTIME   50

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * ���� : YYYYMMDD hh:mm:ss.SSSSSS ���
 */
/*----------------------------------------------------------------------------*/
char* TimeGetDateTime()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYYMMDDhhmmssSSSSSS");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d%02d%02d%02d%02d%02d%06d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : YYYYMMDD hh:mm:ss.SSSSSS ���
 */
/*----------------------------------------------------------------------------*/
char* TimeGetDateTime2()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYYMMDD hh:mm:ss.SSSSSS");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d%02d%02d %02d:%02d:%02d.%06d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : YYYY/MM/DD hh:mm:ss.SSSSSS ���
 */
/*----------------------------------------------------------------------------*/
char* TimeGetDateTime3()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYY/MM/DD hh:mm:ss.SSSSSS");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d/%02d/%02d %02d:%02d:%02d.%06d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : YYYYMMDD ���
 */
/*----------------------------------------------------------------------------*/
char* TimeGetDate()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYYMMDD");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d%02d%02d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : YYYY/MM/DD ���
 */
/*----------------------------------------------------------------------------*/
char* TimeGetDate2()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYY/MM/DD");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d/%02d/%02d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : fcntl �Լ��� ȣ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* TimeGetTime()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "hhmmss");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%02d%02d%02d"
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : fcntl �Լ��� ȣ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* TimeGetTime2()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "hh:mm:ss");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%02d:%02d:%02d"
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : fcntl �Լ��� ȣ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* TimeGetLTime()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "hhmmssSSSSSS");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%02d%02d%02d%06d"
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : fcntl �Լ��� ȣ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* TimeGetLTime2()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "hh:mm:ss.SSSSSS");
		}
		else
		{
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%02d:%02d:%02d.%06d"
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : 
 *    mktime�� �̿��Ͽ� ����ϴ� ����� ����.
 */
/*----------------------------------------------------------------------------*/
char* TimeAddDay(int iDay)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;

		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYYMMDDhhmmssSSSSSS");
		}
		else
		{
			*((time_t *)&stTimeVal) += iDay * 24 * 60 * 60;
			
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d%02d%02d%02d%02d%02d%06d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : 
 *    mktime�� �̿��Ͽ� ����ϴ� ����� ����.
 */
/*----------------------------------------------------------------------------*/
char* TimeAddHour(int iHour)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;

		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			sprintf(strTime, "YYYYMMDDhhmmssSSSSSS");
		}
		else
		{
			*((time_t *)&stTimeVal) += iHour * 60 * 60;
			
			usec = stTimeVal.tv_usec;
			curr = localtime_r((time_t*)&stTimeVal, &stCurr);
			
			sprintf(strTime, "%04d%02d%02d%02d%02d%02d%06d"
				, curr->tm_year + 1900
				, curr->tm_mon + 1
				, curr->tm_mday
				, curr->tm_hour
				, curr->tm_min
				, curr->tm_sec
				, usec
				);
		}
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : ������ ���Ѵ�. 0:�Ͽ��� ..... 6:�����
 */
/*----------------------------------------------------------------------------*/
int TimeGetWeek()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int weekNo = -1;
		
	if (flag)
	{
		time_t timer;
		struct tm t;
		
		timer =time(NULL);                     /* ���� �ð��� �ʴ����� ��´�. */
		localtime_r(&timer, &t);  /* �ʴ����� �ð��� �и��Ͽ� ����ü�� �ִ´�. */
		
		if (!flag)
		{
			printf("UNIX TIME : %d sec\n", timer); /* from 1970/01/01 00:00:00 */
			printf("CURR YEAR : %04d\n", t.tm_year + 1900);
			printf("CURR MON  : %02d\n", t.tm_mon + 1);
			printf("CURR DAY  : %02d\n", t.tm_mday);
			printf("CURR HOUR : %02d\n", t.tm_hour);
			printf("CURR MIN  : %02d\n", t.tm_min);
			printf("CURR SEC  : %02d\n", t.tm_sec);
			printf("CURR WEEK : %d\n", t.tm_wday);
		}
		
		weekNo = t.tm_wday;
	}

	return weekNo;
}

/*============================================================================*/
/*
 * ���� : fcntl �Լ��� ȣ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* TimeGetYear()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			return NULL;
		}
		
		usec = stTimeVal.tv_usec;
		curr = localtime_r((time_t*)&stTimeVal, &stCurr);
		
		sprintf(strTime, "%04d"
			, curr->tm_year + 1900
			);
	}

	return strTime;
}

/*============================================================================*/
/*
 * ���� : fcntl �Լ��� ȣ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* TimeSample()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static char strTime[MAX_STRTIME];
	
	if (flag)
	{
		struct timeval  stTimeVal;
		struct timezone stTimeZone;
		struct tm*      curr;
		struct tm       stCurr;
		int    usec;
		
		if (gettimeofday((struct timeval*) &stTimeVal, &stTimeZone))
		{
			return NULL;
		}
		
		usec = stTimeVal.tv_usec;
		curr = localtime_r((time_t*)&stTimeVal, &stCurr);
		
		sprintf(strTime, "%04d/%02d/%02d %02d:%02d:%02d.%06d"
			, curr->tm_year + 1900
			, curr->tm_mon + 1
			, curr->tm_mday
			, curr->tm_hour
			, curr->tm_min
			, curr->tm_sec
			, usec
			);
	}

	return strTime;
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
	
	if (flag) printf("\x1b[44m< %s >\x1b[0m\n", sProgName);

	if (flag)
	{
		printf("* Part-1\n");
		printf("TimeGetDateTime  >[%s]\n", TimeGetDateTime());
		printf("TimeGetDateTime2 >[%s]\n", TimeGetDateTime2());
		printf("TimeGetDateTime3 >[%s]\n", TimeGetDateTime3());
		printf("TimeGetDate      >[%s]\n", TimeGetDate());
		printf("TimeGetDate2     >[%s]\n", TimeGetDate2());
		printf("TimeGetTime      >[%s]\n", TimeGetTime());
		printf("TimeGetTime2     >[%s]\n", TimeGetTime2());
		printf("TimeGetLTime     >[%s]\n", TimeGetLTime());
		printf("TimeGetLTime2    >[%s]\n", TimeGetLTime2());
		printf("TimeAddDay       >[%s]\n", TimeAddDay(-10));
		printf("TimeAddHour      >[%s]\n", TimeAddHour(-23));
		printf("TimeGetWeek      >[%d]\n", TimeGetWeek());
		printf("TimeGetYear      >[%s]\n", TimeGetYear());
	}
	
	if (flag)
	{
		printf("* Part-2\n");
		printf("TimeGetDateTime  >[%4.4s]\n", TimeGetDateTime() + 4);
	}
}

#endif
