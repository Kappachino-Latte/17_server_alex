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
		write (server, "stop", 5);
		printf ("Client closed\n");
		exit(0);
	}
}
		
int main () {

	//client creates pipe
	sprintf (client_name, "%d", getpid());
	mkfifo (client_name, 0644);
	printf ("[Client] Pipe created\n");
		
	//client sends to server
	server = open ("mario", O_WRONLY);
	printf ("[Client] Connected to server\n");
	
	write (server, client_name, BUFFER);
	printf ("[Client] Sent pipe to server\n");
	
	//client recieves confirmation from server
	client = open (client_name, O_RDONLY);
	char server_msg[BUFFER];
	read (client, server_msg, BUFFER);
	printf ("[Client] Confirmation received: %s\n", server_msg);
		
	//client sends server confirmation
	printf ("[Client] Giving handshake...\n");
	char message[] = "hello to you too";
	write (server, message, sizeof(message));
	
	//removes client pipe
	remove (client_name);
	
	signal (SIGINT, sighandler);
	
	int input, answer;
	
	printf ("\nRot 13 Cipher, use ctrl-c to exit\n\n");
	
	while (1) {
		printf ("Enter a phrase or sentence: ");
		
		fgets (buffer, BUFFER, stdin);
		if (strchr(buffer, '\n')) *strchr(buffer, '\n') = 0;
		
		write (server, buffer, BUFFER);
		read (client, buffer, BUFFER);
		printf ("This is the cipher: %s\n", buffer);
	}
	
	return 0;
}
