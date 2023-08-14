/*
 * This file contains the implementation of the hash table data structure. The
 * intention of this implementation is to be very simple and easy to understand.
 * It was highly inspired by https:*benhoyt.com/writings/hash-table-in-c/.
 * This implementation uses the FNV-1a hash function and separate chaining, and
 * to dial with collisions it was implemented a dynamic array of linked lists.
 * The hash table is initialized with a capacity of 16 entries, and when the
 * number of entries reaches 75% of the capacity, the hash table is expanded
 * to double its capacity.
 * The hash table is a generic data structure, so it can store any type of data.
 * To store a value in the hash table, the value must be passed as a void pointer.
 * To retrieve a value from the hash table, the value must be cast to the correct
 * type.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https:*en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_string(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

static Entry *create_entry(char *key, void *value, Entry *next)
{
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

static u_int64_t get_index(HashTable *table, char *key)
{
    uint64_t hash = hash_string(key);
    uint64_t index = hash % table->capacity;
    return index;
}

static void set_entry(HashTable *table, char *key, void *value)
{
    uint64_t index = get_index(table, key);

    Entry *entry = table->entries[index];

    if (entry == NULL)
    {
        entry = create_entry(key, value, NULL);
        table->entries[index] = entry;

        return;
    }
    else
    {
        Entry *prev = NULL;
        while (entry != NULL)
        {
            if (strcmp(entry->key, key) == 0)
            {
                free(entry->value);
                entry->value = value;
                return;
            }
            prev = entry;
            entry = (Entry *)entry->next;
        }
        entry = create_entry(key, value, NULL);
        prev->next = entry;
    }
}

static void expand_hashtable(HashTable *table)
{
    int prevCapacity = table->capacity;
    Entry **prevEntries = table->entries;

    table->capacity = table->capacity << 1;
    table->entries = (Entry **)calloc(table->capacity, sizeof(Entry *));

    for (int i = 0; i < prevCapacity; i++)
    {
        Entry *entry = prevEntries[i];
        while (entry != NULL)
        {
            set_entry(table, entry->key, entry->value);
            entry = entry->next;
        }
    }

    free(prevEntries);
}

HashTable *hashtable_init()
{
    HashTable *table = malloc(sizeof(HashTable));
    table->count = 0;
    table->capacity = HASH_TABLE_INIT_SIZE;
    table->entries = (Entry **)calloc(table->capacity, sizeof(Entry *));
    return table;
}

void hashtable_destroy(HashTable *table)
{
    for (int i = 0; i < table->capacity; i++)
    {
        Entry *entry = (Entry *)table->entries[i];
        while (entry != NULL)
        {

            free(entry->key);
            free(entry->value);
            Entry *next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(table->entries);
    free(table);
}

void hashtable_insert(HashTable *table, char *key, void *value)
{
    if (key == NULL || value == NULL)
        return;

    if (table->count >= table->capacity * HASH_TABLE_CAPACITY_TRASHOLD)
        expand_hashtable(table);

    set_entry(table, strdup(key), strdup(value));
    table->count++;
}

void *hashtable_get(HashTable *table, char *key)
{
    if (key == NULL)
        return NULL;
    uint64_t index = get_index(table, key);

    Entry *entry = (Entry *)table->entries[index];

    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
        entry = (Entry *)entry->next;
    }

    return NULL;
}

void hashtable_delete(HashTable *table, char *key)
{
    if (key == NULL)
        return;

    uint64_t index = get_index(table, key);

    Entry *entry = (Entry *)table->entries[index];
    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            free(entry->key);
            free(entry->value);
            Entry *next = entry->next;
            free(entry);
            table->entries[index] = next;
            table->count--;
            return;
        }
        entry = (Entry *)entry->next;
    }
}
