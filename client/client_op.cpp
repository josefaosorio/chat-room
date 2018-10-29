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
        std::cerr << "Password ACK says failed" << std::endl;
        return false;
    }

    std::cout << buf << std::endl;

    pubkey = getPubKey();

    if (send_pubkey(sockfd, pubkey) < 0) {
        std::cerr << "Client fails to send pubkey to server" << std::endl;
        return false;
    }

    free(pubkey);
    std::cout << "Registration succeeded" << std::endl;

    return true;
}

void public_message(int sockfd, Queue<std::string> *messages){
    std::string ack_msg;
    std::string msg_to_send;

    if (send_string(sockfd, "P") < 0) {
        std::cerr << "Client fails to send command to server" << std::endl;
        return;
    }

    // pop acknowledgement from Queue
    ack_msg = messages->pop();
    if (ack_msg.compare("1")) {
        std::cerr << "Acknowledgement not received" << std::endl;
        return;
    }
    ack_msg.clear();

    std::cout << "Enter the public message: ";
    std::getline(std::cin, msg_to_send);
    msg_to_send.erase(msg_to_send.rfind('\n'), 1);

    if (send_string(sockfd, msg_to_send) < 0) {
        std::cerr << "Client fails to send message to server" << std::endl;
        return;
    }

    ack_msg = messages->pop();
    if (ack_msg.compare("1") != 0) {
        std::cerr << "Acknowledgement not received" << std::endl;
        return;
    }
    ack_msg.clear();

    //how to tell if a user is still connected - if you try to receive something from
    //a socket that has been closed, it'll return a 0-length message so you can check
    //for that
}

void direct_message(int sockfd, Queue<std::string> *messages) {
    std::string user_list_csv;

    if (send_string(sockfd, "D") < 0) {
        std::cerr << "Client fails to send command to server" << std::endl;
        return;
    }

    user_list_csv = messages->pop();



}

void quit(int sockfd){
    if(send_string(sockfd, std::string("Q")) < 0){
        std::cerr << "Client fails to send command to server" << std::endl;
        return;
    }
}
