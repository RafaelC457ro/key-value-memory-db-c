#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "queue.h"
#include "server.h"

Queue *queue = NULL;

pthread_mutex_t client_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_queue_cond = PTHREAD_COND_INITIALIZER;

void init_server(int port, int buffersize, int maxthread, int maxbacklog, void (*client_handler)(int client_socket))
{
    int server_fd, client_socket;
    pthread_t thread_pool[maxthread];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    queue = queue_init();

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    for (int i = 0; i < maxthread; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_pool_worker, (void *)client_handler);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, maxbacklog) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1)
    {

        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&client_queue_lock);
        queue_enqueue(queue, client_socket);
        pthread_cond_signal(&client_queue_cond);
        pthread_mutex_unlock(&client_queue_lock);
    }

    close(server_fd);
}

int read_client(int client_socket, char *buffer, int buffersize)
{
    int valread;
    valread = read(client_socket, buffer, buffersize);
    return valread;
}

int send_client(int client_socket, char *buffer, int buffersize)
{
    int valread;
    valread = send(client_socket, buffer, buffersize, 0);
    return valread;
}

int close_client(int client_socket)
{
    return close(client_socket);
}

static void *thread_pool_worker(void *args)
{
    // callback function
    void (*client_handler)(int client_socket) = (void (*)(int client_socket))args;

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
        (*client_handler)(client_socket);
    }

    return NULL;
}
