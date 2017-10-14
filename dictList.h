#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

// Manage the dictionary list

void dictListInit(dict_t * dictList, int * dictListSize);
void dictListOpen(dict_t * dictList, int * dictListSize, const char * dictListFilename);
void dictListSave(dict_t * dictList, int dictListSize, const char * dictListFilename);
void dictListAddDict(dict_t dict, dict_t * dictList, int * dictListSize);
void dictListRemoveDict(dict_t * dictList, int * dictListSize, int dictID);