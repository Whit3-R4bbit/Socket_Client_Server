#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){


	
	int sockfd;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);

	// Create client socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket failed!");
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of server
	addr.sin_port = htons(8000); // Port for server connection

	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0){
		printf("Cannot connect to server!\n");
	} else {
		printf("Connected to server!\n");
	}	
	
	char message[] = "My first message!\n";
	char response[100];
	write(sockfd, message, sizeof(message)); // Write message to server

	read(sockfd, response, sizeof(response)); // Read response from server
	printf("%s", response);

	close(sockfd); // Close connection
	return 0;
}
