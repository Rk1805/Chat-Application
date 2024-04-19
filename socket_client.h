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

#define MAX 100
#define PORT 8080
#define SA struct sockaddr

void connect_ip(char* ip);
void* send_m(void *ptr);
void* recieve_m(void *ptr);
#endif