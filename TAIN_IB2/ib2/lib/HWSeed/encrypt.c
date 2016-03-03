/******************************************************************************/
/* 파일명 : encript.c                                                         */
/* 내  용 : SEED 암호화/복호화 프로그램                                       */
/******************************************************************************/
#include <Seedx.h>
#include <SeedAlg.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>



#define            OFFSET_DATA        0
#define            LENGTH_DATA       16
#define            LENGTH_SEED       16

unsigned char MK_table[10][16] = {
{0xb9, 0xe8, 0xb4, 0xde, 0xc0, 0xc7, 0xc7, 0xbe, 0xf3, 0xc0, 0xcc, 0xb1, 0xea, 0xb5, 0xe7, 0xe7 },
{0xbf, 0xaa, 0xbb, 0xe7, 0xc0, 0xc7, 0xc7, 0xb0, 0xed, 0xc0, 0xe5, 0xe5, 0xbf, 0xa9, 0xb1, 0xe2 },
{0xb4, 0xc2, 0xc2, 0xbf, 0xc0, 0xb7, 0xa3, 0xc0, 0xfc, 0xc5, 0xeb, 0xeb, 0xbc, 0xad, 0xb7, 0xc1 },
{0xc0, 0xd6, 0xb4, 0xc2, 0xb0, 0xf7, 0xf7, 0xbd, 0xbd, 0xb1, 0xe2, 0xb7, 0xce, 0xbf, 0xee, 0xee },
{0xc0, 0xfe, 0xc0, 0xba, 0xc0, 0xcc, 0xb5, 0xe9, 0xe9, 0xc7, 0xd1, 0xb5, 0xa5, 0xb8, 0xf0, 0xbf },
{0xb4, 0xb4, 0xd9, 0xd9, 0xbc, 0xba, 0xbd, 0xc7, 0xb0, 0xfa, 0xfa, 0xb1, 0xd9, 0xb8, 0xe9, 0xbc },
{0xd3, 0xbf, 0xa1, 0xa1, 0xba, 0xb8, 0xb6, 0xf7, 0xc0, 0xbb, 0xbb, 0xc3, 0xa3, 0xc0, 0xda, 0xda },
{0xc1, 0xb6, 0xb1, 0xb9, 0xb0, 0xfa, 0xfa, 0xb0, 0xb0, 0xc0, 0xcc, 0xcc, 0xc5, 0xa9, 0xb4, 0xc2 },
{0xc2, 0xb1, 0xa4, 0xc1, 0xd6, 0xc0, 0xba, 0xc7, 0xe0, 0xe0, 0xb0, 0xdc, 0xb7, 0xa1, 0xbf, 0xa1 },
{0xa1, 0xc7, 0xd4, 0xb2, 0xb2, 0xb2, 0xbf, 0xf4, 0xb4, 0xc2, 0xc2, 0xb1, 0xa4, 0xc1, 0xd6, 0xd6 }
};

void Seed(char* master_key,int index,char* sour,char* dest,int opt)
{
	int     i, mok;
	int tlen,len,namegi,k,leng;

	BYTE    pbUKey[20], pbData[20];
	DWORD   pdwRKey[32];
	
	len = LENGTH_DATA;

	/* 암호화키 복호화(MAIN KEY DESCRIPT) */
	memset(pbUKey, 0x00, sizeof(pbUKey));
	memcpy(pbUKey, MK_table[index], 16);
	SeedEncRoundKey(pdwRKey, pbUKey);

	memset(pbData, 0x00, sizeof(pbData));
	memcpy(pbData, master_key, 16);
	SeedDecrypt(pbData, pdwRKey);

	/* 데이타 복호화(DATA DESCRIPT) */
	memset(pbUKey, 0x00, sizeof(pbUKey));
	memcpy(dest, sour, len);

	memcpy(pbUKey, pbData, 16);
	SeedEncRoundKey(pdwRKey, pbUKey);

/*
	mok    = len / 16;
	for(i = 0; i < mok; i++)  {
		memset(pbData, 0x00, sizeof(pbData));
		memcpy(pbData, &sour[OFFSET_DATA+16*i], 16);
		if(opt == 1)  SeedEncrypt(pbData, pdwRKey);
		else          SeedDecrypt(pbData, pdwRKey);
		memcpy(&dest[OFFSET_DATA+16*i], pbData, 16);
	}
*/	

	tlen   = len;
	mok    = len / 16;
	namegi = len % 16;
	k = ( namegi == 0 ) ?  mok : mok+1;
	
	for(i = 0; i < k; i++)
	{	
		leng = (tlen < 16) ? namegi : 16;
		memset(pbData, 0x00, sizeof(pbData));
		memcpy(pbData, &sour[16*i], leng);
		if(opt == 1)  SeedEncrypt(pbData, pdwRKey);
		else          SeedDecrypt(pbData, pdwRKey);
		memcpy(&dest[16*i], pbData, 16);
		tlen -= 16;
	}

}

