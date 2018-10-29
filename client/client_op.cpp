/*
 * File: client_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

 #include "chatclient.h"
 #include "../network_utils/pg3lib.h"

bool user_login(int sockfd, std::string username) {
    char* pubkey;
    std::string buf;
    std::string password;

    // Send username
    if (send_string(sockfd, username) < 0) {
        std::cerr << "Client fails to send username to server" << std::endl;
        return false;
    }

    buf.clear();
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

    buf.clear();
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

void public_message(int sockfd, std::string message){
  if (send_string(sockfd, "P") < 0) {
    std::cerr << "Client fails to send command to server" << std::endl;
    return;
  }

  if(send_string(sockfd, message) < 0){
    std::cerr << "Client fails to send public message to server" << std::endl;
    return;
  }

  //how to tell if a user is still connected - if you try to receive something from
  //a socket that has been closed, it'll return a 0-length message so you can check
  //for that


}

void quit(int sockfd){
  if(send_string(sockfd, std::string("Q")) < 0){
    std::cerr << "Client fails to send command to server" << std::endl;
    return;
  }
}
