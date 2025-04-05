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

static char username[50];

int sign_up(int client_socket) {
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
        	perror("Errore nell'invio richiesta\n");
       		return 1;
	}

	printf("Caricamento...\n");

	char *response = receive_long_data(client_socket);
	if (!response) {
        printf("Errore nella ricezione della risposta\n");
        return 1;
    }

    printf("%s\n", response);

	if(strstr(response, "successo")) {
		strcpy(username, _username);
		free(response);
		return 0;
	}
	else if(strstr(response, "Formato comando errato")) {
		free(response);
		return 1;
	}
	else if(strstr(response, "utente già registrato")){
		free(response);
		return 1;
	}
	else {
		free(response);
		return 1;
	}
}

int sign_in(int client_socket){
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

	char *response = receive_long_data(client_socket);
	if (!response) {
        printf("Errore nella ricezione della risposta\n");
        return 1;
    }

    printf("%s\n", response);

	if(strstr(response, "Attenzione")) {
		strcpy(username, _username);
		free(response);
		return 0;
	}
	else if(strstr(response, "effettuato")) {
		strcpy(username, _username);
		free(response);
		return 0;
	}
	else if(strstr(response, "Formato comando errato")){
		free(response);
		return 1;
	}

	else if(strstr(response, "Username o password errati")){
		free(response);
		return 1;
	}
	else {
		free(response);
		return 1;
	}
}

int rent(int client_socket, int film_id) {
	char request[256];

	time_t timestamp = time(NULL);
	char return_date[11];
	struct tm *tm_info = localtime(&timestamp);
	strftime(return_date, sizeof(return_date), "%Y-%m-%d", tm_info);

	printf("Noleggio del film con ID: %d\n", film_id);

	snprintf(request, sizeof(request), "%s|%s|%d|%s\n", RENT, username, film_id, return_date); 

	if (send(client_socket, request, strlen(request), 0) < 0) {
        perror("Errore nell'invio della richiesta\n");
        return 1;
	}

	printf("Caricamento...\n");

	char *response = receive_long_data(client_socket);
	if (!response) {
        printf("Errore nella ricezione della risposta\n");
        return 1;
    }

    printf("%s\n", response);

	if(strstr(response, "successo")) {
		free(response);
		return 0;
	}
	else if(strstr(response, "Formato comando errato")) {
		free(response);
		return 1;
	}
	else if (strstr(response, "Film non trovato")){
		free(response);
		return 1;
	}
	else if (strstr(response, "Numero massimo di film noleggiati raggiunto")){
		free(response);
		return 1;
	}
	else if (strstr(response, "Nessuna")){
		free(response);
		return 1;
	}
	else {
		free(response);
		return 1;
	}
}

int return_film(int client_socket, int film_id){
	char request[256];

	printf("Restituzione del film con ID: %d\n", film_id);

	snprintf(request, sizeof(request), "%s|%s|%d\n", RETURN, username, film_id);

	if (send(client_socket, request, strlen(request), 0) < 0) {
        perror("Errore nell'invio della richiesta\n");
        return 1;
	}

	printf("Caricamento...\n");

	char *response = receive_long_data(client_socket);
	if (!response) {
        printf("Errore nella ricezione della risposta\n");
        return 1;
    }

    printf("%s\n", response);

	if(strstr(response, "successo")) {
		free(response);
		return 0;
	}
	else if(strstr(response, "Formato comando errato")) {
		free(response);
		return 1;
	}
	else if(strstr(response, "non trovato")){
		free(response);
		return 1;
	}
	else {
		free(response);
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

void return_operation(int client_socket) {
	int selected_index;

	while(1) {
		printf("Vuoi restituire un film? Inserisci l'indice del film: \n");
		printf("Inserisci 0 per annullare: \n");
		scanf("%d", &selected_index);

		if(selected_index == 0) {
			break;
		}
	
		if (selected_index > 0) {
			int result = return_film(client_socket, selected_index);

			// se tutto ok, finisce il ciclo, altrimenti riparte
			if (result == 0) {
				break;
			}
		} else {
			printf("Indice non valido. Nessun film selezionato.\n");
		}
	}
}

void checkout(int client_socket, int *cart_size, int *cart) {
	int choice;

	printf("------ Checkout ------\n");
	printf("Sei sicuro di voler noleggiare i film selezionati? \n");
	printf("1) Per confermare \n");
	printf("0) Per annullare \n");
	scanf("%d", &choice);

	if (choice != 1) {
		printf("Operazione annullata. Torno al menu precedente.\n");
		return;
	}

	printf("Effettuando il checkout per i seguenti film:\n");
    for (int i = 0; i < *cart_size;) {
        printf("Film ID: %d\n", cart[i]);
        int result = rent(client_socket, cart[i]);

		if(result == 0){
			// Rimuovi l'elemento spostando gli elementi successivi
            for (int j = i; j < *cart_size - 1; j++) {
                cart[j] = cart[j + 1];
            }
            (*cart_size)--;
        } else {
            i++;
        }
    }
    printf("Operazione completata. Torno al menu precedente.\n");
}

void rent_operation(int client_socket) {
    int selected_index;
	int cart[RENT_MAX];
	int cart_size = 0;

	while(1) {
		// controllo se il carrello è pieno, se si direttamente checkout
		if (cart_size >= RENT_MAX) {
			checkout(client_socket, &cart_size, cart);
			break;
		}
		
		printf("Vuoi noleggiare un film? Inserisci l'indice del film: \n");
		printf("Inserisci 0 per annullare o per andare al checkout: \n");
		scanf("%d", &selected_index);

		if(selected_index == 0) {
			if (cart_size > 0) {
				checkout(client_socket, &cart_size, cart);
				break;
			} else {
				break;
			}
		}else if (selected_index > 0) {
			if (cart_size < RENT_MAX) {
				cart[cart_size++] = selected_index;
				printf("Film con ID %d aggiunto al carrello.\n", selected_index);
			}
		} else {
			printf("Indice non valido. Nessun film selezionato.\n");
		}
	}
}

void return_menu(int client_socket) {
	char request[256];
	char *response = NULL;

	printf("------ Restituisci ------\n");

	snprintf(request, sizeof(request), "%s|%s\n", MY_RENTALS, username);

	if (send(client_socket, request, strlen(request), 0) < 0) {
        perror("Errore nell'invio della richiesta\n");
        return;
	}
	
	printf("Caricamento...\n");

	response = receive_long_data(client_socket);
	if(!response) {
		perror("Errore nella ricezione della risposta\n");
		return;
	}

	printf("%s\n", response);

	free(response);
	response = NULL;

	return_operation(client_socket);
}

void search_menu(int client_socket) {
	int choice;
	char request[256];
	char *request_type_option;
	char search[50];

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
				fgets(search, sizeof(search), stdin);
				search[strcspn(search, "\n")] = '\0'; // Rimuove il carattere di newline
				request_type_option = SEARCH_TITLE;
				break;
			case 2:
				printf("Inserisci il genere del film: \n");
				fgets(search, sizeof(search), stdin);
				search[strcspn(search, "\n")] = '\0'; // Rimuove il carattere di newline
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
        	perror("Errore nell'invio della richiesta\n");
       		return;
		}

		printf("Caricamento...\n");

		char *response = receive_long_data(client_socket);
		if (!response) {
			perror("Errore nella ricezione della risposta\n");
			return;
		}

		printf("%s\n", response);

		if(strlen(response) == 0 || strstr(response, "Nessun film trovato")) {
			free(response);
			continue;
		}
		free(response);
		response = NULL;

		rent_operation(client_socket);
    }
}