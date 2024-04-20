#include "socket_client.h"

int sockfd = - 1;
void* send_m(void* pack)
{
	// int sockfd=*((int*)(pack->sockf));
	GtkWidget *message_inp = GTK_WIDGET((((struct packer*)(pack))->data)[0]);
	const gchar *message = gtk_entry_get_text((GtkEntry *)message_inp);
	write(sockfd, message, sizeof(message));
	printf("You: %s", message);
	return NULL;
}

void* recieve_m(void* pack)
{
	int sockfd_ = *((int*)(((struct packer*)pack)->sockf));
	char buff[MAXLEN];
	int n;
	GtkWidget* rec_name_panel = GTK_WIDGET((((struct packer*)pack)->data)[0]);
    const gchar* rec_name = gtk_entry_get_text((GtkEntry*)rec_name_panel);
	for (;;) {
		bzero(buff, sizeof(buff));
		read(sockfd_, buff, sizeof(buff));
		GtkWidget *message_inp = GTK_WIDGET((((struct packer*)pack)->data)[2]);
    	GtkWidget *text_box = GTK_WIDGET((((struct packer*)pack)->data)[3]);
		GtkTextIter iter;
    	GtkTextBuffer* buffer;
    	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_box));
    	gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_buffer_insert(buffer,&iter,rec_name,-1);
    	gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_buffer_insert(buffer,&iter,": ",-1);
    	gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_buffer_insert(buffer,&iter,buff,-1);
    	gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_buffer_insert(buffer,&iter,"\n",-1);
		printf("Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf(" Exit...\n");
			break;
		}
	}
	return NULL; 
}


int connect_ip(char* ip,gpointer* data)
{
	pthread_t send_thread, recieve_thread;
	int connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Socket creation failed...\n");
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
		// exit(0);
		return 0;
	}
	else
	{
		// GtkWidget* label = GTK_WIDGET(data[1]);
		// gtk_label_set_label(GTK_LABEL(label),"Connected");
		printf("Connected to the server..\n");
	}
	struct packer* pack1 = (struct packer*)malloc(sizeof(struct packer));
	pack1->sockf = &sockfd;
	pack1->data = data;

	pthread_create(&send_thread,NULL,send_m,pack1);
	pthread_create(&recieve_thread,NULL,recieve_m,pack1);

	
	pthread_join(send_thread,NULL);
	pthread_join(recieve_thread,NULL);

	// close the socket
	close(sockfd);
}