/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
 * NetId: ktong1, jhardey, josorio2
 *
 * chatserver.h
 */

#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "../network_utils/network_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>
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


struct ClientInfo{
    int sock;
    char* pubkey;
    bool empty = false;

    ClientInfo() {}
    ClientInfo(int sockfd, char* key) : sock(sockfd), pubkey(key) {}
};

class ClientMap {
    private:
        std::unordered_map<std::string, ClientInfo> clients;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    public:
        ClientMap();
        void set_info(std::string username, ClientInfo info);
        ClientInfo get(std::string username);
        std::vector<std::string> list_clients();
        int remove_user(std::string username);
};

struct ThreadArgs {
    int sock;
    ClientMap *client_map;
};



/* server set up */
int socket_bind_listen(int port);
int accept_connection(int sockfd);
void *client_handler(void *socket_desc);

void* connection_handler(void *args);
bool handle_login(int sockfd, ClientMap* client_map);
bool handle_commands(int fd, ClientMap* cm);
void broadcast_msg(int sockfd, ClientMap* cm);

#endif
