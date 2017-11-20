// BTree dictionary variables
#ifndef MAIN_H
#define MAIN_H

// dictionary list manage
char dictListFilename[] = "dictList.dat";
dict_t * dictList;
int dictListSize;
dict_t currentDict; // Current selected dict

// current word showing on meaning box
char currentWord[WORD_MAX_LEN];

// GTK variables

// Buffer to use if the function requires a buffer to push notifications
// and you dont want to show these notification
GtkTextBuffer  *NULLnotifyBuff;

GtkBuilder      *builder; 
GtkWidget       *main_window;

// Lookup
GtkEntry *lookupEntry;
GtkEntryCompletion *lookupEntryCompletion;
GtkListStore *lookupEntryWordList;

// Meaning view
GtkTextView *meaningView;
GtkTextBuffer  *meaningViewBuff;

GtkDialog * loadDictPromptDialog;

GtkComboBoxText * dictSelector;

// Word editing box
GtkWidget * wordEditWindow;
GtkEntry * wordEditWordEntry;
GtkTextView * wordEditMeaningTextView;
GtkTextBuffer *wordEditMeaningBuff;
char wordEditOrigin[WORD_MAX_LEN];
int wordEditMode; //  = 0: edit a word on dictionary; = 1: add a word to dictionary


// Word deleting
GtkDialog * wordDeletePromptDialog;


// About this software
GtkWidget * aboutWindow;


// Dict. manager
GtkWidget * dictManagerWindow;
enum
{
   NAME_COLUMN,
   PATH_COLUMN,
   N_COLUMNS
};
GtkTreeView *dictManagerTreeView;
GtkListStore *dictManagerDictList;
GtkTreeSelection *dictManagerSelection;


GtkWidget * dictManagerAddDictForm;
GtkEntry * dictManagerAddDictName;
GtkEntry * dictManagerAddDictPath;

GtkWidget * dictManagerOpeningFailMsgbox;


// Init a database from txt file if the program's db is empty
void createInitDB();



// Match function for lookup completion
gboolean lookupEntryMatchFunc(GtkEntryCompletion *completion,
                                    const gchar *key,
                                    GtkTreeIter *iter,
                                    gpointer user_data);

// Build a word list for lookup completion
void wordListBuild();


// Lookup a word in dictionary
void dictLookup(dict_t dict, const char * word, GtkTextBuffer * meaningViewBuff, char * currentWord);
void dictLookupNext(dict_t dict, char * currentWord, GtkTextBuffer * meaningViewBuff);
void dictLookupPrev(dict_t dict, char * currentWord, GtkTextBuffer * meaningViewBuff);




// SIGNAL HANDLERS : HANDLE GTK GUI SIGNALS


// On starting, if the db is empty, the program will give the user a prompt
// load initial database or not. Two below funcs will handle it.
void on_load_dict_yes (GtkWidget *widget, gpointer data);
void on_load_dict_no (GtkWidget *widget, gpointer data);

// on selecting a word in completion list
int on_lookup_match_selected(GtkEntryCompletion *widget,
               GtkTreeModel       *model,
               GtkTreeIter        *iter,
               gpointer            user_data);

// Next button
void on_next_word_btn();


// Previous button
void on_prev_word_btn();


// Press enter to search on lookup field
void on_lookup_entry_activate();


// Change dictionary using selection box
void on_current_dict_change();


// Handle signals in word deleting function
void on_delete_btn();
void on_delete_btn_yes();
void on_delete_btn_no();

// Handle signals in word editing function
void on_edit_btn();
void on_edit_save();
void on_edit_cancel();

// Handle signals in word adding function
void on_add_btn();


// About this program
void on_about_btn();
void on_about_close_btn();


// Handle signals in dictionary manager
void on_dict_manage_btn();
void dict_manager_update_dictlist();
void dict_manager_delete_selected(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer userdata);
void on_dict_manager_delete_btn();
void on_dict_manager_close();
void dict_manager_opening_error_hide();

void on_dict_manager_add_dict_btn(); // Click add button in dict. manager
void on_dict_manager_add_dict_cancel();
void on_dict_manager_add_dict_action(); // Click add button when adding a dictionary


void on_window_main_destroy();



#endif