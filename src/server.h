#ifndef __MEMDB_SERVER_H__
#define __MEMDB_SERVER_H__

void init_server(int port, int buffersize, int maxthread, int maxbacklog, void (*client_handler)(int client_socket));

int read_client(int client_socket, char *buffer, int buffersize);

int send_client(int client_socket, char *buffer, int buffersize);

int close_client(int client_socket);

static void *thread_pool_worker(void *args);
static void init_pool_theads(int max_threads);

#endif
