/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
 * NetId: ktong1, jhardey, josorio2
 *
 * ClientMap.cpp
 */

#include "chatserver.h"

ClientMap::ClientMap() {
    this->clients = std::unordered_map<std::string, int>();
}

void ClientMap::set(std::string username, int sockfd) {
    pthread_mutex_lock(&mutex);
    clients[username] = sockfd;
    pthread_mutex_unlock(&mutex);
}

int ClientMap::get(std::string username) {
    pthread_mutex_lock(&mutex);
    std::unordered_map<std::string, int>::const_iterator got = clients.find(username);
    if (got == clients.end()) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    int sockfd = got->second;
    pthread_mutex_unlock(&mutex);
    return sockfd;
}

std::vector<std::string> ClientMap::list_clients() {
    pthread_mutex_lock(&mutex);
    std::vector<std::string> list;
    for (auto it = clients.begin(); it != clients.end(); it++) {
        list.push_back(it->first);
    }

    pthread_mutex_unlock(&mutex);
    return list;
}
