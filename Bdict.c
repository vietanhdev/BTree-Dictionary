#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#define ZKYLEN WORD_MAX_LEN
#include "btree.h"

#include "extio.h"
#include "UI.h"

#define NOTIFY_MAX_LEN 2000



BTA *dict;

char searchBox[WORD_MAX_LEN] = "";
char notify1[NOTIFY_MAX_LEN] = "";
char notify2[NOTIFY_MAX_LEN] = "";
char meaningArea[MEAN_MAX_LEN] = "";


void quit() {
    clear();
    UI_InfoBoard();
    printf("Goodbye! See you later...\n");
    btcls(dict);
}

void searchBuff_add(char c) {
    //printf("%d\n", c);
    //getch();
    if (isalpha(c) || isdigit(c)) {
        sprintf(searchBox, "%s%c", searchBox, c);

    }
}

void searchBuff_backspace() {
    if (strlen(searchBox) > 0)
        searchBox[strlen(searchBox)-1] = '\0';
}

void searchBuff_search() {
    char meaning[MEAN_MAX_LEN];
    int searchResult = dictFindWord(dict, searchBox, meaning);
    if (searchResult != 0) {
        UI_Notify1_Push("This word this not in my dictionary.");
    } else {
        strcpy(meaningArea, meaning);
    }
}

int main(int argc, char const *argv[])
{

    btinit();
    btopn("BTree_dict.dat", 0, 0);
    UI_init(dict, meaningArea, notify1, notify2);

    char c;
    while (1) {
        UI_InfoBoard();
        UI_Notify();
        UI_Search(searchBox);

        c = getch();
        
        switch(c) {
            case 27: quit(); return 0; // ESC to exit
            case 93: UI_Menu(); break; // ] for menu
            case '\n': searchBuff_search(); break; // Enter to search
            case 127: searchBuff_backspace(); break; // Backspace
            default: searchBuff_add(c);
        }

    }
    return 0;
}