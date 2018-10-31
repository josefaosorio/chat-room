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
    std::cout << buf << std::endl;

    // Get password from user
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    // Send password to server
    if (send_string(sockfd, password) < 0) {
        std::cerr << "Client fails to send password to server" << std::endl;
        return false;
    }

    // Receives server's acknowledgement
    buf.clear();
    if (recv_string(sockfd, buf) < 0) {
        std::cerr << "Client fails to receive password ACK" << std::endl;
        return false;
    }

    // Checks if password input is correct or not
    if (buf.compare("Authentication succeeded")) {
        return false;
    }

    std::cout << buf << std::endl;

    pubkey = getPubKey();

    // Sends public key to server
    if (send_string(sockfd, std::string(pubkey)) < 0) {
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

    // Sends P command to server
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

    // Get message from user
    std::cout << "Enter the public message: ";
    std::getline(std::cin, msg_to_send);

    // Send message to server
    if (send_string(sockfd, msg_to_send) < 0) {
        std::cerr << "Client fails to send message to server" << std::endl;
        return;
    }

    // pop acknowledgement from Queue
    ack_msg = messages->pop();
    if (ack_msg.compare("1") != 0) {
        std::cerr << "Acknowledgement not received" << std::endl;
        return;
    }
    ack_msg.clear();
}

void direct_message(int sockfd, Queue<std::string> *messages) {
    std::string user_list_csv;
    std::vector<std::string> user_list;
    std::string peer;
    char *pub_key;
    std::string user_msg;
    char *c_user_msg;
    char *encrypted_user_msg;
    std::string msg_ack;

    // Sends D command to server
    if (send_string(sockfd, "D") < 0) {
        std::cerr << "Client fails to send command to server" << std::endl;
        return;
    }

    // Parse comma separated user list
    user_list_csv = messages->pop();
    std::stringstream ss(user_list_csv);
    std::string username;

    while (getline(ss, username, ',')) {
        user_list.push_back(username);
    }

    // Display online users
    std::cout << "Peers online:" << std::endl;
    for (auto it: user_list) {
        std::cout << "\t" << it << std::endl;
    }

    // Ask for target user
    std::cout << "\nPeer to message: ";
    getline(std::cin, peer);

    // Send target user to server
    if (send_string(sockfd, peer) < 0) {
        std::cerr << "Client fails to send command to server" << std::endl;
        return;
    }

    // Get public key of target user
    std::string temp = messages->pop();
    pub_key = (char*)malloc(temp.size() + 1);
    memcpy(pub_key, temp.c_str(), temp.size());
    pub_key[temp.size()] = '\0';

    // Get message from user
    std::cout << "\nMessage: ";
    getline(std::cin, user_msg);

    // Encrypt message from user
    c_user_msg = (char*)malloc(user_msg.size() + 1);
    memcpy(c_user_msg, user_msg.c_str(), user_msg.size());
    c_user_msg[user_msg.size()] = '\0';

    encrypted_user_msg = encrypt(c_user_msg, pub_key);
    std::string encrypted_user_msg_str(encrypted_user_msg);

    // Send encrypted message to server
    if (send_string(sockfd, encrypted_user_msg_str) < 0) {
        std::cerr << "Client fails to send encrypted message" << std::endl;
        return;
    }

    // Get acknowledgement and check values
    msg_ack = messages->pop();
    if (msg_ack.compare("0") == 0) {
        std::cout << "Sorry, this user is no longer online" << std::endl;
        return;
    }
    else if (msg_ack.compare("1") != 0) {
        std::cerr << "Acknowledgement not received" << std::endl;
        return;
    }
    msg_ack.clear();
    free(pub_key);
    free(c_user_msg);
}

void quit(int sockfd){
    // Sends Q command to server
    if(send_string(sockfd, std::string("Q")) < 0){
        std::cerr << "Client fails to send command to server" << std::endl;
        return;
    }

    std::cout << "Bye!" << std::endl;
}

void display_broadcast(std::string msg) {
    std::cout << std::endl;
    std::cout << "*** Incoming public message ***: " << msg << std::endl;
    std::cout << "> ";
    std::cout.flush();
}

void display_direct(std::string sender, std::string msg) {
    // Decrypts user message and displays to target user
    char* decrypted_msg = decrypt((char*)msg.c_str());
    std::string cpp_msg = std::string(decrypted_msg);
    free(decrypted_msg);
    std::cout << "\n*** Incoming message from " << sender << " ***: " << cpp_msg << std::endl;
    std::cout << "> ";
    std::cout.flush();
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

        // Receive message
        if (recv_string(sockfd, msg) < 0) {
            fprintf(stderr, "Failed to receive message");
            continue;
        }

        // Handles acknowledgements and anything that isnt a P or D command
        if (!type.compare("C")){
            msg_queue->push(msg);
        }
        // Handles public messages
        else if (!type.compare("P")){
            display_broadcast(msg);
        }
        // Handles direct messages
        else if (!type.compare("D")){
            display_direct(sender, msg);
        }
    }
}
