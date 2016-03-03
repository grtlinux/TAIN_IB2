
/*
 *  1. 파 일 명 : Passwd.c
 *  2. 기    능 : KFTC 자동이체(CMS) 사용자 암호 암호화 및 복호화 모듈
 */

#include <Common.h>
/* #include <stdio.h>            */
/* #include <ctype.h>            */
/* #include <string.h>           */
/* #include <strings.h>          */
/* #include <time.h>             */
/* #include <sys/time.h>         */
/* #include <LibConstDefine.h>   */
/* #include <LibExtVarDefine.h>  */

/*
typedef struct _PASSWORD_I_
{
	int   iProcTp;       // 처리구분 1: 암호화 2: 복호화
	char  sUserId[20+1]; // 송신자명
	char  sUserPw[20+1]; // 송신자암호
	char  sBizYmd[20+1]; // 거래일
	char  sOrgzCd[20+1]; // 이용기관식별코드
	char  sEnCode[20+1]; // 송신자암호 암호문
}   PASSWORD_I;

typedef struct _PASSWORD_O_
{
	char  sUserPw[20+1]; // 송신자암호
	char  sEnCode[20+1]; // 송신자암호 암호문
}   PASSWORD_O;
*/


#define KFTC_CMS 1 /* 결제원 CMS      */
#define KFTC_KTB 2 /* 결제원 일괄전송 */

/*
 *  Modulus 값
 */
#define CMS_MODULUS  36

static char gsUserId[20+1]; /* 송신자명 + 나머지는 'Z'으로 채움 */
static char gsUserPw[16+1]; /* 송신자암호:6자리+나머지는 확장   */

static char sAlgebra[36] = { '9', '8', '7', '6', '5', '4', '3', '2', '1', '0',
							 'Z', 'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q',
							 'P', 'O', 'N', 'M', 'L', 'K', 'J', 'I', 'H', 'G',
							 'F', 'E', 'D', 'C', 'B', 'A' };

/*
 * sDtTm : YYYYMMDDhhmmssSSSSSS
 */
char *GetDtTm()
{
	static char sDtTm[20+1];
	/* GETDTM(sDtTm); */
	strcpy(sDtTm, TimeGetDateTime());
	return sDtTm;
}

char *GetPassword( char *sFepId, int iProcTp, char *sUserId, char *sPasswd, char *sOrgzCd )
{
	/*LOGPUT( _FL_, 0, 0, "fep_id=%s, proc_tp=%d, user_id=%s, passwd=%s, orgz_cd=%s", sFepId, iProcTp, sUserId, sPasswd, sOrgzCd );*/

	if (memcmp(sFepId, "KFT51", 5) == 0)
	{
		return GetKftcPasswrd( KFTC_KTB, iProcTp, sUserId, sPasswd, sOrgzCd );
	}
	else if (memcmp(sFepId, "KFT52", 5) == 0)
	{
		return GetKftcPasswrd( KFTC_CMS, iProcTp, sUserId, sPasswd, sOrgzCd );
	}
	else if (memcmp(sFepId, "KFT53", 5) == 0)
	{
		return GetKftcPasswrd( KFTC_CMS, iProcTp, sUserId, sPasswd, sOrgzCd );
	}
	else if (memcmp(sFepId, "KFT54", 5) == 0)
	{
		return GetKftcPasswrd( KFTC_CMS, iProcTp, sUserId, sPasswd, sOrgzCd );
	}
	else if (memcmp(sFepId, "KFT55", 5) == 0)
	{
		return GetKftcPasswrd( KFTC_CMS, iProcTp, sUserId, sPasswd, sOrgzCd );
	}
	else
	{
		" ";
	}
}   /* end of GetPassword */

/*
 *  함 수 명 : KftcPasswrd
 *  실 행 명 : 자동이체(CMS) 사용자 암호 암호화 및 복호화 모듈
 *  입력인자 : PASSWORD_I, PASSWORD_O
 */
