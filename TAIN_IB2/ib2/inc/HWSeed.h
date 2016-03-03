/*
 * PROGRAM ID   : HWSeed.h
 * PROGRAM NAME : HWSeed 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : HWSeed 헤더
 * HISTORY      : 2013.10.17 강석. 최초작성
 *
 */

#ifndef _HWSEED_H_
#define _HWSEED_H_

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

/********************* Type Definitions **********************/

typedef unsigned int        DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;

/*
#ifndef TYPE_DEFINITION
	#define TYPE_DEFINITION
	#if defined(__alpha)
		typedef unsigned int        DWORD;
		typedef unsigned short      WORD;
	#else
		typedef unsigned long int   DWORD;
		typedef unsigned short int  WORD;
	#endif
	typedef unsigned char           BYTE;
#endif
*/

/******************* Constant Definitions *********************/

#define NoRounds         16
#define NoRoundKeys      (NoRounds*2)
#define SeedBlockSize    16    /* in bytes */
#define SeedBlockLen     128   /* in bits */

/********************** Common Macros ************************/

#if defined(_MSC_VER)
	#define ROTL(x, n)     (_lrotl((x), (n)))
	#define ROTR(x, n)     (_lrotr((x), (n)))
#else
	#define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))
	#define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))
#endif

/**************** Function Prototype Declarations **************/

#ifndef PROTOTYPES
#define PROTOTYPES  1
#endif

#if PROTOTYPES
#define PROTO_LIST(list)    list
#else
#define PROTO_LIST(list)    ()
#endif

void SeedEncrypt
	PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
void SeedDecrypt
	PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
void SeedEncRoundKey
	PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));
void SeedDecRoundKey
	PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));

/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/

/* function prototype  */
void SeedEncrypt(BYTE *pbData, DWORD *pdwRoundKey);
void SeedDecrypt(BYTE *pbData, DWORD *pdwRoundKey);
void SeedEncRoundKey(DWORD *pdwRoundKey, BYTE *pbUserKey);
void HWSeed(char *master_key, int index, char* sour,char* dest,int opt);


#endif


