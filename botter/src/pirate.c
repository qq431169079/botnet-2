#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include "global.h"
#include "map.h"
#include "file.h"
#include "display.h"
#include "router.h"
#include "sniffer.h"

int main() {
    FILE *fp;
    unsigned int error;
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)getMap,NULL,0,NULL);
    puts("Connecting...\n");
    while(ready != true);

    if((error = initFile(&fp,"data")) != 0){
        printError(error);
        exit(0);
    }

    viewMain ();
    system("cls");
    printIntro();

    initRouter();
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sender,NULL,0,NULL);
    printf("Waiting for connexion...\n\n");
    listener();
    return 0;
}

