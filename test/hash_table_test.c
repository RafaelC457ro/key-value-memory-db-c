#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "hash_table.h"

static void should_init_hash_table(void **state)
{
    HashTable *h = hashtableInit();
    assert_non_null(h);
    assert_int_equal(h->capacity, HASH_TABLE_INIT_SIZE);
    assert_int_equal(h->count, 0);
    hashTableDestroy(h);
}

static void should_insert_hash_table(void **state)
{
    HashTable *h = hashtableInit();
    hashTableInsert(h, "key", "value");
    assert_int_equal(h->count, 1);
    hashTableDestroy(h);
}

static void should_get_hash_table(void **state)
{
    HashTable *h = hashtableInit();
    hashTableInsert(h, "key", "value");
    char *value = (char *)hashTableGet(h, "key");
    assert_string_equal(value, "value");
    hashTableDestroy(h);
}

static void should_update_value(void **state)
{
    HashTable *h = hashtableInit();
    hashTableInsert(h, "key", "value");
    hashTableInsert(h, "key", "value2");
    char *value = (char *)hashTableGet(h, "key");
    assert_string_equal(value, "value2");
    hashTableDestroy(h);
}

static void shoud_delete_a_value(void **state)
{
    HashTable *h = hashtableInit();
    hashTableInsert(h, "key", "value");
    char *value = (char *)hashTableGet(h, "key");
    assert_string_equal(value, "value");

    hashTableDelete(h, "key");
    value = (char *)hashTableGet(h, "key");
    assert_null(value);
}

static void should_insert_and_get_all_values(void **state)
{
    HashTable *h = hashtableInit();
    for (int i = 0; i < 100; i++)
    {
        char *key = malloc(sizeof(char) * 10);
        char *value = malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        hashTableInsert(h, key, value);
        free(key);
        free(value);
    }
    for (int i = 0; i < 100; i++)
    {
        char *key = malloc(sizeof(char) * 10);
        char *value = malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        char *result = (char *)hashTableGet(h, key);
        assert_string_equal(result, value);
        free(key);
        free(value);
    }
    hashTableDestroy(h);
}

static void should_expand_the_hash_table(void **state)
{
    HashTable *h = hashtableInit();
    for (int i = 0; i < 100; i++)
    {
        char *key = malloc(sizeof(char) * 10);
        char *value = malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        hashTableInsert(h, key, "value");
    }
    assert_int_equal(h->count, 100);
    assert_int_equal(h->capacity, 256);
    hashTableDestroy(h);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(should_init_hash_table),
        cmocka_unit_test(should_insert_hash_table),
        cmocka_unit_test(should_get_hash_table),
        cmocka_unit_test(should_insert_and_get_all_values),
        cmocka_unit_test(should_expand_the_hash_table),
        cmocka_unit_test(should_update_value)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}