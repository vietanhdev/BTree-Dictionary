#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "extio.h"
#include "dictionary.h"
#include "UI.h"
#include "string_ext.h"


void createDictionary(BTA *dict, char * notify) {

    printf("\nCreating Dictionary..."); fflush(stdout);

    int WordCount = 0; // count the number of words in dictionary

    btcls(dict); // close if a dictionary file was opened. 
    // below line is required for operating correctly.

    dict = btcrt("BTree_dict.dat", 0, FALSE);

    char *filename="EV_text_dict.txt";
    wchar_t * tmp_word; // widechar word
    wchar_t * tmp_meaning; // widechar meaning
    wchar_t line[LINE_WC_MAX_LEN];
    wchar_t * linePtr;

    char * word;
    char * meaning;

    tmp_word = malloc(WORD_WC_MAX_LEN * sizeof(tmp_word[0]));
    tmp_meaning = malloc(MEAN_WC_MAX_LEN * sizeof(tmp_meaning[0]));

    word = malloc(WORD_MAX_LEN * sizeof(word[0]));
    meaning = malloc(MEAN_MAX_LEN * sizeof(meaning[0]));

    int i;
    FILE *f;
    if((f=fopen(filename,"r"))==NULL){
    		strcpy(notify, "File open failed");
            return;
    } else {

            // Find the first word
            do {
                // Read a line in dict file
                linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
            } while(linePtr != NULL && line[0] != L'@');

            while(linePtr != NULL) {
                // Empty word and meaning strings
                memset(tmp_word,0,sizeof(tmp_word[0]));
                memset(tmp_meaning,0,sizeof(tmp_meaning[0]));

                // Split word
                for (i = 1; iswalpha(line[i]); i++);
                wcsncpy(tmp_word, &line[1], i-1);
                tmp_word[i-1] = '\0';

                // Split meaning
                wcsncpy(tmp_meaning, &line[i], wcslen(line) - i + 1);

                // Continuously read lines containing meaning
                // Read a line in dict file
                linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
                while (linePtr != NULL && line[0] != L'@') {
                    wcscat(tmp_meaning, line);
                    // Read a line in dict file
                    linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
                }


                // Convert from widechar strings to char strings
                wcstombs(word, tmp_word, WORD_MAX_LEN*sizeof(char));
                wcstombs(meaning, tmp_meaning, MEAN_MAX_LEN*sizeof(char));
                
                //printf("WORD: '%s'\n%s\n", word, meaning);

                dictAddWord(dict, word, meaning);
                
                WordCount++;
            }

    }
    fclose(f);
    sprintf(notify, "Loading done. %d words was loaded.", WordCount);

    // Save the dictionary by re-opening
    printf("\nSaving file...\n");
    btcls(dict);
    dict = btopn("BTree_dict.dat", 0, FALSE);
}


int dictFindWord(BTA * dict, char * word, char * meaning) {
    char wordLower[WORD_MAX_LEN];
    strLower(wordLower, word);
    int meaningLength;
    return btsel(dict, wordLower, meaning, MEAN_MAX_LEN, &meaningLength);
}

int dictAddWord(BTA * dict, char * word, char * meaning) {
    char wordLower[WORD_MAX_LEN];
    char old_meaning[MEAN_MAX_LEN];
    int find;

    int return_value; // return value of this function

    // do not add word if word string is empty
    if (strlen(word) <= 0) {
        printf("Stringlen<=0\n"); getch();
        return 1;
    }

    // if word is already available in database, concat new meaning to the end of old meaning
    find = dictFindWord(dict, word, old_meaning);
    if (find == 0) {
        sprintf(meaning, "%s%s", old_meaning, meaning);
    }

    strLower(wordLower, word);
    
    return_value = btins(dict, wordLower, meaning, MEAN_MAX_LEN*sizeof(char));
    printf("%d\n", return_value); getch();

    return return_value;
}