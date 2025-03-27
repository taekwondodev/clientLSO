#include "../h/socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int open_socket() {
	int sock;

	if((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Errore creazione socket");
		exit(EXIT_FAILURE);
	}

	return sock;
}

void setup_server_address(struct sockaddr_un *server_addr) {
    memset(server_addr, 0, sizeof(struct sockaddr_un));          //inizializza a 0
    server_addr->sun_family = AF_UNIX;
    strncpy(server_addr->sun_path, SERVER_PATH, sizeof(server_addr->sun_path) -1);
}

void connection_to_server(int client_socket, struct sockaddr_un *server_addr) {
	if(connect(client_socket, (struct sockaddr*) server_addr, sizeof(struct sockaddr_un)) < 0) {
		perror("Errore connessione al server");
		exit(EXIT_FAILURE);
	}

	printf("------- Benvenuto nella videoteca! -------\n");
}
