/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
 * NetId: ktong1, jhardey, josorio2
 *
 * chatserver.h
 */

#pragma once

#include "../network_utils/network_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unistd.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BACKLOG 1

class ClientMap {
    private:
        std::unordered_map<std::string, int> clients;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    public:
        ClientMap();
        void set(std::string username, int sockfd);
        int get(std::string username);
        std::vector<std::string> list_clients();
};


/* server set up */
int socket_bind_listen(int port);
int accept_connection(int sockfd);
void *client_handler(void *socket_desc);

void* connection_handler(void *sockfd);
void handle_login(void *sockfd);
