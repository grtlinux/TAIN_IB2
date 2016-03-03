/*
 * PROGRAM ID   : Util.c
 * PROGRAM NAME : 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 공통적으로 사용하는 함수들을 정의한다.
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

/*============================================================================*/
/*
 * 내용 : 문자열의 앞뒤 공백을 제거한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilTrim( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p;
		char* q;
		int len = strlen(str);

		if (!flag) LOG(_FL_, 0, 0, "%s-0> [len=%d][%s]", __FUNCTION__, len, str);

		if (len > 0)  /* DATE.2013.11.26 수정추가 */
		{
			/* 뒤 공백처리 */
			for (p = str + len-1; isspace(*p); p--);
			p++;
			*p = 0x00;
	
			if (!flag) LOG(_FL_, 0, 0, "%s-1> [%s]", __FUNCTION__, str);
	
			/* 앞 공백처리 */
			for (p = str; isspace(*p); p++);
	
			if (!flag) LOG(_FL_, 0, 0, "%s-2> [%s]", __FUNCTION__, p);
	
			/* 자리를 옮김 */
			for (q = str; *p != 0x00; p++, q++) {
				*q = *p;
			}
			*q = *p;
	
			if (!flag) LOG(_FL_, 0, 0, "%s-3> [%s]", __FUNCTION__, str);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 문자열의 앞뒤 [ ] 을 제거한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilDelBracket( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* p;
		char* q;
		int len = strlen(str);

		if (!flag) LOG(_FL_, 0, 0, "%s-0> [len=%d][%s]", __FUNCTION__, len, str);

		/* 뒤 공백처리 */
		for (p = str + len-1; isspace(*p) || *p == ']' || *p == '}' || *p == ')'; p--);
		p++;
		*p = 0x00;

		if (!flag) LOG(_FL_, 0, 0, "%s-1> [%s]", __FUNCTION__, str);

		/* 앞 공백처리 */
		for (p = str; isspace(*p) || *p == '[' || *p == '{' || *p == '('; p++);

		if (!flag) LOG(_FL_, 0, 0, "%s-2> [%s]", __FUNCTION__, p);

		/* 자리를 옮김 */
		for (q = str; *p != 0x00; p++, q++) {
			*q = *p;
		}
		*q = *p;

		if (!flag) LOG(_FL_, 0, 0, "%s-3> [%s]", __FUNCTION__, str);
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 문자열의 앞뒤 공백을 제거한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilDelComment( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p = str;

		/* # 문자를 찾아 0x00 문자로 바꾼다. */
		while (*p)
		{
			if (*p == '#')
			{
				*p = 0x00;
				break;
			}

			p++;
		}

		if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, str);
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 문자열을 특수문자로 자른다.
 */
/*----------------------------------------------------------------------------*/
char** UtilStrSplit( char* str, char ch, char** arrItem )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		int index = 0;
		char* p = str;

		arrItem[index++] = p;

		/* # 문자를 찾아 0x00 문자로 바꾼다. */
		while (*p)
		{
			if (*p == ch)
			{
				*p = 0x00;
				if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, arrItem[index-1]);

				arrItem[index++] = p+1;
			}

			p++;
		}

		if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, arrItem[index-1]);

		arrItem[index] = NULL;

		if (!flag) LOG(_FL_, 0, 0, "%s> [str=%s]", __FUNCTION__, str);
	}

	return arrItem;
}

/*============================================================================*/
/*
 * 내용 : /proc/[PID]/cmdline 의 내용을 얻는다.
 */
/*----------------------------------------------------------------------------*/
int UtilGetCmdLine( char* str, char** arr )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int index = 0;

	if (flag) {
		char* p = str;

		arr[index++] = p;

		/* # 문자를 찾아 0x00 문자로 바꾼다. */
		while (p[0] || p[1])
		{
			if (p[0] == 0x00)
			{
				if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, arr[index-1]);

				arr[index++] = p + 1;
			}

			p++;
		}

		if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, arr[index-1]);

		arr[index] = NULL;

		if (!flag) LOG(_FL_, 0, 0, "%s> [str=%s]", __FUNCTION__, str);
	}

	return index;
}

/*============================================================================*/
/*
 * 내용 : 문자열을 args 형태로 만든다.
 *    0x09 : TAB
 *    0x20 : SPACE
 *    0x22 : "
 *    0x27 : '
 *
 *    ARGS :     a aaa "  c  dddddd   a"   123
 */
