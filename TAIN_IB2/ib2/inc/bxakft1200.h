/*
 *  PROGRAM ID      : bxaktf1200.h
 *  PROGRAM NAME    : KFTC CMS �ڷ� �ۼ��� MAC KEY HEADER FORMAT
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 ����, �����ۼ�
 */

#ifndef _BXAKTF1200_H_
#define _BXAKTF1200_H_

#define  MAC_KEY1  "9715471621"
#define  MAC_KEY2  "5219287503"
#if 0
#define  MAC_KEY1  "7258911346" /*-  */
#define  MAC_KEY2  "6313998542" /*-  */
#endif

typedef struct _MAC_KEY_I_
{
    long lDataCnt;       /* �ݺ�Ƚ��: Call�� ���� 1���� 1�� ���� */
    char sDataVal[10+1]; /* Data:������� */
    char sIpData1[10+1]; /* �Է°�1:�ݺ�ȸ�� 1�̸� MacKey1, �׿ܿ��� ���� Output1 */
    char sIpData2[10+1]; /* �Է°�2:�ݺ�ȸ�� 1�̸� MacKey2, �׿ܿ��� ���� Output2 */
    char sMacKey1[10+1]; /* MacKey1         */
    char sMacKey2[10+1]; /* MacKey2         */
}   MAC_KEY_I;

typedef struct _MAC_KEY_O_
{
    char sOpData1[10+1]; /* Output1         */
    char sOpData2[10+1]; /* Output2         */
    char sMacValu[10+1]; /* MAC������       */
}   MAC_KEY_O;

#endif
