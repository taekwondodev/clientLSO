#include "../h/socket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

int open_socket() {
	int sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Errore creazione socket");
		exit(EXIT_FAILURE);
	}

	return sock;
}

void setup_server_address(struct sockaddr_in *server_addr) {
    struct hostent *server;

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);

    server = gethostbyname("server");
    if (server == NULL) {
        fprintf(stderr, "Errore: host server non trovato\n");
        exit(EXIT_FAILURE);
    }
    memcpy(&server_addr->sin_addr.s_addr, server->h_addr, server->h_length);
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
    size_t total_bytes = 0;
    int bytes_received;

    while (1) {
        bytes_received = recv(socket, buffer, CHUNK_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Errore nella ricezione della risposta");
            if (response) free(response);
            return NULL;
        }
        if (bytes_received == 0) {
            break; // Connessione chiusa dal server
        }

        // Aggiungi terminatore
        buffer[bytes_received] = '\0';

        // Alloca/rialloca memoria
        char *temp = realloc(response, total_bytes + bytes_received + 1);
        if (!temp) {
            perror("Errore nella realloc");
            if (response) free(response);
            return NULL;
        }
        response = temp;

        // Copia i dati nel buffer finale
        memcpy(response + total_bytes, buffer, bytes_received + 1);
        total_bytes += bytes_received;

		// Se la risposta è terminata, esci dal ciclo
        if (strstr(response, TERMINATOR)) {
            break;
        }
    }

	 // Se non è stato ricevuto nulla
     if (!response) {
        return NULL;
    }

    // Rimuovi il terminatore (se presente)
    char *terminator_pos = strstr(response, TERMINATOR);
    if (terminator_pos) {
        *terminator_pos = '\0';
    }

    return response;
}
