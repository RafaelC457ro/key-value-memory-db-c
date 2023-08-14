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