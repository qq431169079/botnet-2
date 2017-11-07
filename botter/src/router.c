#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <winsock2.h>
#include "global.h"
#include "chained_list.h"
#include "file.h"
#include "router.h"

int conn_cnt = -1;
int file_cnt = 0;
LINK *file_avail = NULL;
LINK *recpt_log = NULL;

void removeSubstring(char* str,const char* toremove){
  while( (str=strstr(str,toremove)) )
    memmove(str,str+strlen(toremove),1+strlen(str+strlen(toremove)));
}

int initRouter() {
    //Initialise la liste des fichiers disponnible
    char name[25];
    DIR* rep = NULL;
    DIRENT* file = NULL;

    rep = opendir(".");
    if (rep == NULL) return EXIT_FAILURE;

    while ((file = readdir(rep)) != NULL){
        if(strstr(file->d_name,EXT) != NULL){
            strcpy(name, file->d_name);
            removeSubstring(name,EXT);
            addToList(&file_avail,name);
            ++file_cnt;
        }
    }

    closedir(rep);

    return EXIT_SUCCESS;
}

void listener() {
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
        ExitThread(1);
    }

    SOCKADDR_IN addr, client_addr;
    int addrlen = sizeof(addr);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(667);
    addr.sin_family = AF_INET;

    SOCKET slisten = socket(AF_INET,SOCK_STREAM,0);
    bind(slisten,(SOCKADDR*)&addr, addrlen);
    listen(slisten, SOMAXCONN);

    SOCKET connexion;
    char ip[16];
    while(1){
        connexion = accept(slisten, (SOCKADDR*)&client_addr, &addrlen);
        if(connexion != 0){
            strcpy(ip,inet_ntoa(client_addr.sin_addr));
        }

        ARG arg;
        arg.connexion = connexion;
        strcpy(arg.ip,ip);
        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)listenerHandler,(PVOID)&arg,0,NULL);
    }
}

void sender() {
    clock_t t0, t1;
    bool start = false;
    char *address = malloc(16*sizeof(char));

    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,1),&wsaData) != 0){
        ExitThread(1);
    }

    SOCKADDR_IN addr;
    int  addrlen = sizeof(addr);
    addr.sin_port = htons(667);
    addr.sin_family = AF_INET;
    SOCKET connexion;

    while(1){
        switch(start){
            case false :
                t0 = clock()/CLOCKS_PER_SEC;
                start = true;
                break;
            case true :
                t1 = clock()/CLOCKS_PER_SEC;
                if(t1 >= t0 + 10){

                    address = selectAddress();
                    if(address != NULL){
                        addr.sin_addr.s_addr = inet_addr(address);

                        connexion = socket(AF_INET,SOCK_STREAM,0);
                        if(connect(connexion,(SOCKADDR*)&addr,addrlen) != 0){
                            start = false;
                            continue;
                        }

                        senderHandler(connexion,address);
                        start = false;
                    }
                }
                break;
        }
    }
    free(address);
}

char* selectAddress() {
    if(item_cnt < 2) return NULL;

    int i;
    int index;
    char* address = malloc(16*sizeof(char));

    conn_cnt++;

    if(conn_cnt%(item_cnt-1) == 0){
        conn_cnt=0;
    }

    for(i=0; i < item_cnt; i++){
        if(strcmp(addr_list[i],own_ip) == 0){
            ++i;
            break;
        }
    }


    if((index = i+conn_cnt) >= (item_cnt))
        index = (i+conn_cnt) % (item_cnt);

    if(strcmp(addr_list[index],own_ip) == 0){
        index = (i+conn_cnt+1) % (item_cnt);
    }
    strcpy(address,addr_list[index]);

    return address;
}

