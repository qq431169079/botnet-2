#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "file.h"

int initFile(FILE** file,const char* dest) {
  char begining[2];
  char name[200];
	strcpy(path,dest);
	strcpy(name,dest);
	strcat(name,EXT);
	*file = fopen(name,"ab");
  unsigned int size = 0;

	if(file != NULL){

        fseek(*file, 0, SEEK_END);
        size = ftell(*file);
        if(size == 0){
            fprintf(*file,"i=%s;",own_ip);
        }

		return 0;
	}else{
		return 20;
	}
}

int writeFile(char* dest,char* data,unsigned int size) {
    char name[200];
    strcpy(name,dest);
	strcat(name,EXT);
	FILE* file = fopen(name,"wb");

	if(file != NULL){
        fwrite(data, size, sizeof(char), file);
		return 0;
	}else{
		return 20;
	}
}

int startBlock(FILE* file, char *url){
    fprintf(file,"u=%s;",url);
    fputs("d=[",file);
}

int addData(FILE* file, char *key, char *value){
    fprintf(file,"%s=%s;",key,value);
}

int endBlock(FILE* file){
    fputs("]",file);
}

unsigned int getFileContent(char* ip,char** content){
    char c;
    FILE* fp;
    int size = 0;
    if(strcmp(ip,own_ip) == 0){
        fp = fopen("data.ter","rb");
    }
    else {
        char name[200];
        strcpy(name,ip);
        strcat(name,EXT);
        fp = fopen(name,"rb");
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *content = malloc(size*sizeof(char));
    fread(*content,size,1,fp);

    return size;
}
