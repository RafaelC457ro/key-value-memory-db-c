/**
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
 *
 * MIT License
 *
 * Copyright (c) 2017 Rafael Castro
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

HashTable *hashtable_init();

void hashtable_insert(HashTable *table, char *key, void *value);

void *hashtable_get(HashTable *table, char *key);

void hashtable_delete(HashTable *table, char *key);

void hashtable_destroy(HashTable *table);

#endif