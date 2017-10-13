#include <stdlib.h>
#include <string.h>
#include "UI.h"
#include "extio.h"
#include "dictionary.h"
#include "btree.h"


char searchBox[WORD_MAX_LEN] = "";
char suggestBox[SUGGEST_BOX_MAX_LEN] = "";
char notify1[NOTIFY_MAX_LEN] = "";
char notify2[NOTIFY_MAX_LEN] = "";
char meaningArea[MEAN_MAX_LEN] = "";


void UI_InfoBoard() {
    clear();
    int i;

    printf("\n");
    for (i = 0; i < 5; i++) {
        printf("%s\n", InfoBoard[i]);
    }
}

void UI_Menu(BTA * dict) {
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
            case 'a': UI_Dict_AddWord(dict); return;
            case 'd': UI_Dict_DeleteWord(dict); return;
            //case 'l': createDictionary(dict, notify1); return;
            case 'x': return;
            default: strcpy(menuNotify, "Invalid selection!");
        }
    }
    

}

void UI_Search(char * searchBox) {
    printf("\n>> Search: "); printf("%s", searchBox);
    printf("\n\x1b[33m%s\033[0m\n", suggestBox);
    gotoxy(12 + strlen(searchBox), 10);
}

void UI_Notify() {
    printf("%s\n", notify1);
    printf("%s\n", notify2);

    // empty notifications after displaying
    strcpy(notify1, "");
    strcpy(notify2, "");
}

void UI_Notify1_Push(char * s) {
    strcpy(notify1, s);
}

void UI_Notify2_Push(char * s) {
    strcpy(notify2, s);
}


void UI_Dict_AddWord(BTA * dict) {
    char word[WORD_MAX_LEN];
    char meaning[MEAN_MAX_LEN];
    printf("\n*** ADD A WORD:\n");
    printf("Word: "); readLn(stdin, word, WORD_MAX_LEN);
    printf("Meaning:\n"); readLn(stdin, meaning, MEAN_MAX_LEN);
    //dictAddWord(dict, word, meaning);
    wordListAddWord(word, &dictWordList, &dictWordListSize);
    sprintf(notify1, "Added a word: '%s'", word);
}


void UI_Dict_DeleteWord(BTA * dict) {
    char word[WORD_MAX_LEN];
    printf("\n*** DELETE A WORD:\n");
    printf("Word: "); readLn(stdin, word, WORD_MAX_LEN);

    BTint value = 0;
    if (bfndky(dict, word, &value) != QNOKEY) { // found the word
        btdel(dict, word);
        wordListEmpty(&dictWordList, &dictWordListSize);
                // wordListRemoveWord(word, &dictWordList, &dictWordListSize);
                makeWordList(dict, &dictWordList, &dictWordListSize);
        sprintf(notify1, "Deleted a word: '%s'", word);
    } else {
        sprintf(notify1, "Cannot find this word to delete: '%s'", word);
    }

}