#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int open_socket();

void setup_server_address(struct sockaddr_in *server_addr);

void connection_to_server(int client_socket, struct sockaddr_in *server_addr);
