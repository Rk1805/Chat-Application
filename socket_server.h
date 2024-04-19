#ifndef SOCKET_SERVER
#define SOCKET_SERVER

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


void* send_m(void *ptr);
void* recieve_m(void *ptr);
int begin_server();

#endif