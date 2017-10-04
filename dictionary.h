#define WORD_MAX_LEN 200
#define MEAN_MAX_LEN 10000
#define LINE_MAX_LEN 10000

#define WORD_WC_MAX_LEN 200 // for widechar type
#define MEAN_WC_MAX_LEN 10000 // for widechar type
#define LINE_WC_MAX_LEN 10000

#ifndef ZKYLEN
#define ZKYLEN WORD_MAX_LEN
#endif

#include "btree.h"

void createDictionary(BTA *dict, char * notify);
int dictFindWord(BTA *dict, char * word, char * meaning);
int dictAddWord(BTA *dict, char * word, char * meaning);
char *rand_string(char *str, size_t size);
void createDictionaryRandom(BTA *dict);