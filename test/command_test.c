
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "command.h"

static void should_parse_get_command(void **state)
{
    Command *c;
    size_t c_size = 7;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "GET key", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, VALID_COMMAND);
    assert_int_equal(c->type, GET);
    assert_string_equal(c->key, "key");
    assert_null(c->value);
    // I removed the free() call here because it was causing a segfault
    // it's seems like the memory is being freed somewhere else by cmocka
    // https://stackoverflow.com/questions/50001508/allocated-memory-in-multiple-cmocka-unit-tests
}

static void should_parse_set_command(void **state)
{
    Command *c;
    size_t c_size = 50;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET foo bar", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, VALID_COMMAND);
    assert_int_equal(c->type, SET);
    assert_string_equal(c->key, "foo");
    assert_string_equal(c->value, "bar");
}

static void should_parse_set_command_long_string(void **state)
{
    Command *c;
    size_t c_size = 100;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET foo \"bar bar bar\"", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, VALID_COMMAND);
    assert_int_equal(c->type, SET);
    assert_string_equal(c->key, "foo");
    assert_string_equal(c->value, "bar bar bar");
}

static void should_parse_invalid_set_command_long_string_with_double_quotes(void **state)
{
    Command *c;
    size_t c_size = 100;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET foo \"bar bar bar", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, INVALID_QUOTE_FORMAT);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

// set invalid number of arguments more then 3
static void should_parse_invalid_set_command_more_then_three(void **state)
{
    Command *c;
    size_t c_size = 3;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET foo bar fuzz", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, NUM_ARGUMENTS_INVALID);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_set_command_long_string_with_double_quotes_wrong_position_middle(void **state)
{
    Command *c;
    size_t c_size = 100;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET foo bar \"bar bar", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, INVALID_COMMAND);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_get_command(void **state)
{
    Command *c;
    size_t c_size = 7;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "GET", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, NUM_ARGUMENTS_INVALID);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_get_command_extra_args(void **state)
{
    Command *c;
    size_t c_size = 15;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "GET key extra", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, NUM_ARGUMENTS_INVALID);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_set_command(void **state)
{
    Command *c;
    size_t c_size = 7;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, NUM_ARGUMENTS_INVALID);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_set_command_no_value(void **state)
{
    Command *c;
    size_t c_size = 7;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "SET key", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, NUM_ARGUMENTS_INVALID);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_command(void **state)
{
    Command *c;
    size_t c_size = 7;
    char *command = malloc(sizeof(char) * c_size);

    strncpy(command, "aaaaa", c_size);

    int ret = parse_command(command, c);
    assert_int_equal(ret, INVALID_COMMAND);
    assert_int_equal(c->type, INVALID);
    assert_null(c->key);
    assert_null(c->value);
}

static void should_parse_invalid_null_command(void **state)
{
    int ret = parse_command(NULL, NULL);
    assert_int_equal(ret, EMPTY_COMMAND);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(should_parse_get_command),
        cmocka_unit_test(should_parse_set_command),
        cmocka_unit_test(should_parse_set_command_long_string),
        cmocka_unit_test(should_parse_invalid_set_command_long_string_with_double_quotes),
        cmocka_unit_test(should_parse_invalid_get_command),
        cmocka_unit_test(should_parse_invalid_get_command_extra_args),
        cmocka_unit_test(should_parse_invalid_set_command),
        cmocka_unit_test(should_parse_invalid_set_command_no_value),
        cmocka_unit_test(should_parse_invalid_command),
        cmocka_unit_test(should_parse_invalid_null_command),
        cmocka_unit_test(should_parse_invalid_set_command_more_then_three),
        cmocka_unit_test(should_parse_invalid_set_command_long_string_with_double_quotes_wrong_position_middle),

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}