
/*
 *  1. �� �� �� : Passwd.c
 *  2. ��    �� : KFTC �ڵ���ü(CMS) ����� ��ȣ ��ȣȭ �� ��ȣȭ ���
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
	int   iProcTp;       // ó������ 1: ��ȣȭ 2: ��ȣȭ
	char  sUserId[20+1]; // �۽��ڸ�
	char  sUserPw[20+1]; // �۽��ھ�ȣ
	char  sBizYmd[20+1]; // �ŷ���
	char  sOrgzCd[20+1]; // �̿����ĺ��ڵ�
	char  sEnCode[20+1]; // �۽��ھ�ȣ ��ȣ��
}   PASSWORD_I;

typedef struct _PASSWORD_O_
{
	char  sUserPw[20+1]; // �۽��ھ�ȣ
	char  sEnCode[20+1]; // �۽��ھ�ȣ ��ȣ��
}   PASSWORD_O;
*/


#define KFTC_CMS 1 /* ������ CMS      */
#define KFTC_KTB 2 /* ������ �ϰ����� */

/*
 *  Modulus ��
 */
#define CMS_MODULUS  36

static char gsUserId[20+1]; /* �۽��ڸ� + �������� 'Z'���� ä�� */
static char gsUserPw[16+1]; /* �۽��ھ�ȣ:6�ڸ�+�������� Ȯ��   */

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
 *  �� �� �� : KftcPasswrd
 *  �� �� �� : �ڵ���ü(CMS) ����� ��ȣ ��ȣȭ �� ��ȣȭ ���
 *  �Է����� : PASSWORD_I, PASSWORD_O
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
	LOG(_FL_, 0, 0, "   INPUT ó������=[%d]", stPwi.iProcTp );
	LOG(_FL_, 0, 0, "         �۽��ڸ�=[%s]", stPwi.sUserId );
	LOG(_FL_, 0, 0, "       �۽��ھ�ȣ=[%s]", stPwi.sUserPw );
	LOG(_FL_, 0, 0, "           �ŷ���=[%s]", stPwi.sBizYmd );
	LOG(_FL_, 0, 0, "�̿��� �ĺ��ڵ�=[%s]", stPwi.sOrgzCd );
	LOG(_FL_, 0, 0, "�۽��� ��ȣ��ȣ��=[%s]", stPwi.sEnCode );
#endif

	memset( gsUserId, 0x00, sizeof(gsUserId) );
	memset( gsUserPw, 0x00, sizeof(gsUserPw) );

	if( KftcValidation( iBizTp, &stPwi ) < 0 ) return NULL;

	switch( stPwi.iProcTp )
	{
		case  1: KftcEncryption( &stPwi, &stPwo ); break;
		case  2: KftcDecryption( &stPwi, &stPwo ); break;
		default: LOG(_FL_, 0, 0, "ó������ ���� iProcTp=[%d]", stPwi.iProcTp ); return NULL;
	}

	return stPwi.iProcTp == 1 ? stPwo.sEnCode : stPwo.sUserPw;

}   /* end of GetKftcPasswrd() */

/*
 *  �� �� �� : KftcValidation
 *  ��    �� : �Է°� Ȯ��
 *  �Է����� : PASSWORD_I, PASSWORD_O
 */
