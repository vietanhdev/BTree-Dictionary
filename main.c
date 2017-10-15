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


// Create a initial database when no database is avaiable
void createInitDB() {
    dict_t dict;
    createDictionaryDBFromText(&dict, "E-V dict.", "EV_text_dict.txt", "BTree_dict.dat", meaningViewBuff);
    dictListAddDict(dict, &dictList, &dictListSize);
    dictListUpdateSelector(dictList, dictListSize, dictSelector);
    gtk_combo_box_set_active (GTK_COMBO_BOX(dictSelector), 0);
    dictListSave(dictList, dictListSize, dictListFilename);
    currentDict = dict;
    wordListBuild();
}

gboolean lookupEntryMatchFunc(GtkEntryCompletion *completion,
    const gchar *key,
    GtkTreeIter *iter,
    gpointer user_data) {
    GtkTreeModel *model = gtk_entry_completion_get_model(completion);
    gchar *item;
    gtk_tree_model_get(model, iter, 0, &item, -1);
    gboolean ans = (suggestWordCmp((char *)key, (char *)item) == 1) ? TRUE : FALSE;
    g_free(item);
    return ans;
}


void wordListBuild() {
    int i;
    BTint value;
    GtkTreeIter iter;

    char word[WORD_MAX_LEN];
    btpos(currentDict.dict, ZSTART);
    while(bnxtky(currentDict.dict, word, &value) == 0) {
        gtk_list_store_append(lookupEntryWordList, &iter);
        gtk_list_store_set(lookupEntryWordList, &iter, 0, word, -1);
    }

}


int main(int argc, char *argv[])
{
    char *locale;
    locale = setlocale(LC_ALL, "");

    btinit();

    gtk_init(&argc, &argv);

    meaningViewBuff = gtk_text_buffer_new(NULL);    
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "GUI/main.glade", NULL);
 
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    gtk_builder_connect_signals(builder, NULL);

    NULLnotifyBuff = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "NULL-buffer"));

    // Lookup GUI
    lookupEntry = GTK_ENTRY(gtk_builder_get_object(builder, "lookup-entry"));
    meaningView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "meaning-view"));
    gtk_text_view_set_buffer (meaningView, meaningViewBuff);
    gtk_text_buffer_set_text(meaningViewBuff, "", -1);
    dictSelector = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "dict-selector"));


    // Word completion
    lookupEntryCompletion = gtk_entry_completion_new();
    lookupEntryWordList = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;


    gtk_entry_completion_set_model(lookupEntryCompletion, GTK_TREE_MODEL(lookupEntryWordList));
    gtk_entry_completion_set_match_func(lookupEntryCompletion, (GtkEntryCompletionMatchFunc)lookupEntryMatchFunc, NULL, NULL);
    gtk_entry_set_completion(GTK_ENTRY(lookupEntry), lookupEntryCompletion);
    gtk_entry_completion_set_text_column(lookupEntryCompletion, 0);


    // Word editing GUI
    wordEditWindow = GTK_WIDGET(gtk_builder_get_object(builder, "word-edit"));
    wordEditWordEntry = GTK_ENTRY(gtk_builder_get_object(builder, "word-edit-word"));
    wordEditMeaningTextView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "word-edit-meaning"));
    wordEditMeaningBuff = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "word-edit-meaning-buff"));

    // Word deleting GUI
    wordDeletePromptDialog = GTK_DIALOG(gtk_builder_get_object(builder, "del-current-word-prompt"));


    // Dict initial loading
    loadDictPromptDialog = GTK_DIALOG(gtk_builder_get_object(builder, "load-dict-prompt"));

    // Initialize the dictionary list
    dictListOpen(&dictList, &dictListSize, dictListFilename, dictSelector);
    if (dictListSize > 0) {
        currentDict = dictList[0];
    }


    // Load the dictionary
    if (dictListSize == 0) {
        gtk_widget_show(GTK_WIDGET(loadDictPromptDialog));
    }

    
    gtk_widget_show(main_window);
    //g_object_unref(builder);
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


