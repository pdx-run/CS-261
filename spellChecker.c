
#define _CRT_SECURE_NO_WARNINGS  // suppresses scanf Visual Studio compile error

#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 * Citations: 
 * https://codereview.stackexchange.com/questions/115843/dictionary-implementation-using-hash-table-in-c
 * https://github.com/liderrick/Spell-Checker-Using-Hash-Table/blob/master/spellChecker.c
 * https://stackoverflow.com/questions/26685936/valgrind-invalid-write-of-size-1-for-strcpy/26685999
 */
void loadDictionary(FILE* file, HashMap* map) {
    // FIXME: implement

    assert(map != 0);
    for(int i = 0; i < hashMapCapacity(map); i++) {

        char* newWord = nextWord(file);
        struct HashLink* newLink = hashLinkNew(newWord, 0, map->table[i]);
        map->table[i] = newLink;
        free(newWord);  
        map->size++;
    }
}

/**
 * Checks the distance between the user inputted string, and each of the dictionary
 * words based on the levenShtein algorithm of counting the number of steps necessary
 * to complete the equality.
 * @param input
 * @param dict
 * @return levenShtein distance between the input, and dictionary words
 * Citations: https://en.wikipedia.org/wiki/Levenshtein_distance
 * http://rosettacode.org/wiki/Levenshtein_distance#C
 */

int levDistance(const char* i, int iLength, const char* d, int dLength) {
    
    int a = 0;
    int b = 0;
    int c = 0;
    
// return length of the non-empty string if either string is empty    
    if (iLength == 0) {
        return dLength;
    }
    if (dLength == 0) {
        return iLength;
    }

// return the number of changes if both strings have same last letters
    if (i[iLength - 1] == d[dLength - 1]) {
        return levDistance(i, iLength - 1, d, dLength - 1);
    }
/* else, test changing last letter of inputBuffer to that of the dictionary word;
*  removing the last letter of inputBuffer, or removing the last letter of the 
*  dictionary word to get the match.*/

    else {

       a = levDistance(i, iLength - 1, d, dLength - 1);
       b = levDistance(i, iLength - 1, d, dLength - 1);
       c = levDistance(i, iLength - 1, d, dLength - 1);

       if (a > b) {
           a = b;
       }
       if (a > c) {
           a = c;
       }
        
       return a + 1;
    }
}

/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 * Citations: https://en.wikipedia.org/wiki/Levenshtein_distance
 * http://rosettacode.org/wiki/Levenshtein_distance#C
 *https://www.geeksforgeeks.org/to-find-smallest-and-second-smallest-element-in-an-array/
 */
int main(int argc, const char** argv) {
    // FIXME: implement
    HashMap* map = hashMapNew(109581);
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    char originalBuffer[256];
    char* dWord = NULL;
    int dWordLength = 0;
    int iBufferLength = 0;
    int found = 0;
    int quit = 0;
    int likeWordsCapacity = 0;

    while (!quit) {

        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        strcpy(originalBuffer, inputBuffer);

        // lower-case the input string
        for (int i = 0; inputBuffer[i]; i++) {
            inputBuffer[i] = tolower(inputBuffer[i]);
        }

        if (strcmp(inputBuffer, "quit") == 0) {  // Exit for 'quit'
            quit = 1;
        }
        else {
            // Populate levenShtein distances, and return if word is found
            for (int i = 0; i < hashMapCapacity(map); i++) {
                dWord = map->table[i]->key;
                dWordLength = strlen(map->table[i]->key);
                iBufferLength = strlen(inputBuffer);
                map->table[i]->value = levDistance(dWord, dWordLength, inputBuffer, iBufferLength);

                if ((map->table[i]->value == 0) && (strcmp(inputBuffer, "quit") != 0)) {
                    printf("\nThe inputted word: \"%s\"", originalBuffer);
                    printf(" is spelled correctly.\n\n");
                    found++;
                    break;
                }
            }
            // Word not found; provide a list of 5 choices
            if (!(found) && !(quit)) {

                printf("\nThe inputted word: \"%s\"", originalBuffer);
                printf(" is spelled incorrectly!\n");

                // allocate likeWords HashMap
                HashMap* likeWords = malloc(sizeof(HashMap));
                likeWordsCapacity = 5;

                likeWords->capacity = likeWordsCapacity;
                likeWords->size = 0;
                likeWords->table = malloc(sizeof(HashLink*) * likeWordsCapacity);

                // Create and initialize likeWords links
                for (int i = 0; i < likeWordsCapacity; i++) {
                    likeWords->table[i] = NULL;

                    HashLink* newLink = malloc(sizeof(HashLink));
                    newLink->key = malloc(sizeof(char) * (256));
                    strcpy(newLink->key, map->table[i]->key);
                    newLink->value = map->table[i]->value;
                    newLink->next = likeWords->table[i];

                    //struct HashLink* newLink = hashLinkNew(map->table[i]->key, map->table[i]->value, likeWords->table[i]);
                    likeWords->table[i] = newLink;
                    likeWords->size++;
                }

                for (int i = 0; i < hashMapCapacity(map); i++) {
                 
                      if ((map->table[i]->value < map->table[0]->value) &&
                           (map->table[i]->value != likeWords->table[1]->value) &&
                           (map->table[i]->value != likeWords->table[2]->value) &&
                           (map->table[i]->value != likeWords->table[3]->value)) {

                            strcpy(likeWords->table[4]->key, likeWords->table[3]->key);
                            likeWords->table[4]->value = likeWords->table[3]->value;

                            strcpy(likeWords->table[3]->key, likeWords->table[2]->key);
                            likeWords->table[3]->value = likeWords->table[2]->value;

                            strcpy(likeWords->table[2]->key, likeWords->table[1]->key);
                            likeWords->table[2]->value = likeWords->table[1]->value;

                            strcpy(likeWords->table[1]->key, likeWords->table[0]->key);
                            likeWords->table[1]->value = likeWords->table[0]->value;

                            strcpy(likeWords->table[0]->key, map->table[i]->key);
                            likeWords->table[0]->value = map->table[i]->value;

                        }
                }


                // like words
                printf("\nDid you mean...?  \[%s, %s, %s, %s, %s\]\n\n", likeWords->table[0]->key, 
                    likeWords->table[1]->key, likeWords->table[2]->key, likeWords->table[3]->key, 
                    likeWords->table[4]->key);

                //Free memory
                for (int i = 0; i < likeWordsCapacity; i++) {
                    free(likeWords->table[i]->key);
                    free(likeWords->table[i]);
                }
                free(likeWords->table);
                free(likeWords);
            }
            found = 0;
        }
    }
        hashMapDelete(map);
        return 0;
}
