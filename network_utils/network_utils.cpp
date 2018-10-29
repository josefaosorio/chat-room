/*
 * File: network_utils.cpp
 * Name: Herman Tong, Jessica Hardey, Josefa Osorio
 * Netid: ktong1, jhardey, josorio2
 */

#include "network_utils.h"

int send_string(int sockfd, std::string msg) {
    int ret;
    size_t len;

    // send length of msg
    len = msg.length();
    if ((ret = send(sockfd, &len, sizeof(size_t), 0)) < 0) {
        perror("ERROR sending string length");
        return ret;
    }

    // send msg
    if ((ret = send(sockfd, msg.c_str(), len, 0)) < 0) {
        perror ("ERROR sending string message");
        return ret;
    }

    return ret;
}

int recv_string(int sockfd, std::string &msg) {
    int ret;
    size_t len;
    size_t received = 0;
    char buffer[BUFSIZ];

    // recv length of msg
    memset(buffer, 0, BUFSIZ);
    if ((ret = recv(sockfd, buffer, sizeof(size_t), 0)) < 0) {
        perror("ERROR recving string length");
        return ret;
    }
    memcpy(&len, buffer, sizeof(size_t));

    while (received < len) {
        memset(buffer, 0, BUFSIZ);
        if ((ret = recv(sockfd, buffer, len, 0)) < 0) {
            perror("ERROR receiving string");
            return ret;
        }
        received += ret;
        msg.append(std::string(buffer));
    }

    return ret;
}

int send_pubkey(int sockfd, char* pubkey) {
    int ret;
    size_t len;

    len = strlen(pubkey);
    if ((ret = send(sockfd, &len, sizeof(len), 0)) < 0) {
        perror("ERROR sending key length");
        return ret;
    }

    // send msg
    if ((ret = send(sockfd, pubkey, len, 0)) < 0) {
        perror("ERROR sending pubkey");
        return ret;
    }

    return ret;
}

char* recv_pubkey(int sockfd) {
    int ret;
    size_t len;
    size_t received = 0;
    char buffer[BUFSIZ];
    char* pubkey;

    memset(buffer, 0, BUFSIZ);
    if ((ret = recv(sockfd, buffer, sizeof(size_t), 0)) < 0) {
        perror("ERROR receiving key length");
        return NULL;
    }
    memcpy(&len, buffer, sizeof(size_t));

    if ((ret = recv(sockfd, buffer, len, 0)) < 0) {
        perror("ERROR receiving pub key");
        return NULL;
    }

    pubkey = (char*)malloc(len);
    memcpy(pubkey, buffer, len);

    return pubkey;
}