/*----------------------------------------------------------------------------*/
char** UtilGetArgs(char* str, char** args)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int flag = 1;
		int index = 0;
		char* p = str;

		while (*p)
		{
			if (*p == 0x20 || *p == 0x09)  /* SPACE, TAB */
			{
				*p = 0x00;
				p++;
				flag = 1;
				continue;
			}

			if (*p == 0x22)   /* " */
			{
				*p = 0x00;
				args[index ++] = ++p;

				while (*p)
				{
					if (*p == 0x22)
					{
						*p = 0x00;
						p++;
						flag = 1;
						break;
					}
					p++;
				}

				continue;
			}

			if (*p == 0x27)   /* ' */
			{
				*p = 0x00;
				args[index ++] = ++p;

				while (*p)
				{
					if (*p == 0x27)
					{
						*p = 0x00;
						p++;
						flag = 1;
						break;
					}
					p++;
				}

				continue;
			}

			if (flag == 1)
			{
				args[index ++] = p;
				p++;
				flag = 0;
				continue;
			}

			p++;
		}

		args[index] = NULL;
	}

	return args;
}

/*============================================================================*/
/*
 * 내용 : 문자열의 소문자를 대문자로 바꾼다.
 */
/*----------------------------------------------------------------------------*/
char* UtilToUpper( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p = str;

		/* 대문자로 변환 */
		while (*p)
		{
			*p = toupper(*p);

			p++;
		}

		if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, str);
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 문자열의 대문자를 소문자로 바꾼다.
 */
/*----------------------------------------------------------------------------*/
char* UtilToLower( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p = str;

		/* 소문자로 변환 */
		while (*p)
		{
			*p = tolower(*p);

			p++;
		}

		if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, str);
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 숫자형문자열을 Integer 숫자로 변환한다.
 */
/*----------------------------------------------------------------------------*/
int UtilToInt( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret = 0;

	if (flag) {
		ret = atoi(str);
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : N bytes 숫자형문자열을 Integer 숫자로 변환한다.
 */
/*----------------------------------------------------------------------------*/
int UtilToIntN( char* str, int n )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret = 0;

	if (flag) {
		char buf[50];
		sprintf(buf, "%.*s", n, str);

		ret = atoi(buf);
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : 숫자형문자열을 Long 숫자로 변환한다.
 */
/*----------------------------------------------------------------------------*/
long UtilToLong( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	long ret = 0;

	if (flag) {
		ret = atol(str);
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : N bytes 숫자형문자열을 Long 숫자로 변환한다.
 */
/*----------------------------------------------------------------------------*/
long UtilToLongN( char* str, int n )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	long ret = 0;

	if (flag) {
		char buf[50];
		sprintf(buf, "%.*s", n, str);

		ret = atol(buf);
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : N bytes 숫자형문자열을 Long 숫자로 변환한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilFillCopy(char* tgt, char* src, int size, char ch )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int len = strlen(src);

		if (len < size)
		{
			/* ch로 채운다 */
			memset(tgt, ch, size);
			tgt[size] = 0x00;
			memcpy(tgt, src, len);
		}
		else
		{
			/* 일부만 복사한다. */
			strncpy(tgt, src, size);
			tgt[size] = 0x00;
		}
	}

	return tgt;
}

/*============================================================================*/
/*
 * 내용 : YYYYMMDD를 날짜로 세팅한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilSetYYYYMMDD(char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sDate = TimeGetDate();
		char* p = strstr(str, "YYYYMMDD");

		if (p != NULL)
		{
			memcpy(p, sDate, 8);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : FEPID를 세팅한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilSetFEPID(char* str, char* sFepId)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* p = strstr(str, "FEPID");

		if (p != NULL)
		{
			memcpy(p, sFepId, 5);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : MSGID를 세팅한다.
 */
/*----------------------------------------------------------------------------*/
char* UtilSetMSGID(char* str, char* sMsgId)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* p = strstr(str, "XIMSGID");

		if (p != NULL)
		{
			memcpy(p, sMsgId, 7);
		}
	}

	return str;
}

/*============================================================================*/
/*
 * 내용 : 메모리 내용을 앞으로 이동한다.
 */
/*----------------------------------------------------------------------------*/
int UtilMemMove(char* dst, char* src, int n)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* d = dst;
		char* s = src;

		int i;
		for (i=0; i < n; i++, d++, s++)
		{
			*d = *s;
		}
	}

	return n;
}

/*============================================================================*/
/*
 * 내용 : 문자열들을 비교
 */
