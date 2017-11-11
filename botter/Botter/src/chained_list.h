#ifndef CHAINED_LIST_H_INCLUDED
#define CHAINED_LIST_H_INCLUDED
    typedef struct link{
        char ip[16];
        struct link* next;
    }LINK;
    int addToList (LINK **list, char* content);
    int removeFirst (LINK **list);
    unsigned int getListSize (LINK *list);
    int itemExist (LINK *list, char* item);
#endif // CHAINED_LIST_H_INCLUDED
