/*
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


*/
