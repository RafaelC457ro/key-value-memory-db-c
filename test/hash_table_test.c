#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "hash_table.h"

static void should_init_hash_table(void **state)
{
    HashTable *h = hashtable_init();
    assert_non_null(h);
    assert_int_equal(h->capacity, HASH_TABLE_INIT_SIZE);
    assert_int_equal(h->count, 0);
    hashtable_destroy(h);
}

static void should_insert_hash_table(void **state)
{
    HashTable *h = hashtable_init();
    hashtable_insert(h, "key", "value");
    assert_int_equal(h->count, 1);
    hashtable_destroy(h);
}

static void should_get_hash_table(void **state)
{
    HashTable *h = hashtable_init();
    hashtable_insert(h, "key", "value");
    char *value = (char *)hashtable_get(h, "key");
    assert_string_equal(value, "value");
    hashtable_destroy(h);
}

static void should_update_value(void **state)
{
    HashTable *h = hashtable_init();
    hashtable_insert(h, "key", "value");
    hashtable_insert(h, "key", "value2");
    char *value = (char *)hashtable_get(h, "key");
    assert_string_equal(value, "value2");
    hashtable_destroy(h);
}

static void shoud_delete_a_value(void **state)
{
    HashTable *h = hashtable_init();
    hashtable_insert(h, "key", "value");
    char *value = (char *)hashtable_get(h, "key");
    assert_string_equal(value, "value");

    hashtable_delete(h, "key");
    value = (char *)hashtable_get(h, "key");
    assert_null(value);
}

static void should_insert_and_get_all_values(void **state)
{
    HashTable *h = hashtable_init();
    for (int i = 0; i < 100; i++)
    {
        char *key = malloc(sizeof(char) * 10);
        char *value = malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        hashtable_insert(h, key, value);
        free(key);
        free(value);
    }
    for (int i = 0; i < 100; i++)
    {
        char *key = malloc(sizeof(char) * 10);
        char *value = malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        char *result = (char *)hashtable_get(h, key);
        assert_string_equal(result, value);
        free(key);
        free(value);
    }
    hashtable_destroy(h);
}

static void should_expand_the_hash_table(void **state)
{
    HashTable *h = hashtable_init();
    for (int i = 0; i < 100; i++)
    {
        char *key = malloc(sizeof(char) * 10);
        char *value = malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        hashtable_insert(h, key, "value");
    }
    assert_int_equal(h->count, 100);
    assert_int_equal(h->capacity, 256);
    hashtable_destroy(h);
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