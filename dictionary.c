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
    wchar_t * line;
    wchar_t * linePtr;

    char * word;
    char * meaning;

    tmp_word = malloc(WORD_WC_MAX_LEN * sizeof(wchar_t));
    tmp_meaning = malloc(MEAN_WC_MAX_LEN * sizeof(wchar_t));

    word = malloc(WORD_MAX_LEN * sizeof(char));
    meaning = malloc(MEAN_MAX_LEN * sizeof(char));


    //printf("%p - %p - %p - %p\n", tmp_word, tmp_meaning, word, meaning); getch();

    line = malloc(LINE_WC_MAX_LEN * sizeof(wchar_t));

    int i;
    FILE *f;
    if((f=fopen(filename,"r"))==NULL){
    		strcpy(notify, "File open failed");
            return;
    } else {

            printf("Loading...\n");

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
                        printf("Meaning exceeded the maximum length. Word: %S\n", tmp_word);
                        exit(1);
                    }
                    wcscat(tmp_meaning, line);
                    // Read a line in dict file
                    linePtr = fgetws(line, LINE_WC_MAX_LEN, f);
                }


                // Convert from widechar strings to char strings
                wcstombs(word, tmp_word, WORD_MAX_LEN*sizeof(char));
                wcstombs(meaning, tmp_meaning, MEAN_MAX_LEN*sizeof(char));


                dictAddWord(dict, word, meaning);

                // print realtime info
                WordCount++;
                printf("\r> %d words. %s\n", WordCount, word);
                
            }

    }
    fclose(f);

    sprintf(notify, "Loading done. %d words was loaded.", WordCount);

    //printf("%p - %p - %p - %p\n", tmp_word, tmp_meaning, word, meaning); getch();

    //free used memory
    free(word); free(meaning);
    free(tmp_word); free(tmp_meaning);

    // Save the dictionary by re-opening
    printf("\nSaving file...\n");
    btcls(dict);
    dict = btopn("BTree_dict.dat", 0, FALSE);


}

char *rand_string(char *str, size_t size) {
    size_t n;
    int key;
    const char charset[] = "abcdefghijkwxyzABC342390DEFGHIJK";
    if (size) {
        --size;
        for (n = 0; n < size; n++) {
            key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

void createDictionaryRandom(BTA *dict) {

    char * word;

    printf("\nCreating Dictionary..."); fflush(stdout);

    word = malloc(WORD_MAX_LEN * sizeof(char));
    

    btcls(dict); // close if a dictionary file was opened. 
    // below line is required for operating correctly.

    dict = btcrt("BTree_dict.dat", 0, FALSE);

    int i;
    for (i = 1; i < 200000; i++) {
        rand_string(word, 10);
        printf("%d\n", i);
        dictAddWord(dict, word, "skdansdjasndj");
    }

   
    // Save the dictionary by re-opening
    printf("\nSaving file...\n");
    btcls(dict);
    dict = btopn("BTree_dict.dat", 0, FALSE);

    printf("Done!\n");
    getch();

}


int dictFindWord(BTA * dict, char * word, char * meaning) {
    int return_value;
    int meaningLength;
    char * wordLower;


    wordLower = malloc(WORD_MAX_LEN * sizeof(wordLower[0]));
    if (wordLower == NULL) {
        printf("Cannot allocate memory!\n");
        exit(1);
    }

    strLower(wordLower, word);
    return_value =  btsel(dict, wordLower, meaning, MEAN_MAX_LEN, &meaningLength);
    free(wordLower);

    return return_value;
}

int dictAddMeaning(BTA * dict, char * word, char * new_meaning) {

    int meaningLength;
    char * meaning;

    int return_value;

    meaning = malloc(MEAN_MAX_LEN * sizeof(char));

    if (meaning == NULL) {
        printf("Cannot allocate memory!\n");
        exit(1);
    }

    btsel(dict, word, meaning, MEAN_MAX_LEN, &meaningLength);


    if (strlen(meaning) + strlen(new_meaning) > MEAN_MAX_LEN) {
        printf("Cannot load word '%s' - Exceed maximum meaning length.\n", word);
        return -1;
    }

    strcat(meaning, new_meaning);
    return_value = btins(dict, word, meaning, MEAN_MAX_LEN*sizeof(char));
    free(meaning);

    return return_value;
}

int dictAddWord(BTA * dict, char * word, char * meaning) {

    // standarlize word to use as key
    strLower(word, word);
    trim(word);


    int find;
    int return_value;
    BTint value;

    // do not add word if word string is empty
    if (strlen(word) <= 0) {
        return 1;
    }

    // if word is already available in database, concat new meaning to the end of old meaning
    find = bfndky(dict, word, &value);

    if (find == 0) {
        return_value = dictAddMeaning(dict, word, meaning);
    } else {
        return_value = btins(dict, word, meaning, MEAN_MAX_LEN*sizeof(char));
    }

    return return_value;
}