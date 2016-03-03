/* function prototype  */
void SeedEncrypt(BYTE *pbData, DWORD *pdwRoundKey);
void SeedDecrypt(BYTE *pbData, DWORD *pdwRoundKey);
void SeedEncRoundKey(DWORD *pdwRoundKey, BYTE *pbUserKey);
void Seed(char *master_key, int index, char* sour,char* dest,int opt);