char *GetKftcPasswrd( int iBizTp, int iProcTp, char *sUserId, char *sPasswd, char *sOrgzCd )
{
	static PASSWORD_I  stPwi;
	static PASSWORD_O  stPwo;

	memset(&stPwi, 0x00, sizeof(PASSWORD_I) );
	memset(&stPwo, 0x00, sizeof(PASSWORD_O) );
			stPwi.iProcTp = iProcTp;
	strcpy( stPwi.sUserId,  sUserId            );
	strcpy( stPwi.sUserPw,  stPwi.iProcTp == 1 ? sPasswd : "" );
	memcpy( stPwi.sBizYmd,  GetDtTm()+2,     6 );
	strcpy( stPwi.sOrgzCd,  sOrgzCd            );
	strcpy( stPwi.sEnCode,  stPwi.iProcTp == 2 ? sPasswd : "" );

#if 0
	LOG(_FL_, 0, 0, " " );
	LOG(_FL_, 0, 0, "   INPUT 처리구분=[%d]", stPwi.iProcTp );
	LOG(_FL_, 0, 0, "         송신자명=[%s]", stPwi.sUserId );
	LOG(_FL_, 0, 0, "       송신자암호=[%s]", stPwi.sUserPw );
	LOG(_FL_, 0, 0, "           거래일=[%s]", stPwi.sBizYmd );
	LOG(_FL_, 0, 0, "이용기관 식별코드=[%s]", stPwi.sOrgzCd );
	LOG(_FL_, 0, 0, "송신자 암호암호문=[%s]", stPwi.sEnCode );
#endif

	memset( gsUserId, 0x00, sizeof(gsUserId) );
	memset( gsUserPw, 0x00, sizeof(gsUserPw) );

	if( KftcValidation( iBizTp, &stPwi ) < 0 ) return NULL;

	switch( stPwi.iProcTp )
	{
		case  1: KftcEncryption( &stPwi, &stPwo ); break;
		case  2: KftcDecryption( &stPwi, &stPwo ); break;
		default: LOG(_FL_, 0, 0, "처리구분 오류 iProcTp=[%d]", stPwi.iProcTp ); return NULL;
	}

	return stPwi.iProcTp == 1 ? stPwo.sEnCode : stPwo.sUserPw;

}   /* end of GetKftcPasswrd() */

/*
 *  함 수 명 : KftcValidation
 *  기    능 : 입력값 확인
 *  입력인자 : PASSWORD_I, PASSWORD_O
 */
