#define RENT_MAX 5

#define SIGN_UP "REGISTER"
#define SIGN_IN "LOGIN"
#define SEARCH "SEARCH"
#define SEARCH_TITLE "TITLE"
#define SEARCH_GENRE "GENRE"
#define RENT "RENT"
#define RETURN "RETURN"
#define MY_RENTALS "MYRENTALS"

void welcome_menu(int client_socket);

int sign_in(int client_socket);

int sign_up(int client_socket);

int rent(int client_socket, int film_id);

int return_film(int client_socket, int film_id);

void home_menu(int client_socket);

void search_menu(int client_socket);

void return_menu(int client_socket);