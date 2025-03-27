#define BUFFER_SIZE 1024
#define SIGN_UP 1
#define SIGN_IN 2

extern char username[50];

void welcome_menu(int client_socket);

int sign_in(int client_socket);

int sign_up(int client_socket);
