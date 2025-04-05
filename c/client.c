#include "../h/client.h"
#include "../h/socket.h"
#include "../h/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <openssl/sha.h>
#include <cjson/cJSON.h>

static char username[50];

int sign_up(int client_socket) {
	char result[256];
	char request[512];
	char _username[50];
	char password[50];
	unsigned char hashed_password[SHA256_DIGEST_LENGTH];
	char hashed_password_hex[2*SHA256_DIGEST_LENGTH + 1] = {0}; 

	printf("*****************\n");
	printf("Inserisci uno username: \n");
	scanf("%49s", _username);

	printf("Inserisci una password: \n");
	scanf("%49s", password);

	hash_password(password, hashed_password);
	// Converti l'hash binario in stringa esadecimale
    	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        	snprintf(hashed_password_hex + 2*i, 3, "%02x", hashed_password[i]);
    	}

	snprintf(request, sizeof(request), "%s|%s|%s\n", SIGN_UP, _username, hashed_password_hex); 	

	if (send(client_socket, request, strlen(request), 0) < 0) {
        	perror("Errore nell'invio richiesta");
       		return 1;
	}

	printf("Caricamento...\n");

	recv(client_socket, result, sizeof(result), 0);

	if(strcmp(result, "successo") == 0) {
		printf("Registrazione avvenuta con successo!");
		strcpy(username, _username);
		return 0;
	}
	else {
		printf("Errore nel ricevere il messaggio");
		return 1;
	}
}

int sign_in(int client_socket){
	char result[256];
	char request[256];
	char _username[50];
	char password[50];
	unsigned char hashed_password[SHA256_DIGEST_LENGTH];
	char hashed_password_hex[2*SHA256_DIGEST_LENGTH + 1] = {0};


	printf("*****************\n");
	printf("Inserisci uno username: \n");
	scanf("%49s", _username);

	printf("Inserisci una password: \n");
	scanf("%49s", password);

	hash_password(password, hashed_password);
	// Converti l'hash binario in stringa esadecimale
    	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
       		snprintf(hashed_password_hex + 2*i, 3, "%02x", hashed_password[i]);
   	}

	snprintf(request, sizeof(request), "%s|%s|%s\n", SIGN_IN, _username, hashed_password_hex);

	if (send(client_socket, request, strlen(request), 0) < 0) {
        	perror("Errore nell'invio richiesta");
        	return 1;
	}

	printf("Caricamento...\n");

	recv(client_socket, result, sizeof(result), 0);

	if(strcmp(result, "effettuato") == 0) {
		printf("Login avvenuto con successo!");
		strcpy(username, _username);
		return 0;
	}
	else {
		printf("Errore nel ricevere il messaggio");
		return 1;
	}
}

int rent(int client_socket, int film_id) {
	char result[256];
	char request[256];

	time_t timestamp = time(NULL);
	char return_date[11];
	struct tm *tm_info = localtime(&timestamp);
	strftime(return_date, sizeof(return_date), "%Y-%m-%d", tm_info);

	printf("Noleggio del film con ID: %d\n", film_id);

	snprintf(request, sizeof(request), "%s|%s|%d|%s\n", RENT, username, film_id, return_date); 

	if (send(client_socket, request, strlen(request), 0) < 0) {
        	perror("Errore nell'invio della richiesta");
        	return 1;
	}

	printf("Caricamento...\n");

	recv(client_socket, result, sizeof(result), 0);
	if(strcmp(result, "successo") == 0) {
		printf("Noleggio avvenuto con successo!");
		return 0;
	}
	else if (strcmp(result, "Nessuna") == 0){
		printf("Nessuna copia disponibile");
		return 1;
	}
	else {
		printf("Errore risposta server");
		return 1;
	}
}

int return_film(int client_socket, int film_id){
	char result[256];
	char request[256];

	printf("Restituzione del film con ID: %d\n", film_id);

	snprintf(request, sizeof(result), "%s|%s|%d\n", RETURN, username, film_id);

	if (send(client_socket, request, strlen(request), 0) < 0) {
        	perror("Errore nell'invio della richiesta");
        	return 1;
	}

	printf("Caricamento...\n");

	recv(client_socket, result, sizeof(result), 0);
	if(strcmp(result, "successo") == 0) {
		printf("Restituzione avvenuta con successo!");
		return 0;
	}
	else if(strcmp(result, "non trovato") == 0){
		printf("Film non trovato");
		return 1;
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
				break;
			case 2:
				if(sign_in(client_socket) == 0)
					return;
				break;
			case 3:
				close(client_socket);
				exit(EXIT_SUCCESS);
			default:
				printf("Valore non valido\n");
				break;
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
				break;
			case 2:
				return_menu(client_socket);
				break;
			case 3:
				close(client_socket);
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Valore non valido\n");
		}
	}
}

