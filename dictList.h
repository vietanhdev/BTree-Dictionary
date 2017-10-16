#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

// Manage the dictionary list

// Initialize dict list for program
void dictListInit(dict_t ** dictList, int * dictListSize, const char * dictListFilename, GtkComboBoxText *dictSelector);

// Malloc / Empty memory for dict list
void dictListEmpty(dict_t ** dictList, int * dictListSize);

// Open a dict list or create a new one if not exist
void dictListOpen(dict_t ** dictList, int * dictListSize, const char * dictListFilename, GtkComboBoxText *dictSelector);

// Save dict list to file
void dictListSave(dict_t * dictList, int dictListSize, const char * dictListFilename);

// Add a dictionary to dict list
// return 0 if successful
int dictListAddDict(dict_t dict, dict_t ** dictList, int * dictListSize);

// Remove a dict from dict list using id (0->dictListSize);
void dictListRemoveDict(dict_t ** dictList, int * dictListSize, int dictID);

void dictListRemoveDictByNameAndPath(dict_t ** dictList, int * dictListSize, char * dictName, char * dictPath);


// Update dict. selector on GUI
void dictListUpdateSelector(dict_t * dictList, int dictListSize, GtkComboBoxText *dictSelector);


