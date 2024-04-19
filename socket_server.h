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
#define PORT 8080 
#define SA struct sockaddr


void* send_m_server(void *ptr,gpointer* data);
void* recieve_m_server(void *ptr,gpointer* data);
void* begin_server();

#endif