#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h> // read(), write(), close()
#include "socket_server.h"
int sockfd_;
#define MAXLEN 100
#define PORT 8090
#define SA struct sockaddr
void* send_m_server(void* pack)
{
	GtkWidget *message_inp = GTK_WIDGET((((struct packer*)(pack))->data)[0]);
	const gchar *message = gtk_entry_get_text((GtkEntry *)message_inp);
	write(sockfd_, message, sizeof(message));
	printf("You: %s", message);
	return NULL;
}

void* recieve_m_server(void* pack)
{
	int sockfd_ = *(int *)(((struct packer*)pack)->sockf);
	char buff[MAXLEN];
	int n;
	for (;;)
	{
		bzero(buff, sizeof(buff));
		read(sockfd_, buff, sizeof(buff));
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
void *begin_server(void* helper)
{
	pthread_t send_thread, recieve_thread;

	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_ == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORTm
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd_, (SA *)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd_, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	socklen_t len;
	len = sizeof(cli);

	// Accept the data packet from client and verification
	*(int*)(((struct chat_wind_helper*)helper)->connfd) = accept(sockfd_, (SA *)&cli, &len);
	if (*(int*)(((struct chat_wind_helper*)helper)->connfd) < 0)
	{
		printf("Server accept failed...\n");
		exit(0);
	}
	else
	{
		// Retrieve client's IP address
		char client_ip[25];
		struct sockaddr_in *client_addr = (struct sockaddr_in *)&cli;
		inet_ntop(AF_INET, &(client_addr->sin_addr), client_ip, INET_ADDRSTRLEN);
		((struct chat_wind_helper*)helper)->cl_ip = client_ip;
		// GtkWidget* st_label = GTK_WIDGET(((struct chat_wind_helper*)helper)->data[1]);
		// gtk_label_set_label(GTK_LABEL(st_label),"Connected");
		printf("Connected with the client\n");
	}

	// pthread_create(&send_thread, NULL, send_m_server, (int*)(((struct chat_wind_helper*)helper)->connfd));
	// pthread_create(&recieve_thread, NULL, recieve_m_server, (int*)(((struct chat_wind_helper*)helper)->connfd));

	// pthread_join(send_thread, NULL);
	// pthread_join(recieve_thread, NULL);
	struct packer* pack_ = (struct packer*)malloc(sizeof(struct packer));
	pack_->data = NULL;
	pack_->sockf = &sockfd_;
	recieve_m_server(pack_);

	// After chatting close the socket
	close(sockfd_);
	return 0;
}
