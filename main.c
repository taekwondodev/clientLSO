#include "h/client.h"
#include "h/socket.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static int sock;

void sig_handler(int signo) {
	if (signo == SIGINT || signo == SIGTERM) {
		printf("Exiting...closing socket\n");
		close(sock);
		exit(0);
	}
}

int main() {
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	struct sockaddr_un server_addr;

	sock = open_socket();
	setup_server_address(&server_addr);
	connection_to_server(sock, &server_addr);

	welcome_menu(sock);

	return 0;
}
