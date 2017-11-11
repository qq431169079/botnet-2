#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <winsock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include "mapaddr.h"
#include "global.h"
#include "map.h"

char addr_list[MAX_ITEMS][16];
int item_cnt;
char own_ip[16] = {0};
bool ready = false;
bool is_printing = true;
bool new_map = false;

void getMap() {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,1),&wsaData) != 0){
        ExitThread(1);
    }
    SOCKADDR_IN addr;
    int  addrlen = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr(MAPADDR);
    addr.sin_port = htons(666);
    addr.sin_family = AF_INET;

    SOCKET connexion = socket(AF_INET,SOCK_STREAM,0);
    if(connect(connexion,(SOCKADDR*)&addr,addrlen) != 0){
        ExitThread(1);
    }

    char msg[16];
    int state = 0;

    if(recv(connexion,msg,16,0) == SOCKET_ERROR){
        ExitThread(1);
    }
    strcpy(own_ip,msg);
    ready = true;
    while(1){
        if(recv(connexion,msg,16,0) == SOCKET_ERROR){
            ExitThread(1);
        }

        if(state == 0){
            item_cnt = 0;
            state++;
        }
        if(strcmp(msg,"stop") == 0){
            new_map = true;
            state--;
        }
        else {
            memset(addr_list[item_cnt],0,strlen(addr_list[item_cnt]));
            strcat(addr_list[item_cnt],msg);
            item_cnt++;
        }
    }

    ExitThread(0);
}
