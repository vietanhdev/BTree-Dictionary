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


// Match function for lookup completion
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

    GtkTreeIter iter; // iter for completion list

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


    gtk_entry_completion_set_model(lookupEntryCompletion, GTK_TREE_MODEL(lookupEntryWordList));
    gtk_entry_completion_set_match_func(lookupEntryCompletion, (GtkEntryCompletionMatchFunc)lookupEntryMatchFunc, NULL, NULL);
    gtk_entry_set_completion(GTK_ENTRY(lookupEntry), lookupEntryCompletion);
    gtk_entry_completion_set_text_column(lookupEntryCompletion, 0);

    g_signal_connect(lookupEntryCompletion, "match-selected", G_CALLBACK(on_lookup_match_selected), NULL);


    // Word editing GUI
    wordEditWindow = GTK_WIDGET(gtk_builder_get_object(builder, "word-edit"));
    wordEditWordEntry = GTK_ENTRY(gtk_builder_get_object(builder, "word-edit-word"));
    wordEditMeaningTextView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "word-edit-meaning"));
    wordEditMeaningBuff = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "word-edit-meaning-buff"));

    // Word deleting GUI
    wordDeletePromptDialog = GTK_DIALOG(gtk_builder_get_object(builder, "del-current-word-prompt"));




    // Dict. manager
    dictManagerWindow = GTK_WIDGET(gtk_builder_get_object(builder, "dict-manager-window"));
    
    dictManagerTreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "dict-list-view"));
    // create a list to store dictionary on GUI
    dictManagerDictList = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model (dictManagerTreeView, GTK_TREE_MODEL(dictManagerDictList));

    dictManagerSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dictManagerTreeView));

    // show the dict list model on list view
    GtkTreeViewColumn* column;
    column = gtk_tree_view_column_new_with_attributes("Name",
                                      gtk_cell_renderer_text_new(),
                                      "text", NAME_COLUMN,
                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(dictManagerTreeView), column);
    column = gtk_tree_view_column_new_with_attributes("Path",
                                      gtk_cell_renderer_text_new(),
                                      "text", PATH_COLUMN,
                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(dictManagerTreeView), column);


    // adding dict. form
    dictManagerAddDictForm = GTK_WIDGET(gtk_builder_get_object(builder, "dict-manager-add-dict-form"));
    dictManagerAddDictName = GTK_ENTRY(gtk_builder_get_object(builder, "dict-manager-add-dict-name"));
    dictManagerAddDictPath = GTK_ENTRY(gtk_builder_get_object(builder, "dict-manager-add-dict-path"));

    dictManagerOpeningFailMsgbox = GTK_WIDGET(gtk_builder_get_object(builder, "dict-add-error-msg"));
    

    // Dict initial 
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


    aboutWindow = GTK_WIDGET(gtk_builder_get_object(builder, "about-window"));
    
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

// Lookup a match result from lookup field completion
int on_lookup_match_selected(GtkEntryCompletion *widget,
               GtkTreeModel       *model,
               GtkTreeIter        *iter,
               gpointer            user_data) {
    gchar *word;
    gtk_tree_model_get(model, iter, 0, &word, -1);

    // update lookup field
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(lookupEntry), word, -1);

    dictLookup(currentDict, word, meaningViewBuff, currentWord);
    return TRUE;
}



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
    } else {
        gtk_text_buffer_set_text(meaningViewBuff, "Enter a word on lookup field and click delete button to delete it from dictionary.", -1);
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
    } else {
        gtk_text_buffer_set_text(meaningViewBuff, "Lookup a word first to edit it!", -1);
    }
}


void on_add_btn() {

    // if there is no dictionary loaded
    if (currentDict.dict == NULL) {
        gtk_text_buffer_set_text(meaningViewBuff, "There is no dictionary loaded", -1);
        return;
    };


    wordEditMode = 1; // adding mode
    gtk_window_set_title (GTK_WINDOW(wordEditWindow), "Add a word");
    gtk_widget_show(GTK_WIDGET(wordEditWindow));
    gtk_entry_set_text (GTK_ENTRY(wordEditWordEntry), "");
    gtk_text_buffer_set_text (wordEditMeaningBuff,  "", -1);
}


