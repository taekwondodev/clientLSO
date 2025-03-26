#include "socket.h"
#include <stdio.h>
#include <stdlib.h>

int open_socket() {
	int sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Errore creazione socket");
		exit(EXIT_FAILURE);
	}

	return sock;
}

void connection_to_server(int client_socket, struct sockaddr_in *server_addr) {
	if(connect(client_socket, (struct sockaddr *) server_addr, sizeof(*server_addr)) < 0) {
		perror("Errore connessione al server");
		exit(EXIT_FAILURE);
	}
}
