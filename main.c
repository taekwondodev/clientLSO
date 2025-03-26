#include "client.h"
#include "socket.h"

int main() {
	int choice;
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	int sock = open_socket();
	connection_to_server(sock, &server_addr);

	printf("Connesso al server\n");



	printf("*****Benvenuto nella Videoteca!*****\n");

	return 0;
}
