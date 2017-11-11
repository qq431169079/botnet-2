#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#   define MAX_ITEMS 200
#   include <stdbool.h>
    extern char addr_list[MAX_ITEMS][16];
    extern int item_cnt;
    extern int conn_cnt;
    extern char path[500];
    extern char own_ip[16];
    extern bool ready;
#endif // GLOBAL_H_INCLUDED
