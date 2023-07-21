#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#define HASH_TABLE_INIT_SIZE 16

#define HASH_TABLE_CAPACITY_TRASHOLD 0.75

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct h_entry
{
    char *key;
    char *value;
    void *next;
} Entry;

typedef struct h_table
{
    int count;
    int capacity;
    Entry **entries;
} HashTable;

HashTable *hashtableInit();

void hashTableInsert(HashTable *table, char *key, void *value);

void *hashTableGet(HashTable *table, char *key);

void hashTableDelete(HashTable *table, char *key);

void hashTableDestroy(HashTable *table);

#endif