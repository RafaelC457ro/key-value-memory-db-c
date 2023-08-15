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

#ifndef __MENDB_COMMAND_H__
#define __MENDB_COMMAND_H__

enum command_type
{
    GET,
    SET,
    DELETE,
    INVALID
};

enum invalid_command_reason
{
    VALID_COMMAND,
    INVALID_COMMAND,
    EMPTY_COMMAND,
    NUM_ARGUMENTS_INVALID,
    INVALID_QUOTE_FORMAT,
};

typedef struct command
{
    enum command_type type;
    char *key;
    char *value;
} Command;

#define COMMAND_DELIMITER " "
#define COMMAND_QUOTE_DELIMITER "\""

#define MAX_COMMAND_ARGUMENTS 3

#define COMMAND_GET "get"
#define COMMAND_SET "set"
#define COMMAND_DELETE "delete"

static void s_to_lower_case(char *str);
static int validate_double_quote(char *str);
static void set_command_invalid(Command *cmd);

int parse_command(char *input, Command *cmd);

void destroy_command(Command *command);

#endif