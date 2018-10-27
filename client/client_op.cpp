/*
 * File: client_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

 #include "chatclient.h"

 void user_login(int sockfd, char *username) {
     std::string password;

     if (send_string(sockfd, std::string(username)) < 0) {
         std::cerr << "Client fails to send username to server" << std::endl;
         return;
     }

     std::cout << "Enter password: ";
     getline(std::cin, password);

     if (send_string(sockfd, password) < 0) {
         std::cerr << "Client fails to send password to server" << std::endl;
         return;
     }
 }
