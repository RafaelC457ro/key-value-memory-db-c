#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "hash_table.h"
#include "queue.h"
#include "command.h"

#define PORT 8080
#define SERVER_BACKLOG 1024
#define BUFFER_SIZE 1024
#define MAX_THREADS 8

pthread_t thread_pool[MAX_THREADS];
Queue *queue = NULL;
HashTable *hash_table = NULL;

pthread_mutex_t client_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t hash_table_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_queue_cond = PTHREAD_COND_INITIALIZER;

void init_structures(void);

void client_handler(int client_socket);

void *thread_pool_worker(void *args);

void init_server(void);

void init_pool_theads(int max_threads);

void comand_handler(char *input, char *response);

int main(int argc, char *argv[])
{
    init_structures();
    init_pool_theads(MAX_THREADS);
    init_server();

    exit(0);
}

void init_structures(void)
{
    queue = queue_init();
    hash_table = hashtable_init();
}

void init_server()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(server_fd, SERVER_BACKLOG) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // accept
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // add to queue
        pthread_mutex_lock(&client_queue_lock);
        queue_enqueue(queue, client_socket);
        pthread_cond_signal(&client_queue_cond);
        pthread_mutex_unlock(&client_queue_lock);
    }

    // close
    close(server_fd);
}

void init_pool_theads(int max_threads)
{
    for (int i = 0; i < max_threads; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_pool_worker, NULL);
    }
}

void *thread_pool_worker(void *args)
{
    int client_socket;

    while (1)
    {
        pthread_mutex_lock(&client_queue_lock);
        while (queue_is_empty(queue))
        {
            pthread_cond_wait(&client_queue_cond, &client_queue_lock);
        }
        client_socket = queue_dequeue(queue);
        pthread_mutex_unlock(&client_queue_lock);

        printf("Handling client %d\n", client_socket);
        client_handler(client_socket);
    }

    return NULL;
}

void comand_handler(char *input, char *response)
{
    Command *command = malloc(sizeof(Command)); //
    int status;

    // remove \n
    input[strlen(input) - 1] = '\0';

    // parse command
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

        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            break; // disconnected
        }
        printf("Received: %s", buffer);

        // copy buffer to input
        strcpy(input, buffer);

        // handle command
        comand_handler(input, response);

        // send back OK

        send(client_socket, response, strlen(response), 0);
        memset(buffer, 0, BUFFER_SIZE);
        memset(response, 0, BUFFER_SIZE);
    }

    printf("Client disconnected.\n");

    // Close the client socket
    close(client_socket);
    free(response);
}