// Change current dict when user update the selector box
void on_current_dict_change() {
    char * dictName = gtk_combo_box_text_get_active_text (dictSelector);

    // handle exception
    if (dictName == NULL) return;

    int i;
    for (i = 0; i < dictListSize; i++) {
        if (strcmp(dictName, dictList[i].name) == 0) {
            currentDict = dictList[i];
            break;
        }
    }
    wordListBuild();
}


void on_about_close_btn() {
    gtk_widget_hide(GTK_WIDGET(aboutWindow));
}


// Click about (info) button
void on_about_btn() {
    gtk_widget_show(GTK_WIDGET(aboutWindow));
}


// Dict. manager

void dict_manager_update_dictlist() {
    int i;
    GtkTreeIter iter;

    // clear old dict list model
    gtk_list_store_clear(dictManagerDictList);

    // build a new dict list model
    for (i = 0; i < dictListSize; i++) {
        gtk_list_store_insert_with_values(dictManagerDictList, NULL, -1,
            NAME_COLUMN, dictList[i].name,
            PATH_COLUMN, dictList[i].path,
            -1);
    }

}

void on_dict_manage_btn() {
    dict_manager_update_dictlist();
    gtk_widget_show(GTK_WIDGET(dictManagerWindow));
}

// foreach  deleting selected dict.
void dict_manager_delete_selected(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer userdata) {
    char *dictName;
    char *dictPath;


    // Delete dict. from the list
    gtk_tree_model_get(model, iter, NAME_COLUMN, &dictName, -1);
    gtk_tree_model_get(model, iter, PATH_COLUMN, &dictPath, -1);
    dictListRemoveDictByNameAndPath(&dictList, &dictListSize, dictName, dictPath);
    

    // Save to disk
    dictListSave(dictList, dictListSize, dictListFilename);


    // Update dict selector for GUI
    dictListUpdateSelector(dictList, dictListSize, dictSelector);
    gtk_combo_box_set_active (GTK_COMBO_BOX(dictSelector), 0);
    

    // update current dict.
    if (dictListSize > 0) {
        currentDict = dictList[0];
        wordListBuild();
    } else {
        currentDict.dict = NULL;
    }


    // update dict. list on dict manager window
    dict_manager_update_dictlist();

    // Free memory
    g_free(dictName);
    g_free(dictPath);
}
void on_dict_manager_delete_btn() {
    gtk_tree_selection_selected_foreach(dictManagerSelection, dict_manager_delete_selected, NULL);
}

// close the dict. manager
void on_dict_manager_close() {
    gtk_widget_hide(GTK_WIDGET(dictManagerWindow));
}




void dict_manager_opening_error_hide() {
    gtk_widget_hide(GTK_WIDGET(dictManagerOpeningFailMsgbox));
}

// Cancel dict. adding
void on_dict_manager_add_dict_cancel() {
    gtk_widget_hide(GTK_WIDGET(dictManagerAddDictForm));
}

void on_dict_manager_add_dict_action() {
    dict_t dict;
    char * str;

    str = gtk_entry_get_text (dictManagerAddDictName);
    strcpy(dict.name, str);
    str = gtk_entry_get_text (dictManagerAddDictPath);
    strcpy(dict.path, str);

    if (dictListAddDict(dict, &dictList, &dictListSize) != 0) {
        gtk_widget_show(GTK_WIDGET(dictManagerOpeningFailMsgbox));
        return;
    };

    // Save to disk
    dictListSave(dictList, dictListSize, dictListFilename);

    // Update dict selector for GUI
    dictListUpdateSelector(dictList, dictListSize, dictSelector);
    gtk_combo_box_set_active (GTK_COMBO_BOX(dictSelector), 0);


    // update dict. list on dict manager window
    dict_manager_update_dictlist();

    on_dict_manager_add_dict_cancel();
}

// Add a dictionary button
void on_dict_manager_add_dict_btn() {
    gtk_entry_set_text (dictManagerAddDictName, "");
    gtk_entry_set_text (dictManagerAddDictPath, "");
    gtk_widget_show(GTK_WIDGET(dictManagerAddDictForm));
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
