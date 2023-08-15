/**
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