#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chained_list.h"

int addToList (LINK **list, char* content){
    LINK* newlnk;
    if((newlnk = malloc(sizeof(LINK))) == NULL){
        return EXIT_FAILURE;
    }
    strcpy(newlnk->ip,content);
    newlnk->next = NULL;
    if(*list == NULL){
        *list = newlnk;
    } else {
        LINK* tmp= *list;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = newlnk;
    }
    return EXIT_SUCCESS;
}

int removeFirst (LINK **list){
    LINK* local = *list;
    if(*list != NULL) {
        LINK* tmp;
        strcpy(tmp,local->next);
        free(*list);
        *list = tmp;
    }
    else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

unsigned int getListSize (LINK *list){
    unsigned int size = 0;
    LINK* tmp = list;
    if(tmp != NULL){
        ++size;
        while(tmp->next != NULL){
            tmp = tmp->next;
            ++size;
        }
    }
    return size;
}

int itemExist (LINK *list, char* item){
    LINK* tmp = list;
    if(tmp != NULL){
        while(tmp->next != NULL){
            if(strcmp(tmp->ip,item) == 0) {
                return 1;
            }
            tmp = tmp->next;
        }
    }
    return 0;
}
