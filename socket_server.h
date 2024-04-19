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
#include "socket_client.h"
#define MAXLEN 100 
#define PORT 8080 
#define SA struct sockaddr
void* send_m_server(void* pack);
void* recieve_m_server(void* pack);
void* begin_server(void* connfd);
#endif