int KftcValidation( int iBizTp, PASSWORD_I *stPwi )
{
	int  i;

	if( stPwi->iProcTp != 1 && stPwi->iProcTp != 2 ) /* 1: ��ȣȭ 2: ��ȣȭ */
	{
		LOG(_FL_, 0, 0, "ó������ ����=[%d]", stPwi->iProcTp );
		return -1;
	}

	if( stPwi->sUserId == NULL )
	{
		LOG(_FL_, 0, 0, "�۽��ڸ� ����=[%s]", stPwi->sUserId );
		return -1;
	}

	if( stPwi->iProcTp == 1 && stPwi->sUserPw == NULL )
	{
		LOG(_FL_, 0, 0, "�۽��ھ�ȣ ����=[%s]", stPwi->sOrgzCd );
		return -1;
	}

	if( stPwi->sBizYmd == NULL )
	{
		LOG(_FL_, 0, 0, "�ŷ��� ����=[%s]", stPwi->sBizYmd );
		return -1;
	}

	if( stPwi->sOrgzCd == NULL )
	{
		LOG(_FL_, 0, 0, "�̿����ĺ��ڵ� ����=[%s]", stPwi->sOrgzCd );
		return -1;
	}

	if( stPwi->iProcTp == 2 && stPwi->sEnCode == NULL )
	{
		LOG(_FL_, 0, 0, "�۽��ھ�ȣ ��ȣ�� ����=[%s]", stPwi->sEnCode );
		return -1;
	}

/*
	if( strlen(stPwi->sUserId) != sizeof(stPwi->sUserId)-1 )
	{
		LOG(_FL_, 0, 0, "�۽��ڸ� ���� �ڸ���(%2d�ڸ�) ����=[%d]", sizeof(stPwi->sUserId)-1, strlen(stPwi->sUserId) );
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
			gsUserId[i] = 'Z';/* Key������ ���� Z���� ä�� */
		}
		else
		{
			LOG(_FL_, 0, 0, "�۽��ڸ� ����(�����ڸ� ����)=[%s]", stPwi->sUserId );
			return -1;
		}
	}

	/*
	 *  �۽��� ��ȣ Check
	 */
	if( stPwi->iProcTp == 1 )
	{
#if 0
		if( strlen(stPwi->sUserPw) != sizeof(stPwi->sUserPw)-1 )
		{
			LOG(_FL_, 0, 0, "�۽��ھ�ȣ ����:�ڸ�������=[%d]", strlen(stPwi->sUserPw) );
			return -1;
		}
#endif

		for( i = 0; i < strlen(stPwi->sUserPw); i++ )
		{
			if( !isalpha( stPwi->sUserPw[i]) && !isdigit( stPwi->sUserPw[i]) )
			{
				LOG(_FL_, 0, 0, "�۽��ھ�ȣ ����(�����ڸ� ���)=[%s]", stPwi->sUserPw );
				return -1;
			}
			gsUserPw[i] = stPwi->sUserPw[i];
		}

		/*
		 *  6�ڸ� ���Ĵ� Ȯ��
		 */
		for( ; i < 16; i++ )
		{
			gsUserPw[i] = gsUserPw[i%6];
		}

		gsUserPw[i] = '\0'; /* NULL */

	}

	if( iBizTp == KFTC_KTB && stPwi->iProcTp != 2 ) return 0;

	/*
	 *  �̿����ĺ��ڵ� Check
	 */
	for( i = 0; i < (iBizTp == KFTC_KTB ? 2 : 3); i++ )
	{
		if( !isalpha( stPwi->sOrgzCd[i]) && !isdigit( stPwi->sOrgzCd[i]) )
		{
			LOG(_FL_, 0, 0, "�̿����ĺ��ڵ� ����=[%s]", stPwi->sOrgzCd );
			return -1;
		}
	}

	if( iBizTp == KFTC_CMS && stPwi->iProcTp != 2 ) return 0;

	/*
	 *  �۽��� ��ȣ ��ȣ�� Check
	 */
	for( i = 0; i < 16; i++ ) /* J3RY05F3D9R7RBJJ */
	{
		if( !isalpha(stPwi->sEnCode[i]) && !isdigit(stPwi->sEnCode[i]) )
		{
			LOG(_FL_, 0, 0, " �۽��ھ�ȣ ��ȣ�� ����(�����ڸ� ���)=[%s]", stPwi->sEnCode );
			return -1;
		}
	}

	return 0;
}

/*
 *  �� �� �� : KftcEncryption
 *  �� �� �� : �ڵ���ü(CMS) ����� ��ȣ ��ȣȭ
 *  �Է����� : PASSWORD_I, PASSWORD_O
 */
