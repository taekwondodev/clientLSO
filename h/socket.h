#include <sys/un.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>

#define PORT 8080
#define CHUNK_SIZE 1024
#define TERMINATOR "##END##"

int open_socket();

void setup_server_address(struct sockaddr_in *server_addr);

void connection_to_server(int client_socket, struct sockaddr_in *server_addr);

bool send_data(int client_socket, const void *data, size_t data_size);

char* receive_long_data(int client_socket);
