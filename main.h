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

GtkEntry *lookupEntry;
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