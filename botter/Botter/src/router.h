#ifndef ROUTER_H_INCLUDED
#define ROUTER_H_INCLUDED
#   define BUF_SIZ 100
    typedef struct arg{
        SOCKET connexion;
        char ip[16];
    }ARG;
    typedef struct dirent DIRENT;

    void removeSubstring(char* str,const char *toremove);
    int initRouter();
    void listener ();
    void sender ();
    char* selectAddress();
    void listenerHandler(ARG *arg);
    void senderHandler(SOCKET connexion, char conn_ip[16]);
#endif // ROUTER_H_INCLUDED
