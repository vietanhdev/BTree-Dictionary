#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "extio.h"
#include "dictionary.h"
#include "UI.h"

void createDictionary(BTA *dict, char * notify) {

    printf("\nCreating Dictionary...");

    int WordCount = 0;

    dict = btcrt("BTree_dict.dat", 0, 0);

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
                
                dictInsertWord(dict, word, meaning);
                
                WordCount++;
            }

    }
    fclose(f);
    getch();
    sprintf(notify, "Loading done. %d words was loaded.", WordCount);

}


int dictFindWord(BTA *dict, char * word, char * meaning) {
    int meaningLength;
    return btsel(dict, word, meaning, MEAN_MAX_LEN, &meaningLength);
}

int dictInsertWord(BTA *dict, char *word, char * meaning) {
    return btins(dict, word, meaning, MEAN_MAX_LEN*sizeof(char));
}