int KftcValidation( int iBizTp, PASSWORD_I *stPwi )
{
	int  i;

	if( stPwi->iProcTp != 1 && stPwi->iProcTp != 2 ) /* 1: 암호화 2: 복호화 */
	{
		LOG(_FL_, 0, 0, "처리구분 오류=[%d]", stPwi->iProcTp );
		return -1;
	}

	if( stPwi->sUserId == NULL )
	{
		LOG(_FL_, 0, 0, "송신자명 오류=[%s]", stPwi->sUserId );
		return -1;
	}

	if( stPwi->iProcTp == 1 && stPwi->sUserPw == NULL )
	{
		LOG(_FL_, 0, 0, "송신자암호 오류=[%s]", stPwi->sOrgzCd );
		return -1;
	}

	if( stPwi->sBizYmd == NULL )
	{
		LOG(_FL_, 0, 0, "거래일 오류=[%s]", stPwi->sBizYmd );
		return -1;
	}

	if( stPwi->sOrgzCd == NULL )
	{
		LOG(_FL_, 0, 0, "이용기관식별코드 오류=[%s]", stPwi->sOrgzCd );
		return -1;
	}

	if( stPwi->iProcTp == 2 && stPwi->sEnCode == NULL )
	{
		LOG(_FL_, 0, 0, "송신자암호 암호문 오류=[%s]", stPwi->sEnCode );
		return -1;
	}

/*
	if( strlen(stPwi->sUserId) != sizeof(stPwi->sUserId)-1 )
	{
		LOG(_FL_, 0, 0, "송신자명 오류 자리수(%2d자리) 부족=[%d]", sizeof(stPwi->sUserId)-1, strlen(stPwi->sUserId) );
		return -1;
	}
*/

	for( i = 0; i < 16; i++ )
	{
		if( isalpha( stPwi->sUserId[i]) || isdigit( stPwi->sUserId[i]) )
		{
			gsUserId[i] = stPwi->sUserId[i];
		}
		else
		if( isspace( stPwi->sUserId[i] ) || stPwi->sUserId[i] == 0x00 )
		{
			gsUserId[i] = 'Z';/* Key구성을 위해 Z으로 채움 */
		}
		else
		{
			LOG(_FL_, 0, 0, "송신자명 오류(영숫자만 가능)=[%s]", stPwi->sUserId );
			return -1;
		}
	}

	/*
	 *  송신자 암호 Check
	 */
	if( stPwi->iProcTp == 1 )
	{
#if 0
		if( strlen(stPwi->sUserPw) != sizeof(stPwi->sUserPw)-1 )
		{
			LOG(_FL_, 0, 0, "송신자암호 오류:자리수부족=[%d]", strlen(stPwi->sUserPw) );
			return -1;
		}
#endif

		for( i = 0; i < strlen(stPwi->sUserPw); i++ )
		{
			if( !isalpha( stPwi->sUserPw[i]) && !isdigit( stPwi->sUserPw[i]) )
			{
				LOG(_FL_, 0, 0, "송신자암호 오류(영숫자만 허용)=[%s]", stPwi->sUserPw );
				return -1;
			}
			gsUserPw[i] = stPwi->sUserPw[i];
		}

		/*
		 *  6자리 이후는 확장
		 */
		for( ; i < 16; i++ )
		{
			gsUserPw[i] = gsUserPw[i%6];
		}

		gsUserPw[i] = '\0'; /* NULL */

	}

	if( iBizTp == KFTC_KTB && stPwi->iProcTp != 2 ) return 0;

	/*
	 *  이용기관식별코드 Check
	 */
	for( i = 0; i < (iBizTp == KFTC_KTB ? 2 : 3); i++ )
	{
		if( !isalpha( stPwi->sOrgzCd[i]) && !isdigit( stPwi->sOrgzCd[i]) )
		{
			LOG(_FL_, 0, 0, "이용기관식별코드 오류=[%s]", stPwi->sOrgzCd );
			return -1;
		}
	}

	if( iBizTp == KFTC_CMS && stPwi->iProcTp != 2 ) return 0;

	/*
	 *  송신자 암호 암호문 Check
	 */
	for( i = 0; i < 16; i++ ) /* J3RY05F3D9R7RBJJ */
	{
		if( !isalpha(stPwi->sEnCode[i]) && !isdigit(stPwi->sEnCode[i]) )
		{
			LOG(_FL_, 0, 0, " 송신자암호 암호문 오류(영숫자만 허용)=[%s]", stPwi->sEnCode );
			return -1;
		}
	}

	return 0;
}

/*
 *  함 수 명 : KftcEncryption
 *  실 행 명 : 자동이체(CMS) 사용자 암호 암호화
 *  입력인자 : PASSWORD_I, PASSWORD_O
 */
