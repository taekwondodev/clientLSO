#include "../h/client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
				// registrazione e break
			case 2:
				// login e break
			case 3:
				close(client_socket);
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Valore non valido");
		}

	}
}
