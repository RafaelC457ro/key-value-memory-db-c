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
#include <ctype.h>
#include <string.h>

#include "command.h"

int parse_command(char *input, Command *cmd)
{
    int argc = 0;
    char *argv[100];
    char *input_copy;
    char *saveptr;
    char *token;

    if (input == NULL || cmd == NULL)
    {
        return EMPTY_COMMAND;
    }

    input_copy = strdup(input); // not sure if is needed to many memory allocation

    while ((token = strtok_r(input_copy, COMMAND_DELIMITER, &saveptr)) && argc < 100)
    {
        // next saveptr[0] is iqual to "
        if (argc == 1 && saveptr != NULL && saveptr[0] == '"')
        {
            if (validate_double_quote(saveptr) != 0)
            {
                set_command_invalid(cmd);
                return INVALID_QUOTE_FORMAT;
            }
            char *temp;

            temp = strtok_r(NULL, COMMAND_QUOTE_DELIMITER, &saveptr);
            if (temp == NULL)
            {
                set_command_invalid(cmd);
                return INVALID_QUOTE_FORMAT;
            }

            argv[argc] = strdup(token);    // bad
            argv[argc + 1] = strdup(temp); // bad
            // skip saveptr to size of argv[argc - 1] + 1
            saveptr = &saveptr[strlen(argv[argc - 1]) + 1];
            argc += 2;
            input_copy = NULL;
            continue;
        }

        argv[argc++] = strdup(token); // bad
        input_copy = NULL;
    }

    if (argc == 0 || argc > MAX_COMMAND_ARGUMENTS)
    {
        set_command_invalid(cmd);
        return INVALID_COMMAND;
    }

    s_to_lower_case(argv[0]);

    if (strcmp(argv[0], COMMAND_GET) == 0)
    {
        if (argc != 2)
        {
            set_command_invalid(cmd);
            return NUM_ARGUMENTS_INVALID;
        }

        cmd->type = GET;
        cmd->key = strdup(argv[1]);
        cmd->value = NULL;

        return VALID_COMMAND;
    }
    else if (strcmp(argv[0], COMMAND_SET) == 0)
    {
        if (argc != 3)
        {
            set_command_invalid(cmd);
            return NUM_ARGUMENTS_INVALID;
        }

        cmd->type = SET;
        cmd->key = argv[1];
        cmd->value = argv[2];

        return VALID_COMMAND;
    }
    else if (strcmp(argv[0], COMMAND_DELETE) == 0)
    {
        if (argc != 2)
        {
            set_command_invalid(cmd);
            return NUM_ARGUMENTS_INVALID;
        }

        cmd->type = DELETE;
        cmd->key = argv[1];
        cmd->value = NULL;

        return VALID_COMMAND;
    }
    else
    {
        set_command_invalid(cmd);
        return INVALID_COMMAND;
    }
}

void destroy_command(Command *cmd)
{
    if (cmd->key != NULL)
    {
        free(cmd->key);
    }

    if (cmd->value != NULL)
    {
        free(cmd->value);
    }
}

static void s_to_lower_case(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

static int validate_double_quote(char *str)
{
    int len = strlen(str);
    if (str[len - 1] != '"')
    {
        return -1;
    }
    return 0;
}

static void set_command_invalid(Command *cmd)
{
    cmd->type = INVALID;
    cmd->key = NULL;
    cmd->value = NULL;
}