int KftcEncryption( PASSWORD_I *stPwi, PASSWORD_O *stPwo )
{
	int  iCn[16] = {    0, };  /* 암호문의 대수치  */
	int  iPn[16] = {    0, };  /* 평문대수치       */
	int  iKn[16] = {    0, };  /* 암호키 대수치    */

	char sPn[16] = { 0x00, };  /* 송신자 암호 평문 */
	char sKn[16] = { 0x00, };  /* 암호키 영문      */
	char sCn[16] = { 0x00, };  /* 암호문 영문      */

	char cTemp;
	int  i, j;

	/*  << 암호화 알고리즘 >>
	 *  C1=(P1+K1) MOD M
	 *  C2=(P2+K2) MOD M
	 *      ......
	 *  Cn=(Pn+Kn) MOD M
	 *  여기서 C:암호문의 순열 (n=1...16)
	 *       P:평문의 순영(n=1...16)
	 *       K:암호키 영문의 순열(n=1...16)
	 *         기관코드2자리+거래일6자리(yyyymmdd)+전송자명 앞 8자리)
	 *       M:Modulus 36
	 */

	/*  << 전제조건 >>
	 *  1)대문자 영문과 숫자로만 구성하여 적용
	 *  2)키 및 적용데이타는 16자리로 운영
	 *  3)송신자암호는 6자리를 사용하고 암호평문 16자리로 확장한다
	 *  4)키구성시 전송자명이 8자리 미만이면 나머지는 영문자 "Z"로 채운다
	 *  5)대수값이 35이상이면 대수값 0부터 순환함
	 */

	/*
	 *  평문 대수치를 구함: 처음 6자리
	 */
	for( i = 0; i < 16; i++ )
	{
		cTemp  = islower(gsUserPw[i]) ? toupper(gsUserPw[i]) : gsUserPw[i];
		sPn[i] = cTemp;

		for( j = 0; j < 36; j++ )
		{
			if( cTemp != sAlgebra[j] ) continue;

			iPn[i] = j;
			break;
		}
	}

	/*
	 *  Key 대수치 구함
	 */
	if( strlen(stPwi->sOrgzCd) == 8 )
	{
		sprintf( sKn,"%c%c%.6s%.8s", stPwi->sOrgzCd[0], /* 기관코드          */
									 stPwi->sOrgzCd[7],
									 stPwi->sBizYmd,    /* 거래일 6자리      */
									 gsUserId );        /* 전송자명앞 8자리  */
	}
	else
	{
		sprintf( sKn,"%.2s%.6s%.8s", stPwi->sOrgzCd+1,  /* 기관코드          */
									 stPwi->sBizYmd,    /* 거래일 6자리      */
									 gsUserId );        /* 전송자명앞 8자리  */
	}

	for( i = 0; i < 16; i++ )
	{
		cTemp = islower(sKn[i]) ? toupper(sKn[i]) : sKn[i];

		for( j =  0; j < 36; j++ )
		{
			if( cTemp != sAlgebra[j] ) continue;
			iKn[i] = j;
			break;
		}
	};

	/*
	 *  암호문 대수치및 대수 값을 구함
	 */
	for( i = 0; i < 16; i++ )
	{
		iCn[i] = ( iPn[i] + iKn[i] ) % CMS_MODULUS;
		sCn[i] = sAlgebra[iCn[i]];
#if 0
		LOG(_FL_, 0, 0, ">>sPn[%d]:%c,iPn[%d]:%d | sKn[%d]:%c,iKn[%d]:%d => iCn[%d]:%d,sCn[%d]:%c ",
				   i, sPn[i], i, iPn[i], i, sKn[i], i, iKn[i] , i, iCn[i], i, sCn[i]);
#endif
	}

	memcpy( stPwo->sUserPw, gsUserPw, 16 );
	memcpy( stPwo->sEnCode, sCn,      16 );
	return 0;

}   /* end of KftcEncryption() */

/*
 *  함 수 명 : KftcDecryption
 *  실 행 명 : 자동이체(CMS) 사용자 암호 복호화
 *  입력인자 : PASSWORD_I, PASSWORD_O
 */
