#include "socket_client.h"
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
		printf("You : %s", buff);
		
	}
	return NULL;
}

void* recieve_m(void *ptr)
{
	int sockfd=*((int*)ptr);
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf(" Exit...\n");
			break;
		}
	}
	return NULL; 
}

void connect_ip(char* ip)
{
	pthread_t send_thread, recieve_thread;
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
	{
		printf("Socket successfully created..\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!= 0)
	{
		printf("Connection with the server failed...\n");
		exit(0);
	}
	else
	{	
		printf("connected to the server..\n");
	}
	pthread_create(&send_thread,NULL,send_m,&sockfd);
	pthread_create(&recieve_thread,NULL,recieve_m,&sockfd);

	pthread_join(send_thread,NULL);
	pthread_join(recieve_thread,NULL);


	// close the socket
	close(sockfd);
}