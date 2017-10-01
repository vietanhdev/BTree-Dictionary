#include <string.h>
#include "UI.h"
#include "extio.h"
#include "dictionary.h"
#include "btree.h"


BTA * dict;
char * notify1;
char * notify2;
char * meaningArea;

void UI_init(BTA * g_dict, char * g_meaningArea, char * g_notify1, char * g_notify2) {
    dict = g_dict;
    notify1 = g_notify1;
    notify2 = g_notify2;
    meaningArea = g_meaningArea;
    notify1[0] = '\0';
    notify2[0] = '\0';
    meaningArea[0] = '\0';
    UI_Notify1_Push("");
    UI_Notify2_Push("");
}

void UI_InfoBoard() {
    clear();
    int i;

    printf("\n");
    for (i = 0; i < 5; i++) {
        printf("%s\n", InfoBoard[i]);
    }
}

void UI_Menu() {
    int i;
    char c;
    char menuNotify[UI_MENU_NOTIFY_LENGTH] = "";
    int len = MENUOPT;

    while(1) {

        UI_InfoBoard();
        printf("\t*** MENU ***\n");
        for(i = 0; i < len; i++) {
            printf("\t");
            printColor(menuShortcuts[i], YELLOW);
            printf("\t%s\n", menu[i]); 
            
        }
        printColor("\tx", YELLOW);
        printf("\t%s\n", "Close menu"); 
        printf("\t"); printColor(menuNotify, RED); 
        c = getch();

        switch(c) {
            case 'l': createDictionary(dict, notify1); return;
            case 'x': return;
            default: strcpy(menuNotify, "Invalid selection!");
        }
    }
    

}

void UI_Search(char * searchBox) {
    printf("\n>> Search: "); printf("%s", searchBox);
    gotoxy(12 + strlen(searchBox), 10);
}

void UI_Notify() {
    printf("%s\n", notify1);
    printf("%s\n", notify2);
}

void UI_Notify1_Push(char * s) {
    strcpy(notify1, s);
}

void UI_Notify2_Push(char * s) {
    strcpy(notify2, s);
}

void gotoxy(int x,int y) {
    printf("%c[%d;%df",0x1B,y,x);
    fflush(stdout);
}

