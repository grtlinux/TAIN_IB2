/*
 * nuse_field.c
 *
 *  Created on: 2013. 1. 23.
 *      Author: lee
 */
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define ROOT_DIR "/hw01/ibridge/ram/cfg/mapper"

int main( int argc, char **argv ) {
    struct dirent *pstEnt;
    struct stat   stStat;
    DIR  *pstDir;
    FILE *pstFile;
    char sPath[PATH_MAX];
    char sBuff[999];
    int  ii = 0, iFNo, iMaxNo = -1;
    char bits[600000] = { 0x00, }, sNo[7] = { 0x00, };
    char *psDataStart, *psFNo;

    if ((pstDir = opendir (ROOT_DIR)) == NULL)
    {
        printf("opendir fail\n");
    }

    while ((pstEnt = readdir(pstDir)) != NULL)
    {
        if (!strcmp(pstEnt->d_name, ".") || !strcmp (pstEnt->d_name, "..")) continue;
        if (strcmp(pstEnt->d_name, "comp.cfg") != 0) {
            if( memcmp(pstEnt->d_name, "msg_", 4) != 0 || memcmp(pstEnt->d_name+11, ".cfg", 4) != 0 || strlen(pstEnt->d_name) != 15) continue;
        }
        sprintf(sPath, "%s/%s", ROOT_DIR, pstEnt->d_name);

        pstFile = fopen (sPath, "r");
        if (pstFile == NULL)
        {
            printf("fopen[%s] fail\n", sPath); fflush(stdout);
        }

        while (1) {
            if (fgets(sBuff, sizeof(sBuff), pstFile) == NULL) break;
            if (strcmp(pstEnt->d_name, "comp.cfg") == 0) {
                if (strlen(sBuff) < 12) continue;
                psDataStart = sBuff+7;
            } else {
                psDataStart = strstr(sBuff, "|F|");
                if (psDataStart == NULL) continue;
                psDataStart += 3;
            }

            for (psFNo = strtok(psDataStart, ":"); psFNo && psFNo[0]; psFNo = strtok(NULL, ":")) {
                memcpy(sNo, psFNo, 6);
                iFNo = atoi(sNo);
                bits[iFNo] = 1;
                if (iFNo > iMaxNo ) iMaxNo = iFNo;
            }
        }
    }

    for (ii = 1; ii <= iMaxNo; ii++) {
        if (bits[ii] == 1) printf("%06d ", ii);
        if (ii % 100 == 0) printf("\n");
    }
    printf("\n");

    fflush(stdout);
    return 0;
}
