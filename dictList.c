#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include "dictList.h"
#include <string.h>


void dictListUpdateSelector(dict_t * dictList, int dictListSize, GtkComboBoxText *dictSelector) {
    int i;
    gtk_combo_box_text_remove_all(dictSelector);

    for (i = 0; i < dictListSize; ++i) {
        printf("Add dict.: %s\n", dictList[i].name);
        gtk_combo_box_text_insert (dictSelector, i, dictList[i].name, dictList[i].name);
    }

}

// init dictList
void dictListInit(dict_t ** dictList, int * dictListSize, const char * dictListFilename, GtkComboBoxText *dictSelector) {
    dictListEmpty(dictList, dictListSize);
    dictListSave(*dictList, *dictListSize, dictListFilename);
    dictListUpdateSelector(*dictList, *dictListSize, dictSelector);
}


void dictListEmpty(dict_t ** dictList, int * dictListSize) {
    if (*dictList != NULL) {
        free(*dictList);
    }
    *dictList = malloc(0);
    *dictListSize = 0;

}

// open the dictList database file
void dictListOpen(dict_t ** dictList, int * dictListSize, const char * dictListFilename, GtkComboBoxText *dictSelector) {
    int i;

    FILE * dbFile;
    // try to open db file to read
    if ((dbFile = fopen(dictListFilename, "r")) == NULL) {
        // Init an empty dictionary list
        dictListInit(dictList, dictListSize, dictListFilename, dictSelector);
    } else {

        // Init an empty dictionary list
        dictListEmpty(dictList, dictListSize);

        // tmp variable for reading dictionary db
        dict_t dict;
        while (!feof(dbFile) && fread(&dict, sizeof(dict_t), 1, dbFile) > 0) {
            dictListAddDict(dict, dictList, dictListSize);
        }
    }

    if (dbFile != NULL) fclose(dbFile);

    dictListUpdateSelector(*dictList, *dictListSize, dictSelector);
    if (*dictListSize > 0) {
        gtk_combo_box_set_active (GTK_COMBO_BOX(dictSelector), 0);
    }
}

// save the dictList database file
void dictListSave(dict_t * dictList, int dictListSize, const char * dictListFilename) {
    FILE * dbFile;
    if ((dbFile = fopen(dictListFilename, "w")) == NULL) {
        printf("Cannot open dictionary list. Filename : %s\n", dictListFilename);
        exit(1);
    }

    fwrite(dictList, sizeof(dict_t), dictListSize, dbFile);
    fclose(dbFile);
}

// add a dictionary to list
void dictListAddDict(dict_t dict, dict_t ** dictList, int * dictListSize) {

    dictOpen(&(dict));

    *dictList = realloc(*dictList, sizeof(dict_t) * (*dictListSize + 1));
    if (*dictList == NULL) {
        printf("Cannot allocate more memory for dictList.\n");
        exit(1);
    }

    (*dictList)[(*dictListSize)++] = dict;


}


// remove a dictionary from list
void dictListRemoveDict(dict_t ** dictList, int * dictListSize, int dictID) {
    int i;
    dict_t * newDictList;

    if (dictID < *dictListSize) {
        // Close dictionary
        dictClose(&((*dictList)[dictID]));

        for (i = dictID+1; i < *dictListSize; ++i) {
            (*dictList)[dictID-1] = (*dictList)[dictID];
        }
    

        // resize the memory
        (*dictListSize)--;

        newDictList = (dict_t*)malloc(sizeof(dict_t) * (*dictListSize));

        printf("Pass\n");
        if (newDictList == NULL && *dictListSize != 0) {
            printf("A problem happened with memory allocation.\n");
            exit(1);
        } else {
            for (i = 0; i < *dictListSize; i++) {
                newDictList[i] = (*dictList)[i];
            }
            //free(*dictList);
            *dictList = newDictList;
        }

        printf("Pass\n");

    }
}


void dictListRemoveDictByNameAndPath(dict_t ** dictList, int * dictListSize, char * dictName, char * dictPath) {
    int i;

    // exception handle
    if (dictName == NULL || dictPath == NULL) return;

    for (i = 0; i < *dictListSize; i++) {
        if (strcmp(dictName, (*dictList)[i].name) == 0 && strcmp(dictPath, (*dictList)[i].path) == 0) {
            dictListRemoveDict(dictList, dictListSize, i);
        }
    }
}