// Helper per parsing JSON
cJSON* parse_json(const char *data) {
    cJSON *json = cJSON_Parse(data);
    if (json == NULL) {
        printf("Errore nel parsing della risposta JSON\n");
    }
    return json;
}

void return_operation(int index, cJSON *film_list, cJSON *film, int client_socket) {
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
					result = return_film(client_socket, film_id->valueint);
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

void checkout(int client_socket, int cart_size, int *cart) {
	int choice;

	printf("------ Checkout ------\n");
	printf("Sei sicuro di voler noleggiare i film selezionati? \n");
	printf("1) Per confermare \n");
	printf("2) Per annullare \n");
	scanf("%d", &choice);

	if (choice != 1) {
		printf("Operazione annullata. Torno al menu precedente.\n");
		return;
	}

	printf("Effettuando il checkout per i seguenti film:\n");
    for (int i = 0; i < cart_size; i++) {
        printf("Film ID: %d\n", cart[i]);
        rent(client_socket, cart[i]);
    }
    printf("Operazione completata. Torno al menu precedente.\n");
}

void rent_operation(int index, cJSON *film_list, cJSON *film, int client_socket) {
    int selected_index;
	int cart[RENT_MAX];
	int cart_size = 0;

	while(1) {
		// controllo se il carrello è pieno, se si direttamente checkout
		if (cart_size >= RENT_MAX) {
			checkout(client_socket, cart_size, cart);
			break;
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

void printListofFilmJson(cJSON *json, bool isReturn, int client_socket) {
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
                printf("    Timestamp: %s\n", timestamp->valuestring);
                printf("    Due Date: %s\n", due_date->valuestring);
            }
        }
        printf("\n");
		index++;
    }

	if (isReturn) {
		return_operation(index, film_list, film, client_socket);
	} else {
		rent_operation(index, film_list, film, client_socket);
	}
}

void return_menu(int client_socket) {
	char request[256];
	char *response = NULL;

	printf("------ Restituisci ------\n");

	snprintf(request, sizeof(request), "%s|%s\n", MY_RENTALS, username);

	if (send(client_socket, request, strlen(request), 0) < 0) {
        	perror("Errore nell'invio della richiesta");
        	return;
	}
	
	printf("Caricamento...\n");

	response = receive_long_data(client_socket);
	if(!response) {
		perror("Errore nella ricezione della risposta");
		return;
	}

	// Controlla se la risposta inizia con '{' o '[' per determinare se è JSON
    	if (response[0] == '{' || response[0] == '[') {
        	cJSON *json = parse_json(response);
        	if (json) {
            		printListofFilmJson(json, true, client_socket);
           		    cJSON_Delete(json);
        	} else {
           		printf("Errore nel parsing della risposta JSON.\n");
        	}
    	} else {
        	// Se non è JSON, stampiamo il messaggio di errore ricevuto
        	printf("Errore ricevuto dal server: %s\n", response);
   	}

	free(response);
	response = NULL;
}

void search_menu(int client_socket) {
	int choice;
	char request[256];
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

		// Reset search and request_type_option for each iteration
        memset(search, 0, sizeof(search));
        request_type_option = NULL;

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
				continue;
		}

		snprintf(request, sizeof(request), "%s|%s|%s\n", SEARCH, request_type_option, search);

		if (send(client_socket, request, strlen(request), 0) < 0) {
        		perror("Errore nell'invio della richiesta");
       			return;
		}

		printf("Caricamento...\n");

		response = receive_long_data(client_socket);
		if (!response) {
			perror("Errore nella ricezione della risposta");
			return;
		}

		// Controlla se la risposta inizia con '{' o '[' per determinare se è JSON
    		if (response[0] == '{' || response[0] == '[') {
        		cJSON *json = parse_json(response);
        		if (json) {
            			printListofFilmJson(json, false, client_socket);
           			cJSON_Delete(json);
        		} else {
            			printf("Errore nel parsing della risposta JSON.\n");
       			}
    		} else {
        		// Se non è JSON, stampiamo il messaggio di errore ricevuto
        		printf("Errore ricevuto dal server: %s\n", response);
    		}

		free(response);
		response = NULL;
    }
}