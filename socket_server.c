#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include<pthread.h>
#include <unistd.h> // read(), write(), close()

#define MAX 100 
#define PORT 8080 
#define SA struct sockaddr 

void* send_m(void *ptr)
{
	int sockfd=*((int*)ptr);
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		if ((strncmp(buff, "exit", 4)) == 0) 
		{
			printf("Exit...\n");
			break;
		}
		printf("You: %s", buff);
	}
	return NULL;
}

void* recieve_m(void *ptr)
{
	int sockfd=*((int*)ptr);
	char buff[MAX];
	int n;
	for (;;) 
	{
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("Client : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) 
		{
			printf("Exit...\n");
			break;
		}
	}
	return NULL;
}

// Driver function 
int main() 
{ 
	pthread_t send_thread, recieve_thread;
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("Socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n");
    socklen_t len; 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server accept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server accept the client...\n"); 
		
	pthread_create(&send_thread,NULL,send_m,&connfd);
	pthread_create(&recieve_thread,NULL,recieve_m,&connfd);

	pthread_join(send_thread,NULL);
	pthread_join(recieve_thread,NULL);

	// After chatting close the socket 
	close(sockfd); 
	return 0;
}
