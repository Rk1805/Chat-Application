#ifndef SOCKET_CLIENT

#define SCKET_CLIENT
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include<pthread.h>
#include<gtk/gtk.h>

#define MAXLEN 100
#define PORT 8080
#define SA struct sockaddr

struct packer
{
	void* sockf;
	gpointer* data;
};
void connect_ip(char* ip,gpointer* data);
void* send_m(struct packer* pack);
void* recieve_m(struct packer* pack);


#endif