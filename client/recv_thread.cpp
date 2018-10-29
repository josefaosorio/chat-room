/*
 * File: recv_thread.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

#include "chatclient.h"

void display_broadcast(std::string msg) {
    std::cout << std::endl;
    std::cout << "*** Incoming public message ***: " << msg << std::endl;
    std::cout << ">";
}

void display_direct(std::string sender, std::string msg) {
    std::cout << std::endl;
    std::cout << "*** Incoming message from " << sender << " ***: " << msg << std::endl;
    std::cout << ">";
}

void *message_recv_thread(void* args) {
    ThreadArgs *arguments = (ThreadArgs*)args;
    int sockfd = arguments->sockfd;
    Queue<std::string> *msg_queue = arguments->msg_queue;
    free(args);

    std::string type;
    std::string sender;
    std::string msg;

    while (true) {
        type.clear();
        sender.clear();
        msg.clear();

        // Receive message type
        if (recv_string(sockfd, type) < 0) {
            fprintf(stderr, "Failed to receive message type");
            continue;
        }

        // Receive sender
        if (recv_string(sockfd, sender) < 0) {
            fprintf(stderr, "Failed to receive sender");
            continue;
        }

        // Receive msg
        if (recv_string(sockfd, msg) < 0) {
            fprintf(stderr, "Failed to receive message");
            continue;
        }

        std::cout << "type: " << type << std::endl;
        std::cout << "sender: " << sender << std::endl;
        std::cout << "msg: " << msg << std::endl;

        if (!type.compare("C"))
            msg_queue->push(msg);
        else if (!type.compare("P"))
            display_broadcast(msg);
        else if (!type.compare("D"))
            display_direct(sender, msg);
    }
}
