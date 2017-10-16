// BTree dictionary variables


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


void createInitDB();

gboolean lookupEntryMatchFunc(GtkEntryCompletion *completion,
    const gchar *key,
    GtkTreeIter *iter,
    gpointer user_data);


void wordListBuild();

int on_lookup_match_selected(GtkEntryCompletion *widget,
               GtkTreeModel       *model,
               GtkTreeIter        *iter,
               gpointer            user_data);