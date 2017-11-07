#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include "conio.h"
#include "global.h"
#include "map.h"
#include "display.h"
char path[500];

void viewMain (){
    int choice = -1;
    bool jump = false;
    system("cls");
    printIntro();
    printf("\t| 1. Voir la map\n");
    printf("\t| 2. Demarrer\n");
    printf("\t|\n");
    printf("\t| 0. Quitter\n");
    printf("\n");
    do{
        fflush(stdin);
        choice = (int)getch() - 48;
        switch(choice){
            case 0: ExitProcess(EXIT_SUCCESS); break;
            case 1: viewMap();  break;
            case 2: break;
            default: jump = true;
        }
    }while(jump == true);
}

void viewMap () {
    int choice,i,delay = 1;
    bool jump = false;
    system("cls");
    printIntro();
    printf("\t| 0. Retour au menu\n");
    printf("\t| 1. Refresh\n");
    textcolor(11);
    printf("\n[Network map]:");
    textcolor(7);
    for(i=0; i<item_cnt; i++){
        textcolor(7);
        printf("\n# ");
        textcolor(15);
        printf("%s",addr_list[i]);
        textcolor(7);
    }
    printf("\n------------\n");
    do{
        fflush(stdin);
        choice = (int)getch()-48;
        switch(choice){
            case 0: viewMain(); break;
            case 1: viewMap (); break;
            default: jump = true;
        }
    }while(jump == true);
}

void printIntro(){
    textcolor(10);
    printf("\t\t\t\t  .##########.\n");
    printf("\t\t\t      --:::#");
    textcolor(14);
    textbackground(2);
    printf(" BotTer ");
    textbackground(0);
    textcolor(10);
    printf("#:::--\n");
    printf("\t\t\t\t _:##########:_\n");
    textcolor(14);
    printf("\n\t\t Le botnet le plus fort de toute la planete Terre !\n");
    textcolor(7);
    printf("________________________________________________________________________________\n");
}

void printError(int errorCode) {
	textbackground(4);
	printf("ERROR %d:", errorCode);
	textbackground(0);
	textbackground(7);
	putc(' ',stdout);
	textcolor(4);
	switch(errorCode){
		case 0: puts("No error"); break;
		//error code 1X : erreurs de la librairie
		case 10: puts("WSAStartup failed"); break;
		//error code 2X : erreurs de fichier
		case 20: puts("File opening failed"); break;
		case 21: puts("File writing failed"); break;
		//error code 3X : erreurs de sniffer
		case 30: puts(""); break;
		//error code 4X : erreurs de routeur
		case 40: puts("Bot not responding"); break;
		case 41: puts("Connexion failed"); break;
		//error code 5X : erreurs de map
		case 50: puts("Connexion to server failed"); break;
		case 51: puts("Server not responding"); break;

		default: puts("Unknown error");
	}
	textbackground(0);
	textcolor(7);
	putc('\n',stdout);
}