int KftcEncryption( PASSWORD_I *stPwi, PASSWORD_O *stPwo )
{
	int  iCn[16] = {    0, };  /* ��ȣ���� ���ġ  */
	int  iPn[16] = {    0, };  /* �򹮴��ġ       */
	int  iKn[16] = {    0, };  /* ��ȣŰ ���ġ    */

	char sPn[16] = { 0x00, };  /* �۽��� ��ȣ �� */
	char sKn[16] = { 0x00, };  /* ��ȣŰ ����      */
	char sCn[16] = { 0x00, };  /* ��ȣ�� ����      */

	char cTemp;
	int  i, j;

	/*  << ��ȣȭ �˰��� >>
	 *  C1=(P1+K1) MOD M
	 *  C2=(P2+K2) MOD M
	 *      ......
	 *  Cn=(Pn+Kn) MOD M
	 *  ���⼭ C:��ȣ���� ���� (n=1...16)
	 *       P:���� ����(n=1...16)
	 *       K:��ȣŰ ������ ����(n=1...16)
	 *         ����ڵ�2�ڸ�+�ŷ���6�ڸ�(yyyymmdd)+�����ڸ� �� 8�ڸ�)
	 *       M:Modulus 36
	 */

	/*  << �������� >>
	 *  1)�빮�� ������ ���ڷθ� �����Ͽ� ����
	 *  2)Ű �� ���뵥��Ÿ�� 16�ڸ��� �
	 *  3)�۽��ھ�ȣ�� 6�ڸ��� ����ϰ� ��ȣ�� 16�ڸ��� Ȯ���Ѵ�
	 *  4)Ű������ �����ڸ��� 8�ڸ� �̸��̸� �������� ������ "Z"�� ä���
	 *  5)������� 35�̻��̸� ����� 0���� ��ȯ��
	 */

	/*
	 *  �� ���ġ�� ����: ó�� 6�ڸ�
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
	 *  Key ���ġ ����
	 */
	if( strlen(stPwi->sOrgzCd) == 8 )
	{
		sprintf( sKn,"%c%c%.6s%.8s", stPwi->sOrgzCd[0], /* ����ڵ�          */
									 stPwi->sOrgzCd[7],
									 stPwi->sBizYmd,    /* �ŷ��� 6�ڸ�      */
									 gsUserId );        /* �����ڸ�� 8�ڸ�  */
	}
	else
	{
		sprintf( sKn,"%.2s%.6s%.8s", stPwi->sOrgzCd+1,  /* ����ڵ�          */
									 stPwi->sBizYmd,    /* �ŷ��� 6�ڸ�      */
									 gsUserId );        /* �����ڸ�� 8�ڸ�  */
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
	 *  ��ȣ�� ���ġ�� ��� ���� ����
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
 *  �� �� �� : KftcDecryption
 *  �� �� �� : �ڵ���ü(CMS) ����� ��ȣ ��ȣȭ
 *  �Է����� : PASSWORD_I, PASSWORD_O
 */
int KftcDecryption( PASSWORD_I *stPwi, PASSWORD_O *stPwo )
{
	int  iCn[16] = {    0, };  /* ��ȣ���� ���ġ  */
	int  iPn[16] = {    0, };  /* �򹮴��ġ       */
	int  iKn[16] = {    0, };  /* ��ȣŰ ���ġ    */

	char sPn[16] = { 0x00, };  /* �۽��� ��ȣ �� */
	char sKn[16] = { 0x00, };  /* ��ȣŰ ����      */
	char sCn[16] = { 0x00, };  /* ��ȣ�� ����      */

	char cTemp;
	int  i,j;

	/*  << ��ȣȭ �˰��� >>
	 *  P1=( (C1+M1) - K1 ) MOD M
	 *  P2=( (C2+M2) - K2 ) MOD M
	 *      ......
	 *  Pn=( (Cn+Mn) - Kn ) MOD M

	 *  ���⼭ C:��ȣ���� ���� (n=1...16)
	 *         P:���� ����(n=1...16)
	 *         K:��ȣŰ ������ ����(n=1...16)
	 *           ����ڵ�2�ڸ�+�ŷ���6�ڸ�(yyyymmdd)+�����ڸ� �� 8�ڸ�)
	 *         M:Modulus 36
	 */

	/*
	 *  ��ȣ�� ���ġ�� ����
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
	 *  Key ���ġ ����
	 */
	if( strlen(stPwi->sOrgzCd) == 8 )
	{
		sprintf( sKn,"%c%c%.6s%.8s", stPwi->sOrgzCd[0], /* ����ڵ�          */
									 stPwi->sOrgzCd[7],
									 stPwi->sBizYmd,    /* �ŷ��� 6�ڸ�      */
									 gsUserId );        /* �����ڸ�� 8�ڸ�  */
	}
	else
	{
		sprintf( sKn,"%.2s%.6s%.8s", stPwi->sOrgzCd+1,  /* ����ڵ�          */
									 stPwi->sBizYmd,    /* �ŷ��� 6�ڸ�      */
									 gsUserId );        /* �����ڸ�� 8�ڸ�  */
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
	 *  �� ���ġ�� �� ������� ����
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
	 *  ��ȣȭ
	 */
	/*sReturn = GetPassword("KFT51", 1, "KIMCHUNG", "1234567890123456", "243" );*/
	sReturn = GetPassword("KFT51", 1, "HCMSTEST", "HCT052", "269" );
	if( sReturn == NULL )
	{
		LOG(_FL_, 0, 0, "GetPassword call error" );
		return;
	}
	LOG(_FL_, 0, 0, " OUTPUT ���ڵ���ȣ=[%s]", sReturn );

	/*
	 *  ��ȣȭ
	 */
	strcpy( sEnCode, sReturn );
	sReturn = GetPassword("KFT51", 2, "HCMSTEST", sEnCode, "269" );
	if( sReturn == NULL )
	{
		LOG(_FL_, 0, 0, "GetPassword call error" );
		return;
	}
	LOG(_FL_, 0, 0, " OUTPUT ���ڵ���ȣ=[%s]", sReturn );

}

#endif
