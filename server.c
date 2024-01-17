#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;

void *connection_handler(void *socket_desc){
	
	char buff[100];
	int threadsock = *(int*)socket_desc;

	bzero(buff, sizeof(buff));
	int byt = read(threadsock, buff, sizeof(buff)); // read message from client

	sem_wait(&mutex); // Lock critical section
	
	FILE *fptr = fopen("shared_file.txt", "a");  // Open file for writing
	fprintf(fptr, "%s", buff);
	fclose(fptr);

	sem_post(&mutex); // Release critical section

	printf("%s", buff);

	int valwrite = write(threadsock, buff, sizeof(buff)); // write message back to client

	close(threadsock); // close the connection
}

int main(int argc, char *argv[]){


	sem_init(&mutex, 0, 1);


	if (argc < 2){
		printf("Please supply a port to run on -> ./server <PORT>\n");
		return 0;
	}
	
	int sockfd;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);

	// Create socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket failed!");
	} else{
		printf("Socket created!\n");
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[1]));

	// Bind address to socket
	if (bind(sockfd, (struct sockaddr*)&addr, (sizeof(addr))) < 0){
		perror("Bind failed!");
	}else{
		printf("Bind successful\n");
	}
	
	// Enable socket listening
	if (listen(sockfd, 3) < 0){
		perror("Listening failed!");
	}else{
		printf("Listening...\n");
	}
	
	
	pthread_t tid[50]; // Create array of 50 threads
	int i = 0;

	for (;;){
		
		int newsockfd = accept(sockfd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);  // receive incoming connections

		if (newsockfd < 0){
			perror("Connection not accepted!");
		}
		printf("Connection accepted!\n");

		pthread_create(&tid[i], NULL, connection_handler, (void*) &newsockfd); // Create thread to handle connection
		
		if (i == 49){
			i = 0; // return thread use index back to 0
		}else{
			i++; // increment index of thread to use
		}

	}

	return 0;
}
