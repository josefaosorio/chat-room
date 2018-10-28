/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
 * NetId: ktong1, jhardey, josorio2
 *
 * ClientMap.cpp
 */

#include "chatserver.h"

ClientMap::ClientMap() {
    this->clients = std::unordered_map<std::string, ClientInfo>();
}

void ClientMap::set_info(std::string username, ClientInfo info) {
    pthread_mutex_lock(&mutex);
    clients[username] = info;
    pthread_mutex_unlock(&mutex);
}

ClientInfo ClientMap::get(std::string username) {
    pthread_mutex_lock(&mutex);
    std::unordered_map<std::string, ClientInfo>::const_iterator got = clients.find(username);
    if (got == clients.end()) {
        pthread_mutex_unlock(&mutex);
        ClientInfo null_struct;
        null_struct.empty = true;
        return null_struct;
    }
    ClientInfo info = got->second;
    pthread_mutex_unlock(&mutex);
    return info;
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
