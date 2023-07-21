#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

void printTable(HashTable *table)
{
    int empty = 0;
    int collisions = 0;
    for (int i = 0; i < table->capacity; i++)
    {
        Entry *entry = (Entry *)table->entries[i];
        if (entry != NULL)
        {
            printf("index: %d key: %s value: %s ", i, entry->key, entry->value);
            Entry *next = (Entry *)entry->next;
            while (next != NULL)
            {
                printf("-> key: %s value: %s ", next->key, next->value);
                next = (Entry *)next->next;
                collisions++;
            }
            printf("\n");
        }
        else
        {
            empty++;
        }
    }

    printf("count: %d\n", table->count);
    printf("capacity: %d\n", table->capacity);
    printf("empty: %d\n", empty);
    printf("collisions: %d\n", collisions);
    // collisions percentage
    double percentage = ((float)collisions / (float)table->count) * 100.0;
    printf("collisions percentage: %f\n", percentage);
}

int main(int argc, char *argv[])
{

    printf("hello world\n");

    exit(0);
}
