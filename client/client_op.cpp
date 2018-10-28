/*
 * File: client_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

 #include "chatclient.h"

bool user_login(int sockfd, std::string username) {
    char* pubkey;
    std::string buf;
    std::string password;

    // Send username
    if (send_string(sockfd, username) < 0) {
        std::cerr << "Client fails to send username to server" << std::endl;
        return false;
    }

    if (recv_string(sockfd, buf) < 0) {
        std::cerr << "Client fails to receive username ACK" << std::endl;
        return false;
    }

    // Get password from user and send to server
    std::cout << buf << std::endl;

    std::cout << "Enter password: ";
    std::cin >> password;

    if (send_string(sockfd, password) < 0) {
        std::cerr << "Client fails to send password to server" << std::endl;
        return false;
    }

    if (recv_string(sockfd, buf) < 0) {
        std::cerr << "Client fails to receive password ACK" << std::endl;
        return false;
    }

    if (buf.compare("Authentication succeeded")) {
        std::cout << buf << std::endl;
        return false;
    }

    std::cout << buf << std::endl;
    
    pubkey = getPubKey();

    if (send_pubkey(sockfd, pubkey) < 0) {
        std::cerr << "Client fails to send pubkey to server" << std::endl;
        return false;
    }    
}
