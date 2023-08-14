#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#include "hash_table.h"
#include "queue.h"
#include "command.h"
#include "server.h"

#define PORT 8080
#define SERVER_BACKLOG 1024
#define BUFFER_SIZE 1024
#define MAX_THREADS 8

HashTable *hash_table = NULL;

pthread_mutex_t hash_table_lock = PTHREAD_MUTEX_INITIALIZER;

void init_structures(void);

void client_handler(int client_socket);

void comand_handler(char *input, char *response);

int main(int argc, char **argv)
{
    init_structures();
    init_server(PORT, BUFFER_SIZE, MAX_THREADS, SERVER_BACKLOG, client_handler);

    exit(0);
}

void init_structures(void)
{
    hash_table = hashtable_init();
}

void comand_handler(char *input, char *response)
{
    Command *command = malloc(sizeof(Command)); //
    int status;

    input[strlen(input) - 1] = '\0';

    status = parse_command(input, command);

    switch (status)
    {
    case VALID_COMMAND:

        if (command->type == GET)
        {
            pthread_mutex_lock(&hash_table_lock);
            char *value = (char *)hashtable_get(hash_table, command->key);
            pthread_mutex_unlock(&hash_table_lock);
            if (value == NULL)
            {

                sprintf(response, "(nil)\n");
            }
            else
            {
                sprintf(response, "\"%s\"\n", value);
            }
        }
        else if (command->type == SET)
        {
            // lock
            pthread_mutex_lock(&hash_table_lock);
            hashtable_insert(hash_table, command->key, command->value);
            pthread_mutex_unlock(&hash_table_lock);

            sprintf(response, "OK\n");
        }
        else if (command->type == DELETE)
        {

            pthread_mutex_lock(&hash_table_lock);
            hashtable_delete(hash_table, command->key);
            pthread_mutex_unlock(&hash_table_lock);

            sprintf(response, "OK\n");
        }
        break;
    case INVALID_COMMAND:
        sprintf(response, "Invalid command\n");
        break;

    case EMPTY_COMMAND:
        sprintf(response, "Empty command\n");
        break;

    case NUM_ARGUMENTS_INVALID:
        sprintf(response, "Invalid number of arguments\n");
        break;

    case INVALID_QUOTE_FORMAT:
        sprintf(response, "Invalid quote format\n");
        break;

    default:
        break;
        sprintf(response, "Invalid command\n");
    }

    free(command);
}

void client_handler(int client_socket)
{
    char *response = malloc(BUFFER_SIZE * sizeof(char));
    char *input = malloc(BUFFER_SIZE * sizeof(char));

    while (1)
    {
        int status;
        Command *command;
        char buffer[BUFFER_SIZE] = {0};

        int valread = read_client(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            break; // disconnected
        }

        printf("Received: %s", buffer);

        strcpy(input, buffer);

        comand_handler(input, response);

        send_client(client_socket, response, strlen(response));
        memset(buffer, 0, BUFFER_SIZE);
        memset(response, 0, BUFFER_SIZE);
    }

    printf("Client disconnected.\n");
    // Close the client socket
    close_client(client_socket);
    free(response);
    free(input);
}
