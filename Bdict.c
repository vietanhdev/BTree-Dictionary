#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#define ZKYLEN WORD_MAX_LEN
#include "btree.h"

#include "extio.h"
#include "UI.h"


BTA *dict;


void quit() {
    clear();
    UI_InfoBoard();
    printf("Goodbye! See you later...\n");
    btcls(dict);
}

void searchBuff_add(char c) {
    if (isalpha(c) || isdigit(c)) {
        sprintf(searchBox, "%s%c", searchBox, c);
    }
}

void searchBuff_backspace() {
    if (strlen(searchBox) > 0)
        searchBox[strlen(searchBox)-1] = '\0';
}

void searchBuff_search() {
    if (strlen(searchBox) == 0) return;
    char meaning[MEAN_MAX_LEN];
    int searchResult = dictFindWord(dict, searchBox, meaning);
    if (searchResult != 0) {
        UI_Notify1_Push("This word this not in my dictionary.");
    } else {
        clear();
        UI_InfoBoard();
        printf("[[[ "); printUpper(searchBox); printf(" ]]]");
        printf("%s\n", meaning);
        printf("Press any key to exit word view...\n");
        getch();
        strcpy(searchBox, "");
    }
}


int main(int argc, char const *argv[])
{

    btinit();
    dict = btopn("BTree_dict.dat", 0, FALSE);

    char c;
    while (1) {
        UI_InfoBoard();
        UI_Notify();
        UI_Search(searchBox);

        c = getch();
        
        switch(c) {
            case 27: quit(); return 0; // ESC to exit
            case 93: UI_Menu(dict); break; // ] for menu
            case '\n': searchBuff_search(); break; // Enter to search
            case 127: searchBuff_backspace(); break; // Backspace
            default: searchBuff_add(c);

        }

    }
    return 0;
}