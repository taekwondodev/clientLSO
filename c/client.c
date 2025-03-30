#include "../h/client.h"
#include "../h/socket.h"
#include "../h/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <cjson/cJSON.h>

static char username[50];

int sign_up(int client_socket) {
	int result;
	char *request_type = SIGN_UP;
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
	char *request_type = SIGN_IN;
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

int rent(int client_socket, int film_id) {
	int result;
	char *request_type = RENT;
	time_t timestamp = time(NULL);

	printf("Noleggio del film con ID: %d\n", film_id);

	if((send(client_socket, request_type, strlen(request_type) + 1, 0)) < 0) {
		perror("Errore nell'invio richiesta");
		return 1;
	}

	if((send(client_socket, username, strlen(username) + 1, 0)) < 0) {
		perror("Errore nell'invio username");
		return 1;
	}

	if((send(client_socket, &film_id, sizeof(film_id), 0)) < 0) {
		perror("Errore nell'invio film_id");
		return 1;
	}

	if((send(client_socket, &timestamp, sizeof(timestamp), 0)) < 0) {
		perror("Errore nell'invio timestamp");
		return 1;
	}

	printf("Caricamento...\n");

	recv(client_socket, &result, sizeof(result), 0);
	if(result == 0) {
		printf("Noleggio avvenuto con successo!");
		return result;
	}
	else {
		perror("Errore nel ricevere il messaggio");
		return 1;
	}
}

int return_film(int client_socket, int film_id){
	int result;
	char *request_type = RETURN;

	printf("Restituzione del film con ID: %d\n", film_id);

	if((send(client_socket, request_type, strlen(request_type) + 1, 0)) < 0) {
		perror("Errore nell'invio richiesta");
		return 1;
	}

	if((send(client_socket, username, strlen(username) + 1, 0)) < 0) {
		perror("Errore nell'invio username");
		return 1;
	}

	if((send(client_socket, &film_id, sizeof(film_id), 0)) < 0) {
		perror("Errore nell'invio film_id");
		return 1;
	}

	printf("Caricamento...\n");

	recv(client_socket, &result, sizeof(result), 0);
	if(result == 0) {
		printf("Restituzione avvenuta con successo!");
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
		printf("2) Restituire un film \n");
		printf("3) Esci \n");

		scanf("%d", &choice);

		switch(choice) {
			case 1:
				search_menu(client_socket);
			case 2:
				return_menu(client_socket);
			case 3:
				close(client_socket);
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Valore non valido\n");
		}
	}
}

void return_menu(int client_socket) {
	char *request_type = MY_RENTALS;
	char *response = NULL;

	printf("------ Restituisci ------\n");

	if ((send(client_socket, request_type, strlen(request_type) + 1, 0)) < 0) {
		perror("Errore nell'invio richiesta");
		return;
	}

	if ((send(client_socket, username, strlen(username) + 1, 0)) < 0) {
		perror("Errore nell'invio username");
		return;
	}

	printf("Caricamento...\n");

	response = receive_long_data(client_socket);
	if(!response) {
		perror("Errore nella ricezione della risposta");
		return;
	}

	cJSON *json = parse_json(response);
    if (json) {
        printListofFilmJson(json, true);
        cJSON_Delete(json);
    }
    free(response);
}

void search_menu(int client_socket) {
	int choice;
	char *request_type = SEARCH;
	char *request_type_option;
	char search[50];
	char *response = NULL;

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
			case 2:
				printf("Inserisci il genere del film: \n");
				scanf("%49s", search);
				request_type_option = SEARCH_GENRE;
			case 3:
				return;
			default:
				printf("Valore non valido\n");
		}

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

		response = receive_long_data(client_socket);
		if (!response) {
			perror("Errore nella ricezione della risposta");
			return;
		}

		cJSON *json = parse_json(response);
		if (json) {
			printListofFilmJson(json, false);
			cJSON_Delete(json);
		}
		free(response);
    }
}

/*******************************************************************************************/

void printListofFilmJson(cJson *json, bool isReturn) {
	cJSON *film_list = json;
    cJSON *film;
	int index = 1;

    cJSON_ArrayForEach(film, film_list) {
        cJSON *film_id = cJSON_GetObjectItem(film, "film_id");
        cJSON *titolo = cJSON_GetObjectItem(film, "titolo");
        cJSON *genere = cJSON_GetObjectItem(film, "genere");
        cJSON *copie_totali = cJSON_GetObjectItem(film, "copie_totali");
        cJSON *copie_disponibili = cJSON_GetObjectItem(film, "copie_disponibili");

        printf("[%d] Film ID: %d\n", index, film_id->valueint);
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
		index++;
    }

	if (isReturn) {
		return_operation(index, film_list, film);
	} else {
		rent_operation(index, film_list, film);
	}
}

void return_operation(int index, cJSON *film_list, cJSON *film) {
	int selected_index;

	while(1) {
		printf("Vuoi restituire un film? Inserisci l'indice del film: \n");
		printf("Inserisci 0 per annullare: ");
		scanf("%d", &selected_index);

		if(selected_index == 0) {
			break;
		}
	
		if (selected_index > 0 && selected_index < index) {
			// Trova il film selezionato
			int current_index = 1;
			int result = 0;
			cJSON_ArrayForEach(film, film_list) {
				if (current_index == selected_index) {
					cJSON *film_id = cJSON_GetObjectItem(film, "film_id");
					return_film(client_socket, film_id->valueint);
					break;
				}
				current_index++;
			}

			// se tutto ok, finisce il ciclo, altrimenti riparte
			if (result == 0) {
				break;
			}
		} else {
			printf("Indice non valido. Nessun film selezionato.\n");
		}
	}
}

void rent_operation(int index, cJSON *film_list, cJSON *film) {
    int selected_index;
	int cart[RENT_MAX];
	int cart_size = 0;

	while(1) {
		// controllo se il carrello è pieno, se si direttamente checkout
		if (cart_size >= RENT_MAX) {
			checkout(client_socket, cart_size, cart);
		}
		
		printf("Vuoi noleggiare un film? Inserisci l'indice del film: \n");
		printf("Inserisci 0 per annullare o per andare al checkout: ");
		scanf("%d", &selected_index);

		if(selected_index == 0) {
			if (cart_size > 0) {
				checkout(client_socket, cart_size, cart);
			} else {
				break;
			}
		}
	
		if (selected_index > 0 && selected_index < index) {
			// Trova il film selezionato
			int current_index = 1;
			cJSON_ArrayForEach(film, film_list) {
				if (current_index == selected_index) {
					cJSON *film_id = cJSON_GetObjectItem(film, "film_id");

					if (cart_size < RENT_MAX) {
						cart[cart_size++] = film_id->valueint;
						printf("Film con ID %d aggiunto al carrello.\n", film_id->valueint);
					}
					break;
				}
				current_index++;
			}
		} else {
			printf("Indice non valido. Nessun film selezionato.\n");
		}
	}
}

void checkout(int client_socket, int cart_size, int *cart) {
	printf("Effettuando il checkout per i seguenti film:\n");
    for (int i = 0; i < cart_size; i++) {
        printf("Film ID: %d\n", cart[i]);
        rent(client_socket, cart[i]);
    }
    printf("Operazione completata. Torno al menu precedente.\n");
    break;
}

// Helper per parsing JSON
cJSON* parse_json(const char *data) {
    cJSON *json = cJSON_Parse(data);
    if (json == NULL) {
        printf("Errore nel parsing della risposta JSON\n");
    }
    return json;
}