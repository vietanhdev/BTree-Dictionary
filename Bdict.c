#include <locale.h>
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


void searchBuff_Suggest() {
    wordListSuggest(suggestBox, searchBox, dictWordList, dictWordListSize);
}

void searchBuff_add(char c) {
    if (isprint(c)) {
        sprintf(searchBox, "%s%c", searchBox, c);
        searchBuff_Suggest();
    }
}

void searchBuff_backspace() {
    if (strlen(searchBox) > 0) {
        searchBox[strlen(searchBox)-1] = '\0';
        searchBuff_Suggest();
    } 
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

void printAllWords(BTA * dict) {
    int rsize;

    char word[WORD_MAX_LEN];
    char meaning[MEAN_MAX_LEN];

    btpos(dict, ZSTART);
    while(btseln(dict, word, meaning, MEAN_MAX_LEN, &rsize) == 0) {
        printf("%s\t%s\n",  word, meaning);   
    }
    getch();
}


int main(int argc, char const *argv[])
{


    char *locale;

    locale = setlocale(LC_ALL, "");


    // Load the dictionary
    printf("Loading data ...\n");
    btinit();
    dict = btopn("BTree_dict.dat", 0, FALSE);
    if (dict != NULL) makeWordList(dict, &dictWordList, &dictWordListSize);

    //printAllWords(dict);

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