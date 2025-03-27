#include <sys/un.h>
#include <sys/socket.h>

#define SERVER_PATH ""
#define BUFFER_SIZE 1024

int open_socket();

void setup_server_address(struct sockaddr_un *server_addr);

void connection_to_server(int client_socket, struct sockaddr_un *server_addr);
