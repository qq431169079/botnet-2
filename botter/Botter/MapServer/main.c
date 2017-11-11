#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_CONN 200

typedef struct addr {
    SOCKET connexion;
    char address[16];
    bool is_connected;
} ADDR;

ADDR addr_list[MAX_CONN];
int addr_cnt = 0;
int conn_cnt = 0;

void MultiClientThread(); //Send list to everyone all 1min
void removeConnexion (index);
int sendListToAll();
void verifyConnexion();
void shuffleList();

bool is_verifying = false;
bool is_sending = false;

//MANAGER

int main(int argc, char **argv) {
    char msg[16];
    int i;
    bool find;
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,1),&wsaData) != 0){
        printf("startup failed\n");
        return 1;
    }

    SOCKADDR_IN addr, client_addr;
    int addrlen = sizeof(addr);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(666);
    addr.sin_family = AF_INET;

    SOCKET slisten = socket(AF_INET,SOCK_STREAM,NULL);
    bind(slisten,(SOCKADDR*)&addr, addrlen);
    listen(slisten, SOMAXCONN);
    printf("Waiting for connexion...\n\n");

    CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)MultiClientThread,NULL,NULL,NULL);
    CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)verifyConnexion,NULL,NULL,NULL);

    SOCKET newConnexion;
    while(addr_cnt <= MAX_CONN){
        newConnexion = accept(slisten, (SOCKADDR*)&client_addr, &addrlen);
        if(newConnexion == 0){
            printf("%s: [connexion]\t[FAIL]\n",inet_ntoa(client_addr.sin_addr));
            return 1;
        }else {
            find = false;
            printf("%s: [connexion]\t[DONE]\n",inet_ntoa(client_addr.sin_addr));
            for(i = 0; i < addr_cnt; i++){
                if(strcmp(inet_ntoa(client_addr.sin_addr),addr_list[i].address) == 0){
                    printf("%s: [listing]\t[ALREADY EXIST]\n",addr_list[i].address);
                    addr_list[i].connexion = newConnexion;
                    addr_list[i].is_connected = true;
                    find = true;
                    break;
                }
            }
            if(find == false){
                addr_list[addr_cnt].connexion = newConnexion;
                addr_list[i].is_connected = true;
                strcpy(addr_list[addr_cnt].address,inet_ntoa(client_addr.sin_addr));
                printf("%s: [listing]\t[DONE]\n",addr_list[addr_cnt].address);
                addr_cnt++;
            }

            sendListToAll();
            printf("%s: [sending]\t[DONE]\n",inet_ntoa(client_addr.sin_addr));
            printf("-------------\n");
        }
    }
    return 0;
}

void MultiClientThread() {
    clock_t t0, t1;
    bool start = false;

    while(1) {
        switch(start){
        case false :
            t0 = clock()/CLOCKS_PER_SEC;
            start = true;
            break;
        case true :
            t1 = clock()/CLOCKS_PER_SEC;
            if(t1 >= t0 + 10 && is_verifying == false){
                is_sending = true;
                sendListToAll();
                is_sending = false;
                start = false;
            }
            break;
        }
    }
}

void verifyConnexion() {
    int i;
    clock_t t0, t1;
    bool start = false;

    while(1) {
        switch(start){
        case false :
            t0 = clock()/CLOCKS_PER_SEC;
            start = true;
            break;
        case true :
            t1 = clock()/CLOCKS_PER_SEC;
            if(t1 >= t0 + 2 && is_sending == false){
                is_verifying = true;
                for(i=0; i < addr_cnt; i++){
                    if(addr_list[i].is_connected == true && send(addr_list[i].connexion,NULL,NULL,NULL) == SOCKET_ERROR){
                        addr_list[i].is_connected = false;
                        printf("%s: [DISCONNECTED]\n",addr_list[i].address);
                        printf("-------------\n");
                        sendListToAll();
                    }
                }
                is_verifying = false;
                start = false;
            }
            break;
        }
    }

    return 1;
}

int sendListToAll() {
    int i,j;
    shuffleList();
    FILE *list_file = fopen("list","wb");
    for(i=0; i < addr_cnt; i++){
        if(addr_list[i].is_connected == true){
            for(j=0; j <= addr_cnt; j++){
                if(j == addr_cnt){
                    send(addr_list[i].connexion,"stop",5,NULL);
                }
                else if(addr_list[j].is_connected == true && send(addr_list[i].connexion,addr_list[j].address,16,NULL) == SOCKET_ERROR){
                    printf("%s: [sending]\t[FAIL]\n",addr_list[i].address);
                    break;
                }
            }
            fprintf(list_file,"%s-1\n",addr_list[i].address);
        }
        else fprintf(list_file,"%s-0\n",addr_list[i].address);
    }

    fclose(list_file);
    return 1;
}

void shuffleList() {
   int i;
   ADDR first;

    for(i=0; i<addr_cnt;i++){
        if(i==0) first = addr_list[0];
        addr_list[i] = addr_list[i+1];
        if(i==addr_cnt-1) addr_list[i] = first;
    }
}

