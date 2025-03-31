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

void setup_server_address(struct sockaddr_in *server_addr) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr->sin_port = htons(PORT);
}

void connection_to_server(int client_socket, struct sockaddr_in *server_addr) {
	if(connect(client_socket, (struct sockaddr*) server_addr, sizeof(*server_addr)) < 0) {
		perror("Errore connessione al server");
		exit(EXIT_FAILURE);
	}

	printf("------- Benvenuto nella videoteca! -------\n");
}

bool send_data(int client_socket, const void *data, size_t size) {
	if (send(client_socket, data, size, 0) < 0) {
        perror("Errore nell'invio dati");
        return false;
    }
    return true;
}

char* receive_long_data(int socket) {
    char buffer[CHUNK_SIZE];
    char *response = NULL;
    int total_bytes = 0;

    while (1) {
        int bytes_received = recv(socket, buffer, CHUNK_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Errore nella ricezione della risposta");
            free(response);
            return NULL;
        }
        if (bytes_received == 0) {
            break; // Connessione chiusa dal server
        }

        response = realloc(response, total_bytes + bytes_received + 1);
        if (!response) {
            perror("Errore nella realloc");
            return NULL;
        }

        memcpy(response + total_bytes, buffer, bytes_received);
        total_bytes += bytes_received;
        response[total_bytes] = '\0';

		// Se la risposta è terminata, esci dal ciclo
        if (strstr(response, TERMINATOR)) {
            break;
        }
    }

	// Rimuovi il terminatore dalla risposta
    char *terminator_pos = strstr(response, TERMINATOR);
    if (terminator_pos) {
        *terminator_pos = '\0';
    }

    return response;
}
