#ifndef SOCKET_CLIENT

#define SOCKET_CLIENT
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
#include "socket_server.h"
#define MAXLEN 100
#define PORT 8090
#define SA struct sockaddr


int connect_ip(char* ip,gpointer* data);
void* send_m(void* pack);
void* recieve_m(void* pack);


#endif