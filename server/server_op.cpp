/*
 * File: server_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

#include "chatserver.h"

void* connection_handler(void *sockfd) {
    int sock = *(int*)sockfd;
    free(sockfd);
    std::string buf;
    while (1) {
        recv_string(sock, buf);
        std::cout << buf << std::endl;
    }

    return NULL;
}

void handle_login(void *sockfd) {
    std::string username;
    std::fstream user_creds;
    std::string user;
    std::string password;
    std::string pass;
    // get username from client
    if (recv_string(*(int*)sockfd, username) < 0) {
        std::cerr << "Server fails to receive username" << std::endl;
        return;
    }

    std::cout << username << std::endl;
    
    if (recv_string(*(int*)sockfd, password) < 0) {
        std::cerr << "Server fails to receive password" << std::endl;
        return;
    }

    std::cout << password << std::endl;

    // check if username already exists, if not add it
    user_creds.open("user_creds.txt", std::fstream::app|std::ios::in);

    while (user_creds >> user >> pass) {
        std::cout << user << pass << std::endl;
        if (user.compare(username) == 0 && pass.compare(password) == 0) {
            // ask user for password
            std::string success_msg = "Client successfully authenticated\n";
            if (send_string(*(int*)sockfd, success_msg) < 0) {
                std::cerr << "Server fails to send authentication message" << std::endl;
                return;
            }
            break;
        }

    }
    if (user.empty() || pass.empty()) {
        user_creds << user << pass;
    }

    // std::cout << "after while in handle login" << std::endl;
    // user_creds << username;
    user_creds.close();
}
