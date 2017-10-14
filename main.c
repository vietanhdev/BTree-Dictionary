#include <locale.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extio.h"
#include "dictionary.h"
#define ZKYLEN WORD_MAX_LEN
#include "btree.h"
#include "dictList.h"
#include "main.h"



void createTestDB() {
    dictListInit(dictList, &dictListSize);
    dict_t dict;
    createDictionaryDBFromText(&dict, "E-V dict.", "EV_text_dict.txt", "BTree_dict.dat", meaningViewBuff);
    dictListAddDict(dict, dictList, &dictListSize);
    dictListSave(dictList, dictListSize, dictListFilename);
    currentDict = dict;
}


// Prototypes

int main(int argc, char const *argv[])
{
    char *locale;
    locale = setlocale(LC_ALL, "");

    gtk_init(NULL, NULL);

    meaningViewBuff = gtk_text_buffer_new(NULL);    
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "GUI/main.glade", NULL);
 
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    gtk_builder_connect_signals(builder, NULL);


    lookupEntry = GTK_ENTRY(gtk_builder_get_object(builder, "lookup-entry"));
    meaningView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "meaning-view"));
    gtk_text_view_set_buffer (meaningView, meaningViewBuff);
    gtk_text_buffer_set_text(meaningViewBuff, "", -1);


    // Load the dictionary
    btinit();
    //dict = btopn("BTree_dict.dat", 0, FALSE);
    //if (dict == NULL) {
        loadDictPromptDialog = GTK_DIALOG(gtk_builder_get_object(builder, "load-dict-prompt"));
        gtk_widget_show(GTK_WIDGET(loadDictPromptDialog));
    //}

    gtk_widget_show(main_window);
    g_object_unref(builder);
    gtk_main();
    return 0;
}


void dictLookup(dict_t dict, const char * word, GtkTextBuffer * meaningViewBuff, char * currentWord) {
    char meaning[MEAN_MAX_LEN];
    int searchResult = dictFindWord(dict, word, meaning);
    if (searchResult != 0) {
        gtk_text_buffer_set_text(meaningViewBuff, "Not found!", -1);
    } else {
        strcpy(currentWord, word);
        gtk_text_buffer_set_text(meaningViewBuff, meaning, -1);
    }
}

void dictLookupNext(dict_t dict, char * currentWord, GtkTextBuffer * meaningViewBuff) {
    char meaning[MEAN_MAX_LEN];
    int searchResult = dictFindNextWord(dict, currentWord, meaning);
    if (searchResult != 0) {
        gtk_text_buffer_set_text(meaningViewBuff, "There is no next word!", -1);
    } else {
        gtk_text_buffer_set_text(meaningViewBuff, meaning, -1);
    }
}

void dictLookupPrev(dict_t dict, char * currentWord, GtkTextBuffer * meaningViewBuff) {
    char meaning[MEAN_MAX_LEN];
    int searchResult = dictFindPrevWord(dict, currentWord, meaning);
    if (searchResult != 0) {
        gtk_text_buffer_set_text(meaningViewBuff, "There is no previous word!", -1);
    } else {
        gtk_text_buffer_set_text(meaningViewBuff, meaning, -1);
    }
}
 
// SIGNAL HANDLER - handle signals from GTK GUI

void on_next_word_btn() {
    dictLookupNext(currentDict, currentWord, meaningViewBuff);
    gtk_entry_buffer_set_text (gtk_entry_get_buffer (lookupEntry), currentWord, strlen(currentWord));
}

void on_prev_word_btn() {
    dictLookupPrev(currentDict, currentWord, meaningViewBuff);
    gtk_entry_buffer_set_text (gtk_entry_get_buffer (lookupEntry), currentWord, strlen(currentWord));
}

void on_lookup_entry_activate() {
    const gchar * searchEntryText = gtk_entry_buffer_get_text (gtk_entry_get_buffer (lookupEntry));
    dictLookup(currentDict, searchEntryText, meaningViewBuff, currentWord);
}



// When starting the dictionary, if a BTree database is not available, use will be prompt to create new database from text file.
// This func. is responsible for process "Yes" button. 
void loadDictPromptYes (GtkWidget *widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(loadDictPromptDialog));
    createTestDB();
}
// This func. is responsible for process "No" button.
void loadDictPromptNo (GtkWidget *widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(loadDictPromptDialog));
}

// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}
