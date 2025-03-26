#include <arpa/inet.h>
#include <unistd.h>

int open_socket();

void connection_to_server(int client_socket, struct sockaddr_in *server_addr);
