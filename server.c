#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#define BUFFER 256

int server, client;
char client_name[BUFFER];
char buffer[BUFFER];
	
static void sighandler (int signo) {
	if (signo == SIGINT) {
		remove ("mario");
		printf ("Server closed\n");
		exit(0);
	}
}

void rot13 (char * str) {
	int shift = 13;
	while (*str) {
		if (isupper(*str)) *str = (((*str + shift) - 'A') %26) + 'A';
		else if (islower(*str)) *str = (((*str + shift) - 'a') %26) + 'a';
		
		str++;
	}
}

int main () {

	signal (SIGINT, sighandler);
	
	while (1) {
		//server creates pipe
		mkfifo ("mario", 0644);
		
		//server waits for connection
		printf ("[Server] Waiting for client connection...\n");
		server = open ("mario", O_RDONLY);
		
		//server recieves client pipe
		read (server, client_name, BUFFER);
		printf ("[Server] Client pipe recieved: %s\n", client_name);
		
		//server sends to client
		client = open (client_name, O_WRONLY);
		printf ("[Server] Giving handshake...\n");
		char message[] = "hello";
		write (client, message, sizeof(message));
		
		//server recieves client confirmation
		char client_msg[BUFFER];
		read (server, client_msg, BUFFER);
		printf ("[Server] Client confirmation recieved: %s\n", client_msg);
		
		//removes server pipe
		remove ("mario");
			
			while (1) {
			
				read (server, buffer, BUFFER);
				if (!strcmp(buffer, "stop")) {
					break;
				}
			
				rot13 (buffer);
				write (client, buffer, BUFFER);
			}
			
		close (server);
		close (client);
		printf ("[Server] Client has been disconnected\n\n");
	}
	
	return 0;
}
