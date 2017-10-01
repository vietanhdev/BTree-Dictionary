#define WORD_MAX_LEN 50
#define MEAN_MAX_LEN 1000
#define LINE_MAX_LEN 200

#define WORD_WC_MAX_LEN 50 // for widechar type
#define MEAN_WC_MAX_LEN 1000 // for widechar type
#define LINE_WC_MAX_LEN 200

#ifndef ZKYLEN
#define ZKYLEN WORD_MAX_LEN
#endif

#include "btree.h"

void createDictionary(BTA *dict, char * notify);
int dictFindWord(BTA *dict, char * word, char * meaning);
int dictAddWord(BTA *dict, char * word, char * meaning);