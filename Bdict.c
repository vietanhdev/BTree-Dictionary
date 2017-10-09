#include <locale.h>
#include <gtk/gtk.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#define ZKYLEN WORD_MAX_LEN
#include "btree.h"

#include "extio.h"
#include "UI.h"


GtkBuilder      *builder; 
GtkWidget       *main_window;

GtkEntry *lookupEntry;
GtkTextView *meaningView;
GtkTextBuffer  *meaningViewBuff;


BTA *dict;

void quit() {
    // clear();
    // UI_InfoBoard();
    btcls(dict);
    exit(0);
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

    char notify[100];
    if (dict == NULL) createDictionary(&dict, notify);



    gtk_init(&argc, &argv);

    meaningViewBuff = gtk_text_buffer_new(NULL);    
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "GUI/main.glade", NULL);
 
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    gtk_builder_connect_signals(builder, NULL);
 

    lookupEntry = GTK_ENTRY(gtk_builder_get_object(builder, "lookup-entry"));
    meaningView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "meaning-view"));
    gtk_text_view_set_buffer (meaningView, meaningViewBuff);
    gtk_text_buffer_set_text(meaningViewBuff, "meaning", -1);


    g_object_unref(builder);
 
    gtk_widget_show(main_window);                
    gtk_main();


    return 0;
}


void on_lookup_entry_activate() {
    const gchar * searchEntryText = gtk_entry_buffer_get_text (gtk_entry_get_buffer (lookupEntry));

    char meaning[MEAN_MAX_LEN];
    int searchResult = dictFindWord(dict, searchEntryText, meaning);
    if (searchResult != 0) {
        gtk_text_buffer_set_text(meaningViewBuff, "Not found!", -1);
    } else {
        gtk_text_buffer_set_text(meaningViewBuff, meaning, -1);
    }
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
    quit();
}
