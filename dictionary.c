#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <gtk/gtk.h>
#include "extio.h"
#include "dictionary.h"
#include "string_ext.h"


// This function create a BTree dictionary from a txt file
void createDictionaryDBFromText(dict_t * dict, char * dictName, char * textFileName, char * dbFileName, GtkTextBuffer  *notifyBuff) {

    strcpy(dict->name, dictName);
    strcpy(dict->path, dbFileName);

    gtk_text_buffer_set_text (notifyBuff, "Loading dictionary...\n", -1);

    int WordCount = 0; // count the number of words in dictionary

    btcls(dict->dict); // close if a dictionary file was opened. 
    // below line is required for operating correctly.

    dict->dict =  btcrt(dict->path, 0, FALSE);

    wchar_t * tmp_word; // widechar word
    wchar_t * tmp_meaning; // widechar meaning
    wchar_t * line;
    wchar_t * linePtr;

    char notify[MEAN_MAX_LEN];

    char * word;
    char * meaning;

    tmp_word = malloc(WORD_WC_MAX_LEN * sizeof(wchar_t));
    tmp_meaning = malloc(MEAN_WC_MAX_LEN * sizeof(wchar_t));

    word = malloc(WORD_MAX_LEN * sizeof(char));
    meaning = malloc(MEAN_MAX_LEN * sizeof(char));


    line = malloc(LINE_WC_MAX_LEN * sizeof(wchar_t));

    int i;
    FILE *f;
    if((f=fopen(textFileName,"r"))==NULL){
            gtk_text_buffer_insert_at_cursor (notifyBuff, "File open failed!\n", -1);
            return;
    } else {

            // Find the first word
            do {
                // Read a line in dict file
                linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
            } while(linePtr != NULL && line[0] != L'@');

            while(linePtr != NULL) {

                if (feof(f)) {
                    return;
                }

                if (wcslen(line) == 0) {
                    // Read a line in dict file
                    linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
                    continue;
                }

                // Split word
                for (i = 1; line[i] != L'/' && line[i] != L'\n' && line[i] != L'\0'; i++);
                wcsncpy(tmp_word, &line[1], i-1);
                tmp_word[i-1] = L'\0';


                // Split meaning
                wcsncpy(tmp_meaning, &line[i], wcslen(line) - i + 1);

                // Continuously read lines containing meaning
                // Read a line in dict file
                linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
                while (linePtr != NULL && line[0] != L'@') {
                    if (wcslen(tmp_meaning) + wcslen(line) > MEAN_MAX_LEN) {
                        sprintf(notify, "Meaning exceeded the maximum length. Word: %S\n", tmp_word);
                        gtk_text_buffer_insert_at_cursor (notifyBuff, notify, -1);
                        exit(1);
                    }
                    wcscat(tmp_meaning, line);
                    // Read a line in dict file
                    linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
                }


                // Convert from widechar strings to char strings
                wcstombs(word, tmp_word, WORD_MAX_LEN*sizeof(char));
                wcstombs(meaning, tmp_meaning, MEAN_MAX_LEN*sizeof(char));


                dictAddWord(dict->dict, word, meaning, notifyBuff);

                // print realtime info
                WordCount++;
                // sprintf(notify, "\33[2K\r> %d words. %s\0", WordCount, word);
                // gtk_text_buffer_insert_at_cursor (notifyBuff, notify, -1);
                
            }

    }
    fclose(f);

    sprintf(notify, "Loading done. %d words was loaded.", WordCount);
    gtk_text_buffer_insert_at_cursor (notifyBuff, notify, strlen(notify));


    //free used memory
    free(word); free(meaning);
    free(tmp_word); free(tmp_meaning);
    free(line);

    // Save the dictionary by re-opening
    sprintf(notify, "\nSaving file...\n");
    gtk_text_buffer_insert_at_cursor (notifyBuff, notify, -1);
    btcls(dict->dict);
    dict->dict = btopn(dbFileName, 0, FALSE);

    gtk_text_buffer_insert_at_cursor (notifyBuff, notify, strlen(notify));

    gtk_text_buffer_insert_at_cursor (notifyBuff, "Loading done. Use lookup entry to lookup words.", -1);
}

// Create new dictionary
dict_t dictCreate(char* name, char* path) {
    dict_t newDict;

    strcpy(newDict.name, name);
    strcpy(newDict.path, path);

    newDict.dict = btcrt(path, 0, FALSE);

    return newDict;
}