void listenerHandler(ARG *arg) {

    SOCKET connexion = arg->connexion;
    char ip[16];
    char tmp[16];
    LINK* avail;
    unsigned long size = 0;
    char* data;
    char size_char[15];
    int i;
    strcpy(ip,arg->ip);

    // Recois la taille du fichier
    recv(connexion,size_char,15,0);
    size = atoi(size_char);
    printf("Reception de données...\n");
    // Recois et écrit le contenu du fichier
    data = malloc(size*sizeof(char));
    recv(connexion,data,size,0);
    printf("Ecriture de données...\n");
    writeFile(ip,data,size);
    if(itemExist (file_avail, ip) == 0) ++file_cnt;
    printf("Ecriture reussi\n");

    //Ajoute aux logs de récéption et à la liste des fichiers disponnibles et supprime le premier fichier de la liste
    addToList(&recpt_log, ip);
    if(getListSize(recpt_log) >= item_cnt){
        removeFirst(&recpt_log);
    }
    if(itemExist(file_avail, ip) == 0) {
        addToList(&file_avail, ip);
    }
    if(getListSize(file_avail) >= 5){
        removeFirst(&file_avail);
    }

    // Recois la liste des fichiers disponnibles
    while(1){
        recv(connexion,tmp,16,0);
        if(strcmp(tmp,"stop") == 0) break;
        addToList(&avail, tmp);
    }

    // Demande en priorité un fichier qu'on ne possède pas
    unsigned int len;
    LINK* tmp_avail;
    bool jmp = false;
    char choice[16];
    bool null = false;

    if(getListSize(avail) > 0){
        len = getListSize(avail);
        tmp_avail = avail;
        for(i=0; i<len; i++){
            if(itemExist(file_avail, tmp_avail->ip) == 0){
                strcpy(choice,tmp_avail->ip);
                send(connexion,tmp_avail->ip,16,0);
                jmp = true;
                break;
            }
            tmp_avail = tmp_avail->next;
        }
    }else {
        jmp = true;
        null = true;
        send(connexion,"null",16,0);
        closesocket(connexion);
        ExitThread(0);
    }

    //OU Demande le premier fichier des logs de rec. qui est présent dans la liste
    if(jmp == false){
        len = getListSize(recpt_log);
        LINK* tmp_log = recpt_log;
        for(i=0; i<len; i++){
            if(itemExist(avail, tmp_log->ip) == 1){
                jmp = true;
                strcpy(choice,tmp_log->ip);
                send(connexion,tmp_log->ip,16,0);
                break;
            }
            tmp_log = tmp_log->next;
        }
    }

    if(jmp == false){
        null = true;
        send(connexion,"null",16,0);
    }

    if(null == false){
        unsigned long size2 = 0;
        char* data2;
        char size_char2[15];

        // Recois la taille du fichier
        recv(connexion,size_char2,15,0);
        size2 = atoi(size_char2);

        // Recois le contenu du fichier
        data2 = malloc(size2*sizeof(char));
        recv(connexion,data2,size2,0);
        writeFile(choice,data2,size2);
        if(itemExist(file_avail, choice) == 0) ++file_cnt;

        // Ajoute aux logs de récéption  et à la liste des fichiers disponnibles  et supprime le premier fichier de la liste
        addToList(&recpt_log, choice);
        if(getListSize(recpt_log) >= item_cnt){
            removeFirst(&recpt_log);
        }
        if(itemExist(file_avail, choice) == 0) {
            addToList(&file_avail, choice);
        }
        if(getListSize(file_avail) >= 5){
            removeFirst(&file_avail);
        }
    }
    //closesocket(connexion);
    ExitThread(0);
}

void senderHandler(SOCKET connexion, char conn_ip[16]) {
    char* data;
    char size_char[15];
    char choice[16];
    LINK* avail;
    unsigned long size = getFileContent(own_ip,&data);
    printf("Envoi de données...\n");
    sprintf(size_char,"%lu",size);
    int i;

    // Envoi la taille du fichier
    send(connexion,size_char,15,0);

    // Envoi le contenu du fichier
    send(connexion,data,size,0);

    // Envoi la liste des fichiers disponnibles
    avail = file_avail;
    for(i=0; i<file_cnt; i++){
        if(avail == NULL) break;
        if(strcmp(conn_ip, avail->ip) != 0 && strcmp(avail->ip,"data") != 0){
            send(connexion,avail->ip,16,0);
        }
        avail = avail->next;
    }
    send(connexion,"stop\0",5,0);

    // Recois la demande de fichier
    recv(connexion,choice,16,0);

    if(strcmp(choice,"null") != 0 && strcmp(choice,"") != 0){
        char* data2;
        char size_char2[15];
        unsigned long size2;

        // Envoi la taille du fichier
        size2 = getFileContent(choice,&data2);
        sprintf(size_char2,"%lu",size2);
        send(connexion,size_char2,15,0);

        // Envoi le contenu du fichier
        send(connexion,data2,size2,0);
    }

    closesocket(connexion);
}
