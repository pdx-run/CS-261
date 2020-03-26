/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Roger Hill 
 * Date: 3/5/2020

 Citations:  http://crasseux.com/books/ctutorial/Macro-functions.html
 http://enthusiaststudent.blogspot.com/2017/03/hashing-with-separate-chaining-c-program.html
 https://codereview.stackexchange.com/questions/194807/hashtable-implementation-c
 https://github.com/kniffina/CS-261-Data-Structures/blob/master/Hashtable/hashMap.c
 https://eklitzke.org/an-introduction-to-valgrind-memcheck
 */

#define _CRT_SECURE_NO_WARNINGS  // suppresses scanf Visual Studio compile error

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key) {  // pointer to key character array
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        r += key[i];
    }
    return r;    // cumulative ascii sum of the key characters returned
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next) {
    
 // memory for the pointer to string
    HashLink* link = malloc(sizeof(HashLink));

 // memory for the character string
    link->key = malloc(sizeof(char) * (strlen(key) + 1));  
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key); 
    free(link);      
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity) {

    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);

    for (int i = 0; i < capacity; i++) {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map) {

    // FIXME: implement

    assert(map != 0);
    
    struct HashLink* current;
    struct HashLink* temp;

    for (int i = 0; i < hashMapCapacity(map); i++) {
        current = map->table[i];
        while (current != NULL) {

            temp = current;
            current = current->next;
            hashLinkDelete(temp);
            
        }
    }
    free(map->table);
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity) {
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key) {

    // FIXME: implement

    assert(map != 0);
    struct HashLink* current;
 
// calculate table index(bucket)
    int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
    if (idx < 0) {
        idx += hashMapCapacity(map);
    }

    current = map->table[idx];   // select chain index
    while (current != NULL) {
        if (strcmp(current->key,key) == 0) { 

            return &(current->value);  // return pointer to the link->value
        }
        current = current->next;
    }
    return NULL; // key not found, return NULL
}

/**
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, 
 * all of the links need to rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity) {

    // FIXME: implement

    struct HashMap *newMap = hashMapNew(hashMapCapacity(map) * 2);
    struct HashLink* current;

    for (int i = 0; i < hashMapCapacity(map); i++) {
        current = map->table[i];
        while (current != NULL) {
            hashMapPut(newMap, current->key, current->value);
            current = current->next;
        }
    }
    map = newMap;
    hashMapDelete(newMap);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value) {
    
    // FIXME: implement

    assert(map != 0);

// calculate bucket index
    int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
    if (idx < 0) {
        idx += hashMapCapacity(map);
    }

    if (hashMapContainsKey(map, key)) {   // if key exists
        
        *hashMapGet(map, key) = value;    // update dereferenced memory location with value
    }
    else {

        struct HashLink* newLink = hashLinkNew(key, value, map->table[idx]);
        map->table[idx] = newLink;
        map->size++;

    }
    if (hashMapTableLoad(map) > MAX_TABLE_LOAD) {
        resizeTable(map, (hashMapCapacity(map) * 2));
    }
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 Citation:  Worksheet 38
 */
void hashMapRemove(HashMap* map, const char* key) {
    // FIXME: implement

    assert(map != 0);

    int idx = HASH_FUNCTION(key) % hashMapCapacity(map);
    if (idx < 0) {
        idx += hashMapCapacity(map);
    }
    
    if (hashMapContainsKey(map, key)) {

        struct HashLink* current = map->table[idx];
        struct HashLink* prev = NULL;

        while (current != NULL) {

            if (strcmp(current->key, key) == 0) { 

                if (prev == NULL) { // key found in 1st position
                    map->table[idx] = current->next;
                }
                else {                           
                    prev->next = current->next;
                }
                hashLinkDelete(current);
                map->size--;
                return;
            }
            prev = current;
            current = current->next;
        }
    }
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key) { // DONE!

    // FIXME: implement
 
    assert(map != 0);

    if (hashMapGet(map, key) != NULL) {
        return 1; // found key
    }
    else {

        return 0; // key not found
    }
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map) { // DONE!
   
    // FIXME: implement
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map) { // DONE!

    // FIXME: implement
    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map) { // DONE!

    // FIXME: implement
    
    assert(map != 0);
    int emptyLinks = 0;
    for (int i = 0; i < hashMapCapacity(map); i++) {
        
        if (map->table[i] == NULL) {
            emptyLinks++;
        }
    }
    return emptyLinks;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map) { // DONE!

    // FIXME: implement
    
    assert(map != 0);
    double loadFactor = 0.0;
    loadFactor = hashMapSize(map) / (double)hashMapCapacity(map);
    return loadFactor;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map) { // DONE!
  // FIXME: implement

    struct HashLink* temp;

    for (int i = 0; i < hashMapCapacity(map); i++) {
        temp = map->table[i];
        while (temp != 0) {
            printf("  \nKey:%s Value:%d ", temp->key, temp->value);
            temp = temp->next;
        }
    }
}