int dictOpen(dict_t * dict) {
    dict->dict = btopn(dict->path, 0, FALSE);

    if(dict->dict != NULL) return 0;
    else {
        printf("Cannot open BTA file: %s\n", dict->path);
        return 1;
    } 
}

int dictClose(dict_t *dict){
    btcls(dict->dict);
    return 1;
}


// To determin if str is prefix of word
// return 1: is prefix; 0: not prefix; 2: str=word
int suggestWordCmp(char * str, char * word) {
    // Handle exception
    if (str == NULL || word == NULL) return 0;

    int i;
    if (strcmp(str, word) == 0) return 2;
    if (strlen(str) > strlen(word)) return 0;

    for (i = 0; i < strlen(str); i++) {
        if (str[i] != word[i]) {
            return 0;
        }
    }

    return 1;
}

int dictAddMeaning(BTA * dict, char * word, char * new_meaning, GtkTextBuffer  *notifyBuff) {

    int meaningLength;
    char * meaning;

    int return_value;

    meaning = malloc(MEAN_MAX_LEN * sizeof(char));

    if (meaning == NULL) {
        printf("Cannot allocate memory!\n");
        exit(1);
    }

    btsel(dict, word, meaning, MEAN_MAX_LEN, &meaningLength);

    meaningLength += strlen(new_meaning); // meaning length = old meaning length + new meaning length
    
    if (strlen(meaning) + strlen(new_meaning) > MEAN_MAX_LEN) {
        gtk_text_buffer_insert_at_cursor (notifyBuff, "Cannot load word - Exceed maximum meaning length:  ", -1);
        gtk_text_buffer_insert_at_cursor (notifyBuff, word, strlen(word));
        gtk_text_buffer_insert_at_cursor (notifyBuff, "\n", -1);
        return -1;
    }

    strcat(meaning, new_meaning);
    return_value = btins(dict, word, meaning, meaningLength*sizeof(char));
    free(meaning);

    return return_value;
}

int dictAddWord(BTA * dict, char * word, char * meaning, GtkTextBuffer  *notifyBuff) {
    char wordLower[WORD_MAX_LEN];


    // standarlize word to use as key
    strLower(wordLower, word);
    trim(wordLower);


    int find;
    int return_value;
    BTint value;

    // do not add word if word string is empty
    if (strlen(wordLower) <= 0) {
        return 1;
    }

    // if word is already available in database, concat new meaning to the end of old meaning
    find = bfndky(dict, wordLower, &value);

    if (find == 0) {
        return_value = dictAddMeaning(dict, wordLower, meaning, notifyBuff);
    } else {
        return_value = btins(dict, wordLower, meaning, strlen(meaning)*sizeof(char));
    }

    return return_value;
}

// delete word from dictionary
void dictDelWord(BTA * dict, char * word, GtkTextBuffer  *notifyBuff) {
    // standarlize word to use as key
    strLower(word, word);
    trim(word);

    btdel(dict, word);

    gtk_text_buffer_set_text (notifyBuff, "Deleted a word: ", -1);
    gtk_text_buffer_insert_at_cursor (notifyBuff, word, -1);
    gtk_text_buffer_insert_at_cursor (notifyBuff, "\0", -1);

}


int dictFindWord(dict_t dict, const char * word, char * meaning) {
    int return_value;
    int meaningLength;
    char * wordLower;


    wordLower = malloc(WORD_MAX_LEN * sizeof(char));
    if (wordLower == NULL) {
        printf("Cannot allocate memory!\n");
        exit(1);
    }

    strLower(wordLower, word);
    return_value =  btsel(dict.dict, wordLower, meaning, MEAN_MAX_LEN, &meaningLength);
    free(wordLower);

    return return_value;
}

int dictFindNextWord(dict_t dict, char * currentWord, char * meaning) {
    int ret;
    int meaningLength;

    BTint value;

    bfndky(dict.dict, currentWord, &value);

    ret =  btseln(dict.dict, currentWord, meaning, MEAN_MAX_LEN, &meaningLength);

    return ret;
}

int dictFindPrevWord(dict_t dict, char * currentWord, char * meaning) {
    int ret;
    int meaningLength;

    BTint value;

    bfndky(dict.dict, currentWord, &value);

    ret =  btselp(dict.dict, currentWord, meaning, MEAN_MAX_LEN, &meaningLength);

    return ret;
}