int KftcDecryption( PASSWORD_I *stPwi, PASSWORD_O *stPwo )
{
	int  iCn[16] = {    0, };  /* 암호문의 대수치  */
	int  iPn[16] = {    0, };  /* 평문대수치       */
	int  iKn[16] = {    0, };  /* 암호키 대수치    */

	char sPn[16] = { 0x00, };  /* 송신자 암호 평문 */
	char sKn[16] = { 0x00, };  /* 암호키 영문      */
	char sCn[16] = { 0x00, };  /* 암호문 영문      */

	char cTemp;
	int  i,j;

	/*  << 복호화 알고리즘 >>
	 *  P1=( (C1+M1) - K1 ) MOD M
	 *  P2=( (C2+M2) - K2 ) MOD M
	 *      ......
	 *  Pn=( (Cn+Mn) - Kn ) MOD M

	 *  여기서 C:암호문의 순열 (n=1...16)
	 *         P:평문의 순영(n=1...16)
	 *         K:암호키 영문의 순열(n=1...16)
	 *           기관코드2자리+거래일6자리(yyyymmdd)+전송자명 앞 8자리)
	 *         M:Modulus 36
	 */

	/*
	 *  암호문 대수치를 구함
	 */
	for( i = 0; i < 16; i++ )
	{
		cTemp = islower(stPwi->sEnCode[i]) ? toupper(stPwi->sEnCode[i]) : stPwi->sEnCode[i];
		for( j =  0; j < 36; j++ )
		{
			if( cTemp != sAlgebra[j] ) continue;
			iCn[i] = j;
			break;
		}
	}

	/*
	 *  Key 대수치 구함
	 */
	if( strlen(stPwi->sOrgzCd) == 8 )
	{
		sprintf( sKn,"%c%c%.6s%.8s", stPwi->sOrgzCd[0], /* 기관코드          */
									 stPwi->sOrgzCd[7],
									 stPwi->sBizYmd,    /* 거래일 6자리      */
									 gsUserId );        /* 전송자명앞 8자리  */
	}
	else
	{
		sprintf( sKn,"%.2s%.6s%.8s", stPwi->sOrgzCd+1,  /* 기관코드          */
									 stPwi->sBizYmd,    /* 거래일 6자리      */
									 gsUserId );        /* 전송자명앞 8자리  */
	}

	for( i = 0; i < 16; i++ )
	{
		cTemp = islower(sKn[i]) ? toupper(sKn[i]):sKn[i];

		for( j =  0; j < 36; j++ )
		{
			if( cTemp != sAlgebra[j] ) continue;
			iKn[i] = j;
			break;
		}
	}

	/*
	 *  평문 대수치및 평문 대수값을 구함
	 */
	for( i = 0; i < 16; i++ )
	{
		iPn[i] = (( iCn[i] + CMS_MODULUS ) - iKn[i] ) % CMS_MODULUS;
		sPn[i] = sAlgebra[iPn[i]];

#if 0
		LOG(_FL_, 0, 0, ">>sCn[%d]:%c,iCn[%d]:%d ,iKn[%d]:%d,sKn[%d]:%c => iPn[%d]:%d,sPn[%d]:%c " ,
				   i, sCn[i], i, iCn[i], i, iKn[i], i, sKn[i] , i, iPn[i], i, sPn[i]);
#endif
	}

	strncpy( stPwo->sUserPw, sPn,            16 );
	strncpy( stPwo->sEnCode, stPwi->sEnCode, 16 );
	return 0;
}


#ifdef _EXE

/*
 *
 */
void main( int argc, char *argv[] )
{
	char *sReturn;
	char  sEnCode[20+1];

	/*
	 *  암호화
	 */
	/*sReturn = GetPassword("KFT51", 1, "KIMCHUNG", "1234567890123456", "243" );*/
	sReturn = GetPassword("KFT51", 1, "HCMSTEST", "HCT052", "269" );
	if( sReturn == NULL )
	{
		LOG(_FL_, 0, 0, "GetPassword call error" );
		return;
	}
	LOG(_FL_, 0, 0, " OUTPUT 인코딩암호=[%s]", sReturn );

	/*
	 *  복호화
	 */
	strcpy( sEnCode, sReturn );
	sReturn = GetPassword("KFT51", 2, "HCMSTEST", sEnCode, "269" );
	if( sReturn == NULL )
	{
		LOG(_FL_, 0, 0, "GetPassword call error" );
		return;
	}
	LOG(_FL_, 0, 0, " OUTPUT 디코딩암호=[%s]", sReturn );

}

#endif