void HWSeed(char* master_key,int index,char* sour,char* dest,int opt)
{
	int     i, mok;
	int tlen,len,namegi,k,leng;

	BYTE    pbUKey[20], pbData[20];
	DWORD   pdwRKey[32];

	len = LENGTH_DATA;

	/* 암호화키 복호화(MAIN KEY DESCRIPT) */
	memset(pbUKey, 0x00, sizeof(pbUKey));

	memcpy(pbUKey, MK_table[index], 16);
	SeedEncRoundKey(pdwRKey, pbUKey);

	memset(pbData, 0x00, sizeof(pbData));
	memcpy(pbData, master_key, 16);
	SeedDecrypt(pbData, pdwRKey);

	/* 데이타 복호화(DATA DESCRIPT) */
	memset(pbUKey, 0x00, sizeof(pbUKey));
	memcpy(dest, sour, len);

	memcpy(pbUKey, pbData, 16);
	SeedEncRoundKey(pdwRKey, pbUKey);

	tlen   = len;
	mok    = len / 16;
	namegi = len % 16;
	k = ( namegi == 0 ) ?  mok : mok+1;

	for(i = 0; i < k; i++)
	{
		leng = (tlen < 16) ? namegi : 16;
		memset(pbData, 0x00, sizeof(pbData));
		memcpy(pbData, &sour[16*i], leng);
		if(opt == 1)  SeedEncrypt(pbData, pdwRKey);
		else          SeedDecrypt(pbData, pdwRKey);
		memcpy(&dest[16*i], pbData, 16);
		tlen -= 16;
	}
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef _EXE

int main()
{
    unsigned char session_key[16] = 
    {
    '1','1','1','1','1','1','1','1',
    '1','1','1','1','1','1','1','1'
    };
/*
    char x_data[16] = 
    {
    0x05, 0x3A, 0xA0, 0xF1, 0x79, 0x3B, 0x24, 0x20, 
    0x69, 0x42, 0x27, 0x60, 0x53, 0x3D, 0x58, 0x3C 
    };
*/
   
    unsigned char plnPassword[16];
    unsigned char encPassword[16];
    unsigned char decPassword[16];

    int index;
   
    char dump[1000], temp[1000];
    int i;

	if (1)
	{
	    memset(dump, 0x00, sizeof(dump));
	    for(i = 0; i < 16; i++) {
	        sprintf(temp, "0x%02X ", session_key[i]);
	        strcat(dump, temp);
	    } 
	    printf("session_key[%s]\n", dump); 
	}

	if (1)
	{
	    index = 1;
	    printf("index = %d\n", index);
	    memset(plnPassword, 0x20, sizeof(plnPassword));
	    memcpy(plnPassword, "1111", 4);
	
	    printf("비밀번호[%16.16s]\n", plnPassword);  
	}

	if (1)
	{
	    /************************/
	    /* 암호화               */
	    /************************/
	
	    HWSeed(session_key, index, plnPassword, encPassword, 1);
	
	    memset(dump, 0x00, sizeof(dump));
	    for(i = 0; i < 16; i++) {
	        sprintf(temp, "0x%02X ", encPassword[i]);
	        strcat(dump, temp);
	    } 
	    printf("암호화  [%s]\n", dump); 
	}

	if (1)
	{
	    /************************/
	    /* 복호화               */
	    /************************/
	    memset(plnPassword, 0x00, sizeof(plnPassword));
	    
	    HWSeed(session_key, index, encPassword, plnPassword, 2);
	  
	    printf("복호화  [%16.16s]\n", plnPassword);
	}

	if (1)
	{
		printf("\n\n");
		
		unsigned char encPass[16] = {
			0x09, 0xA1, 0x22, 0x43, 0xAB, 0x31, 0x82, 0x3F, 0x46, 0x10, 0x26, 0x17, 0x6D, 0xF4, 0xEE, 0xAF
		};
		
	    memset(dump, 0x00, sizeof(dump));
	    for(i = 0; i < 16; i++) {
	        sprintf(temp, "0x%02X ", encPass[i]);
	        strcat(dump, temp);
	    } 
	    printf("암호문 [%s]\n", dump); 

	    /************************/
	    /* 복호화               */
	    /************************/
	    memset(plnPassword, 0x00, sizeof(plnPassword));
	    
	    HWSeed(session_key, index, encPass, plnPassword, 2);
	  
	    printf("복호문 [%16.16s]\n", plnPassword);
	}

    return 0;
}

#endif