void on_delete_btn_yes() {
    gtk_widget_hide(GTK_WIDGET(wordDeletePromptDialog));
    dictDelWord(currentDict.dict, currentWord, meaningViewBuff);
    wordListBuild();
}

void on_delete_btn_no() {
    gtk_widget_hide(GTK_WIDGET(wordDeletePromptDialog));
}

// Handle delete button
void on_delete_btn() {
    if (strlen(currentWord) > 0) {
        gtk_widget_show(GTK_WIDGET(wordDeletePromptDialog));
    }
}

void on_edit_save() {
    GtkTextIter start;
    GtkTextIter end;

    char word[WORD_MAX_LEN];
    char meaning[MEAN_MAX_LEN];

    gtk_widget_hide(GTK_WIDGET(wordEditWindow));

    gtk_text_buffer_get_start_iter (wordEditMeaningBuff, &start);
    gtk_text_buffer_get_end_iter (wordEditMeaningBuff, &end);

    strcpy(word, (char*)gtk_entry_get_text (GTK_ENTRY(wordEditWordEntry)));
    strcpy(meaning, (char*)gtk_text_buffer_get_text (wordEditMeaningBuff, &start, &end, 1));

    if (wordEditMode == 0) { // edit word
        // Delete original word and add new edited word
        dictDelWord(currentDict.dict, wordEditOrigin, NULLnotifyBuff);
        gtk_text_buffer_set_text(meaningViewBuff, "Edited: ", -1);
    } else { // add word
        gtk_text_buffer_set_text(meaningViewBuff, "Added: ", -1);
    }
    dictAddWord(currentDict.dict, word, meaning, meaningViewBuff);
    
    gtk_text_buffer_insert_at_cursor (meaningViewBuff, word, -1);

    wordListBuild();
}

void on_edit_cancel() {
    gtk_widget_hide(GTK_WIDGET(wordEditWindow));
}

void on_edit_btn() {
    GtkTextIter start;
    GtkTextIter end;
    gchar * wordMeaning;

    if (strlen(currentWord) > 0) {
        wordEditMode = 0; // editing mode

        gtk_window_set_title (GTK_WINDOW(wordEditWindow), "Editing current word");

        gtk_entry_set_text (GTK_ENTRY(wordEditWordEntry), currentWord);
        strcpy(wordEditOrigin, currentWord);

        gtk_text_buffer_get_start_iter (meaningViewBuff, &start);
        gtk_text_buffer_get_end_iter (meaningViewBuff, &end);

        wordMeaning = gtk_text_buffer_get_text (meaningViewBuff, &start, &end, 1);

        gtk_text_buffer_set_text (wordEditMeaningBuff,  wordMeaning, strlen(wordMeaning));
        
        gtk_widget_show(GTK_WIDGET(wordEditWindow));
    }
}


void on_add_btn() {
    wordEditMode = 1; // adding mode
    gtk_window_set_title (GTK_WINDOW(wordEditWindow), "Add a word");
    gtk_widget_show(GTK_WIDGET(wordEditWindow));
}


// Change current dict when user update the selector box
void on_current_dict_change() {
    char * dictName = gtk_combo_box_text_get_active_text (dictSelector);
    int i;
    for (i = 0; i < dictListSize; i++) {
        if (strcmp(dictName, dictList[i].name) == 0) {
            currentDict = dictList[i];
            break;
        }
    }
    wordListBuild();
}


// When starting the dictionary, if a BTree database is not available, use will be prompt to create new database from text file.
// This func. is responsible for process "Yes" button. 
void on_load_dict_yes (GtkWidget *widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(loadDictPromptDialog));
    createInitDB();
}
// This func. is responsible for process "No" button.
void on_load_dict_no (GtkWidget *widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(loadDictPromptDialog));
}

// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}
