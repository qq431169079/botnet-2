#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include "global.h"
#include "map.h"
#include "file.h"
#include "router.h"
#include "sniffer.h"
char path[500];

int main() {
    FILE *fp;
    unsigned int error;
    FreeConsole();

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)getMap,NULL,0,NULL);

    while(ready != true);
    if((error = initFile(&fp,"data")) != 0){
        exit(0);
    }

    fclose(fp);

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)initSniffer,NULL,0,NULL);
    initRouter();
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sender,NULL,0,NULL);
    listener();
    return 0;
}

