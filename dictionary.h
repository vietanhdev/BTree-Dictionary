#include <gtk/gtk.h>

#define WORD_MAX_LEN 200
#define MEAN_MAX_LEN 40000
#define LINE_MAX_LEN 40000

#define WORD_WC_MAX_LEN 200 // for widechar type
#define MEAN_WC_MAX_LEN 40000 // for widechar type
#define LINE_WC_MAX_LEN 40000

#define SUGGEST_BOX_MAX_LEN 10000
#define SUGGEST_WORD_NUM 5

#define DICTNAME_MAX_LEN 200
#define DICTPATH_MAX_LEN 500

#ifndef ZKYLEN
#define ZKYLEN WORD_MAX_LEN
#endif

#include "btree.h"

extern char ** dictWordList;
extern int dictWordListSize;


int dictFindWord(BTA *dict, const char * word, char * meaning);
void createDictionary(const char * textFileName, const char * dictFileName, BTA ** dict, GtkTextBuffer  *notifyBuff);
void makeWordList(BTA * dict, char *** wordList, int * wordListSize);
void wordListAddWord(char * word, char *** wordList, int * wordListSize);
//void wordListRemoveWord(char * word, char *** wordList, int * wordListSize);
void wordListEmpty(char *** wordList, int * wordListSize);
int wordListSuggest(char * suggestStr, char * str, char ** wordList, int wordListSize);
int suggestWordCmp(char * str, char * word);

