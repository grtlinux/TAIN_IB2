/*
 * PROGRAM ID   : Util.c
 * PROGRAM NAME : ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ���������� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
 * ���� : ���ڿ��� �յ� ������ �����Ѵ�.
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

		if (len > 0)  /* DATE.2013.11.26 �����߰� */
		{
			/* �� ����ó�� */
			for (p = str + len-1; isspace(*p); p--);
			p++;
			*p = 0x00;
	
			if (!flag) LOG(_FL_, 0, 0, "%s-1> [%s]", __FUNCTION__, str);
	
			/* �� ����ó�� */
			for (p = str; isspace(*p); p++);
	
			if (!flag) LOG(_FL_, 0, 0, "%s-2> [%s]", __FUNCTION__, p);
	
			/* �ڸ��� �ű� */
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
 * ���� : ���ڿ��� �յ� [ ] �� �����Ѵ�.
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

		/* �� ����ó�� */
		for (p = str + len-1; isspace(*p) || *p == ']' || *p == '}' || *p == ')'; p--);
		p++;
		*p = 0x00;

		if (!flag) LOG(_FL_, 0, 0, "%s-1> [%s]", __FUNCTION__, str);

		/* �� ����ó�� */
		for (p = str; isspace(*p) || *p == '[' || *p == '{' || *p == '('; p++);

		if (!flag) LOG(_FL_, 0, 0, "%s-2> [%s]", __FUNCTION__, p);

		/* �ڸ��� �ű� */
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
 * ���� : ���ڿ��� �յ� ������ �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* UtilDelComment( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p = str;

		/* # ���ڸ� ã�� 0x00 ���ڷ� �ٲ۴�. */
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
 * ���� : ���ڿ��� Ư�����ڷ� �ڸ���.
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

		/* # ���ڸ� ã�� 0x00 ���ڷ� �ٲ۴�. */
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
 * ���� : /proc/[PID]/cmdline �� ������ ��´�.
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

		/* # ���ڸ� ã�� 0x00 ���ڷ� �ٲ۴�. */
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
 * ���� : ���ڿ��� args ���·� �����.
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
 * ���� : ���ڿ��� �ҹ��ڸ� �빮�ڷ� �ٲ۴�.
 */
/*----------------------------------------------------------------------------*/
char* UtilToUpper( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p = str;

		/* �빮�ڷ� ��ȯ */
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
 * ���� : ���ڿ��� �빮�ڸ� �ҹ��ڷ� �ٲ۴�.
 */
/*----------------------------------------------------------------------------*/
char* UtilToLower( char* str )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) {
		char* p = str;

		/* �ҹ��ڷ� ��ȯ */
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
 * ���� : ���������ڿ��� Integer ���ڷ� ��ȯ�Ѵ�.
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
 * ���� : N bytes ���������ڿ��� Integer ���ڷ� ��ȯ�Ѵ�.
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
 * ���� : ���������ڿ��� Long ���ڷ� ��ȯ�Ѵ�.
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
 * ���� : N bytes ���������ڿ��� Long ���ڷ� ��ȯ�Ѵ�.
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
 * ���� : N bytes ���������ڿ��� Long ���ڷ� ��ȯ�Ѵ�.
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
			/* ch�� ä��� */
			memset(tgt, ch, size);
			tgt[size] = 0x00;
			memcpy(tgt, src, len);
		}
		else
		{
			/* �Ϻθ� �����Ѵ�. */
			strncpy(tgt, src, size);
			tgt[size] = 0x00;
		}
	}

	return tgt;
}

/*============================================================================*/
/*
 * ���� : YYYYMMDD�� ��¥�� �����Ѵ�.
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
 * ���� : FEPID�� �����Ѵ�.
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
 * ���� : MSGID�� �����Ѵ�.
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
 * ���� : �޸� ������ ������ �̵��Ѵ�.
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
 * ���� : ���ڿ����� ��
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
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
		/* UtilTrim �� �׽�Ʈ�Ѵ�. */
		/* ���ڿ� ����� ������ �� ����. ������. */

		char strTest[100];

		strcpy(strTest, "  \t \f  12 34 ������ \n  \n\n     ");

		LOG(_FL_, 0, 0, "[%s]", strTest);
		UtilTrim(strTest);
		LOG(_FL_, 0, 0, "[%s]", strTest);
	}

	if (!flag)
	{
		/* comment ���� '#' ���ĸ� �ڸ���. */
		char strTest[100];

		strcpy(strTest, "  1   2  #  34   5 6 ");

		LOG(_FL_, 0, 0, "[%s]", strTest);
		UtilDelComment(strTest);
		LOG(_FL_, 0, 0, "[%s]", strTest);
	}

	if (!flag)
	{
		/* ���ڿ� ���̿� �ִ� '|' ���ڷ� �ڸ���. */
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
		/* ������ ���ڿ� ��ȯ */
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
		char* str = "����";
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
