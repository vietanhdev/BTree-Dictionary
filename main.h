// BTree dictionary variables

// dictionary list manage
const char dictListFilename[] = "dictList.dat";
dict_t * dictList;
int dictListSize;
dict_t currentDict; // Current selected dict

// current word showing on meaning box
char currentWord[WORD_MAX_LEN];

// GTK variables
GtkBuilder      *builder; 
GtkWidget       *main_window;

GtkEntry *lookupEntry;
GtkTextView *meaningView;
GtkTextBuffer  *meaningViewBuff;

GtkDialog * loadDictPromptDialog;