/*----------------------------------------------------------------------------*/
int UtilTest01()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* fepids[] = {
			"1NHB", "1KBB01", "2LTC02", "1SSC"
		};
		int fepidCnt = sizeof(fepids) / sizeof(char*);

		printf("sizeof(fepids) = %d\n", fepidCnt);

		int i;
		for (i=0; i < fepidCnt; i++)
		{
			printf("(%d) = %c[%d:%s]\n", i, *fepids[i], strlen(fepids[i]+1), fepids[i]+1);
		}
	}

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

	if (!flag)
	{
		/* UtilTrim 을 테스트한다. */
		/* 문자열 상수는 변경할 수 없다. 고정임. */

		char strTest[100];

		strcpy(strTest, "  \t \f  12 34 가나다 \n  \n\n     ");

		LOG(_FL_, 0, 0, "[%s]", strTest);
		UtilTrim(strTest);
		LOG(_FL_, 0, 0, "[%s]", strTest);
	}

	if (!flag)
	{
		/* comment 문자 '#' 이후를 자른다. */
		char strTest[100];

		strcpy(strTest, "  1   2  #  34   5 6 ");

		LOG(_FL_, 0, 0, "[%s]", strTest);
		UtilDelComment(strTest);
		LOG(_FL_, 0, 0, "[%s]", strTest);
	}

	if (!flag)
	{
		/* 문자열 사이에 있는 '|' 문자로 자른다. */
		char strTest[100];
		char* arrItem[20];
		int i;

		strcpy(strTest, "  1 |  2 | # | 34 |  5 6| |");

		LOG(_FL_, 0, 0, "[%s]", strTest);
		UtilStrSplit(strTest, '|', arrItem);
		for (i=0; i<20 && arrItem[i] != NULL; i++)
		{
			LOG(_FL_, 0, 0, "%d) [%s]", i, arrItem[i]);
		}
	}

	if (!flag)
	{
		/* 숫자형 문자열 변환 */
		char* str = "1234567890";

		LOG(_FL_, 0, 0, "UtilToInt -> [%d]", UtilToInt(str));
		LOG(_FL_, 0, 0, "UtilToInt -> [%d]", UtilToIntN(str, 3));
	}

	if (!flag)
	{
		char* str = "  123 4 ";
		LOG(_FL_, 0, 0, "[%s][%d]", str, atoi(str));
		LOG(_FL_, 0, 0, "UtilToInt -> [%d]", UtilToInt(str));
		LOG(_FL_, 0, 0, "UtilToInt -> [%d]", UtilToIntN(str, 4));
	}

	if (!flag)
	{
		char str[200];
		char* org = "12345 ABCDE jkldk du3kdv,DAfa";

		strcpy(str, org);
		LOG(_FL_, 0, 0, "[%s] UtilToUpper [%s]", org, UtilToUpper(str));

		strcpy(str, org);
		LOG(_FL_, 0, 0, "[%s] UtilToLower [%s]", org, UtilToLower(str));
	}

	if (!flag)
	{
		char* src = "012345678901234567890";
		char tgt[100];

		UtilFillCopy(tgt, src, 50, '*');
		printf("tgt[%s]\n", tgt);

		UtilFillCopy(tgt, src, 10, '0');
		printf("tgt[%s]\n", tgt);
	}

	if (!flag)
	{
		UtilTest01();
	}

	if (!flag)
	{
		char buf[100];

		strcpy(buf, "/hw01/ibridge/log/YYYYMMDD/error.log");

		UtilSetYYYYMMDD(buf);
		printf("[%s]\n", buf);
	}

	if (!flag)
	{
		char buf[4000] = { 0x00 };
		printf("buf=[%s]\n", buf);

		int i;
		for (i=0; i < 10; i++)
		{
			strcat(buf, "1234567890");
			printf("%d) buf=[%s]\n", i, buf);
		}
	}

	if (!flag)
	{
		char* str = "나는";
		printf("s       [%s]\n", str);
		printf("10s     [%10s]\n", str);
		printf("010s    [%-010s]\n", str);
		printf("010.10s [%010.10s]\n", str);
	}

	if (!flag)
	{
		char* str = "1234567890";
		printf("str= [%s]\n", str);
		printf("5s      [%5s]\n", str);
		printf("-5.5s   [%-5.5s]\n", str);
		printf("-20.20s [%-20.20s]\n", str);
		printf("20.5s   [%20.5s]\n", str);
	}

	if (!flag)
	{
		char str[100];

		strcpy(str, "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");

		UtilMemMove(str, str+10, 20);

		printf("str= [%s]\n", str);
	}

	if (!flag)
	{
		char buf[100];

		strcpy(buf, "   [  1234; 56   ;543     ] ");
		UtilDelBracket(buf);

		printf("[%s]\n", buf);
	}

	if (!flag)
	{
		char buf[100];

		strcpy(buf, "/hw01/ibridge/log/YYYYMMDD/error.log");
		UtilSetYYYYMMDD(buf);

		char* p = rindex(buf, '/');
		*p = 0x00;

		printf("[%s]\n", buf);
	}

	if (flag)
	{
		char buf[128];
		char* args[100];

		strcpy(buf, "a    aaa     \"  'c'  dddddd   a\"   123  '      '   ' 123  \"543\"   4888 '   ");
		if (flag) printf("[%s]\n", buf);

		if (flag) UtilGetArgs(buf, args);

		int i;
		for (i = 0; args[i]; i++)
		{
			printf("(%d) [%s]\n", i, args[i]);
		}
	}
}

#endif
