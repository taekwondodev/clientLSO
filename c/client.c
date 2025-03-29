#include "../h/client.h"
#include "../h/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <cjson/cJSON.h>

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

void search_menu(int client_socket) {
	int choice;
	char* request_type = SEARCH;
	char* request_type_option;
	char search[50];
	char *response = NULL;
	int total_bytes = 0;
	char buffer[CHUNK_SIZE];

	while(1) {
		printf("------ Cerca ------\n");
		printf("Scegli un opzione: \n");
		printf("1) Cerca per titolo \n");
		printf("2) Cerca per genere \n");
		printf("3) Torna indietro \n");

		scanf("%d", &choice);

		switch(choice) {
			case 1:
				printf("Inserisci il titolo del film: \n");
				scanf("%49s", search);
				request_type_option = SEARCH_TITLE;
				break;
			case 2:
				printf("Inserisci il genere del film: \n");
				scanf("%49s", search);
				request_type_option = SEARCH_GENRE;
				break;
			case 3:
				return;
			default:
				printf("Valore non valido\n");
		}

		// invio richiesta SEARCH
		if((send(client_socket, request_type, strlen(request_type) + 1, 0)) < 0) {
			perror("Errore nell'invio richiesta");
			return;
		}

		if((send(client_socket, request_type_option, strlen(request_type_option) + 1, 0)) < 0) {
			perror("Errore nell'invio opzione");
			return;
		}

		if((send(client_socket, search, strlen(search) + 1, 0)) < 0) {
			perror("Errore nell'invio ricerca");
			return;
		}

		printf("Caricamento...\n");

		/// ricezione risposta a blocchi perchè non sappiamo la lunghezza
		while(1) {
			int bytes_received = recv(client_socket, buffer, CHUNK_SIZE - 1, 0);
        	if (bytes_received < 0) {
            	perror("Errore nella ricezione della risposta");
				free(response);
            	return;
        	}
			if (bytes_received == 0) {
				break; // Connessione chiusa dal server
			}

			response = realloc(response, total_bytes + bytes_received + 1);
			if (!response) {
				perror("Errore nella realloc");
				return;
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
			*terminator_pos = '\0'; // Sostituisci il terminatore con il terminatore di stringa
		}

		// Parsing della risposta in JSON
        cJSON *json = cJSON_Parse(buffer);
        if (json == NULL) {
            printf("Errore nel parsing della risposta JSON\n");
			free(response);
            return;
        }

		printListofFilmJson(json);
		cJSON_Delete(json); // Libera la memoria allocata per il JSON
		free(response);
    }
}

void printListofFilmJson(cJson *json) {
	cJSON *film_list = json;
    cJSON *film;
    cJSON_ArrayForEach(film, film_list) {
        cJSON *film_id = cJSON_GetObjectItem(film, "film_id");
        cJSON *titolo = cJSON_GetObjectItem(film, "titolo");
        cJSON *genere = cJSON_GetObjectItem(film, "genere");
        cJSON *copie_totali = cJSON_GetObjectItem(film, "copie_totali");
        cJSON *copie_disponibili = cJSON_GetObjectItem(film, "copie_disponibili");

        printf("Film ID: %d\n", film_id->valueint);
        printf("Titolo: %s\n", titolo->valuestring);
        printf("Genere: %s\n", genere->valuestring);
        printf("Copie Totali: %d\n", copie_totali->valueint);
        printf("Copie Disponibili: %d\n", copie_disponibili->valueint);

        // Iterazione sui prestiti
        cJSON *prestiti = cJSON_GetObjectItem(film, "prestiti");
        if (cJSON_IsArray(prestiti)) {
            cJSON *prestito;
            cJSON_ArrayForEach(prestito, prestiti) {
                cJSON *user_id = cJSON_GetObjectItem(prestito, "user_id");
                cJSON *timestamp = cJSON_GetObjectItem(prestito, "timestamp");
                cJSON *due_date = cJSON_GetObjectItem(prestito, "due_date");

                printf("  Prestito:\n");
                printf("    User ID: %s\n", user_id->valuestring);
                printf("    Timestamp: %ld\n", timestamp->valuedouble);
                printf("    Due Date: %s\n", due_date->valuestring);
            }
        }
        printf("\n");
    }
}