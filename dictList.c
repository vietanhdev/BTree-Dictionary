#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

// init dictList
void dictListInit(dict_t * dictList, int * dictListSize) {
    if (dictList != NULL) {
        free(dictList);
    }
    dictList = malloc(0);
    *dictListSize = 0;
}

// open the dictList database file
void dictListOpen(dict_t * dictList, int * dictListSize, const char * dictListFilename) {
    FILE * dbFile;
    // try to open db file to read
    if ((dbFile = fopen(dictListFilename, "r")) == NULL) {
        // if file is not exist, create a new file
        if ((dbFile = fopen(dictListFilename, "w")) == NULL) {

            // Init an empty dictionary list
            dictListInit(dictList, dictListSize);
        }
    } else {

        // Init an empty dictionary list
        dictListInit(dictList, dictListSize);

        // tmp variable for reading dictionary db
        dict_t dict;
        while (fread(&dict, 1, sizeof(dict_t), dbFile) > 0) {
            (*dictListSize)++;
            if ((dictList = realloc(dictList, (*dictListSize) * sizeof (dict_t) )) == NULL) {
                printf("Cannot allocate more memory for dictionary list.\n");
                exit(1);
            }

            dictList[*dictListSize - 1] = dict;
        }

    }

    fclose(dbFile);
}

// save the dictList database file
void dictListSave(dict_t * dictList, int dictListSize, const char * dictListFilename) {
    FILE * dbFile;
    if ((dbFile = fopen(dictListFilename, "w")) == NULL) {
        printf("Cannot open dictionary list. Filename : %s\n", dictListFilename);
        exit(1);
    }

    fwrite(dictList, sizeof(dict_t), dictListSize * sizeof(dict_t), dbFile);
    fclose(dbFile);
}

// add a dictionary to list
void dictListAddDict(dict_t dict, dict_t * dictList, int * dictListSize) {
    dictList = realloc(dictList, sizeof(dict_t) * (*dictListSize + 1));
    dictList[(*dictListSize)++] = dict;
}


// remove a dictionary from list
void dictListRemoveDict(dict_t * dictList, int * dictListSize, int dictID) {
    int i;

    if (dictID < *dictListSize) {
        for (i = dictID+1; i < *dictListSize; ++i) {
            dictList[dictID-1] = dictList[dictID];
        }

        // resize the memory
        (*dictListSize)--;
        if ((dictList=realloc(dictList, sizeof(dict_t) * (*dictListSize))) == NULL) {
            printf("A problem happened with memory allocation.\n");
            exit(1);
        }
    }
}