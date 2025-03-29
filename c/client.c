#include "../h/client.h"
#include "../h/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

static char username[50];

int sign_up(int client_socket) {
	int result;
	char* request_type = SIGN_UP;
	char _username[50];
	char password[50];
	unsigned char hashed_password[SHA256_DIGEST_LENGTH];

	printf("*****************\n");
	printf("Inserisci uno username: \n");
	scanf("%49s", _username);

	printf("Inserisci una password: \n");
	scanf("%49s", password);

	hash_password(password, hashed_password);

	// invio richiesta SIGN_UP
	if((send(client_socket, request_type, strlen(request_type) + 1, 0)) < 0) {
		perror("Errore nell'invio richiesta");
		return 1;
	}

	// invio username
	if((send(client_socket, _username, strlen(_username) + 1, 0)) < 0) {
		perror("Errore nell'invio username");
		return 1;
	}

	// invio password
	if((send(client_socket, hashed_password, SHA256_DIGEST_LENGTH, 0)) < 0) {
		perror("Errore nell'invio password");
		return 1;
	}

	printf("Caricamento...\n");

	// ricevuta risposta
	recv(client_socket, &result, sizeof(result), 0);

	if(result == 0) {
		printf("Registrazione avvenuta con successo!");
		strcpy(username, _username);
		return result;
	}
	else {
		perror("Errore nel ricevere il messaggio");
		return 1;
	}
}

int sign_in(int client_socket){
	int result;
	char* request_type = SIGN_IN;
	char _username[50];
	char password[50];
	unsigned char hashed_password[SHA256_DIGEST_LENGTH];

	printf("*****************\n");
	printf("Inserisci uno username: \n");
	scanf("%49s", _username);

	printf("Inserisci una password: \n");
	scanf("%49s", password);

	hash_password(password, hashed_password);

	// invio richiesta SIGN_IN
	if((send(client_socket, request_type, strlen(request_type) + 1, 0)) < 0) {
		perror("Errore nell'invio richiesta");
		return 1;
	}

	// invio username
	if((send(client_socket, _username, strlen(_username) + 1, 0)) < 0) {
		perror("Errore nell'invio username");
		return 1;
	}

	// invio password
	if((send(client_socket, hashed_password, SHA256_DIGEST_LENGTH, 0)) < 0) {
		perror("Errore nell'invio password");
		return 1;
	}

	printf("Caricamento...\n");

	// ricevuta risposta
	recv(client_socket, &result, sizeof(result), 0);

	if(result == 0) {
		printf("Login avvenuto con successo!");
		strcpy(username, _username);
		return result;
	}
	else {
		perror("Errore nel ricevere il messaggio");
		return 1;
	}
}

void welcome_menu(int client_socket) {
	int choice;

	while(1) {
		printf("Per usare il Sistema devi autenticarti\n");
		printf("Scegli un opzione: \n");
		printf("1) Registrati al Sistema \n");
		printf("2) Accedi al Sistema \n");
		printf("3) Esci \n");

		scanf("%d", &choice);

		switch(choice) {
			case 1:
				if(sign_up(client_socket) == 0)
					return;
			case 2:
				if(sign_in(client_socket) == 0)
					return;
			case 3:
				close(client_socket);
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Valore non valido\n");
		}
	}
}

void home_menu(int client_socket) {
	int choice;

	while(1) {
		printf("------ Home ------\n");
		printf("Scegli un opzione: \n");
		printf("1) Cercare un film \n");
		printf("2) Noleggiare un film \n");
		printf("3) Restituire un film \n");
		printf("4) Visualizzare i miei noleggi \n");
		printf("5) Esci \n");

		scanf("%d", &choice);

		switch(choice) {
			case 1:
				
			case 2:
				
			case 3:

			case 4:
				close(client_socket);
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Valore non valido\n");
		}
	}
}
