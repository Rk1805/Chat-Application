#ifndef SOCKET_SERVER
#define SOCKET_SERVER

#include<gtk/gtk.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include<pthread.h>
#include<arpa/inet.h>
#include <unistd.h> // read(), write(), close()
#define MAXLEN 100 
#define PORT 8090 
#define SA struct sockaddr
struct packer
{
	void* sockf;
	gpointer* data;
};
struct chat_wind_helper
{
	char* cl_ip;
	int* connfd;
};
void* send_m_server(void* pack);
void* recieve_m_server(void* pack);
void* begin_server(void* connfd);

#endif