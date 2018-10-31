/*
 * File: server_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

#include "chatserver.h"
#include "../network_utils/pg3lib.h"

void* connection_handler(void *args) {
    bool flag = true;

    // Get arguments from pointer
    ThreadArgs *thread_args = (ThreadArgs*)args;
    int sock = thread_args->sock;
    ClientMap *client_map = thread_args->client_map;
    free(args);

    // Authenticate user, kills connection if fails
    if (!handle_login(sock, client_map))
        pthread_exit(NULL);

    while (flag) {
        flag = handle_commands(sock, client_map);
    }

    return NULL;
}

bool send_msg(int sockfd, std::string type, std::string sender, std::string msg) {
    // Send message type to client
    if (send_string(sockfd, type) < 0) {
        fprintf(stderr, "Failed to send msg type");
        return false;
    }

    // Send target user to client
    if (send_string(sockfd, sender) < 0) {
        fprintf(stderr, "Failed to send sender");
        return false;
    }

    // Send message to client
    if (send_string(sockfd, msg) < 0) {
        fprintf(stderr, "Failed to send msg");
        return false;
    }

    return true;
}

void broadcast_msg(int sockfd, ClientMap* cm) {
    std::string msg;
    std::string username;
    ClientInfo info;

    // Sending command
    if (!send_msg(sockfd, std::string("C"), std::string("dummy"), std::string("1")))
        return;

    // Receiving message from client
    if (recv_string(sockfd, msg) < 0) {
        fprintf(stderr, "ERROR in recving message to broadcast");
        return;
    }

    // Getting the client info, checking the socket, and sending the P message
    for (auto it : cm->list_clients()) {
        username = it;
        info = cm->get(username);

        if (info.sock == sockfd)
            continue;

        if (!send_msg(info.sock, std::string("P"), std::string("dummy"), msg))
            return;
    }

    if (!send_msg(sockfd, std::string("C"), std::string("dummy"), std::string("1")))
        return;
}

void direct_msg(int sockfd, ClientMap* cm) {
    std::string msg;
    std::string sender;
    std::string user;
    std::string pubkey;
    std::string user_list = std::string();
    ClientInfo info;
    ClientInfo senderInfo;



    // Generate comma separated list of users and send to client
    for (auto it : cm->list_clients()) {
        if ((cm->get(it)).sock != sockfd){
          user_list += it + ",";
        }

    }
    user_list.erase(user_list.size()-1);

    // Send command to client
    if (send_msg(sockfd, std::string("C"), std::string("dummy"), user_list) < 0) {
        fprintf(stderr, "ERROR in sending user list to client");
        return;
    }

    // Get targeted user from client
    if (recv_string(sockfd, user) < 0) {
        fprintf(stderr, "ERROR in recving target user from client");
        return;
    }

    // Send public key back to client
    info = cm->get(user);
    if (send_msg(sockfd, std::string("C"), std::string("dummy"), info.pubkey) < 0) {
        fprintf(stderr, "Failed to send pubkey to client");
        return;
    }

    // Get message from client
    if (recv_string(sockfd, msg) < 0) {
        fprintf(stderr, "Failed to receive message from client");
        return;
    }

    // Check if client is still online, if so send the message
    if (info.empty) {
        if (send_msg(sockfd, std::string("C"), std::string("dummy"), std::string("0")) < 0) {
            fprintf(stderr, "Failed to send fail ACK to client");
        }
        return;
    }

    // Find sender's username
    for (auto it : cm->list_clients()) {
        ClientInfo ci = cm->get(it);
        if (ci.sock == sockfd) {
            sender = it;
            break;
        }
    }

    // Send message to target client
    if (send_msg(info.sock, std::string("D"), sender, msg) < 0) {
        fprintf(stderr, "Failed to send message to target client");
        return;
    }

    // send ACK that message was sent
    if (send_msg(sockfd, std::string("C"), std::string("dummy"), std::string("1")) < 0) {
        fprintf(stderr, "Failed to send fail ACK to client");
    }
}

bool handle_commands(int fd, ClientMap* client_map){
    std::string op;
    bool running = true;

    while(running) {
        op = std::string();

        // get socket fd
        if (recv_string(fd, op) < 0){
            std::cerr << "Error receiving operation" << std::endl;
            return false;
        }
        // Get command and process accordingly
        if (!op.compare("P")){
            broadcast_msg(fd, client_map);
            return true;
        }
        else if(!op.compare("D")){
            direct_msg(fd, client_map);
            return true;
        }
        else if(!op.compare("Q")){
            close(fd);

            // Remove user from list of online users
            for (auto a : client_map->list_clients()) {
                ClientInfo i = client_map->get(a);
                if (i.sock == fd){
                    if (client_map->remove_user(a) < 0){
                        std::cerr << "Error removing user" << std::endl;
                    }
                }
            }
            return false;
        }
    }
}

/* Returns true if successfully authenticate user,
 * false otherwise
 */
bool handle_login(int sockfd, ClientMap* client_map) {
    bool user_exist = false;
    bool auth_success;
    std::fstream user_creds;
    std::string client_username;
    std::string username;
    std::string client_password;
    std::string password;
    std::string reply;
    std::string pubkey_string;

    // Get username
    if (recv_string(sockfd, client_username) < 0) {
        std::cerr << "Server fails to receive username" << std::endl;
        return false;
    }

    // Check if username exist
    user_creds.open("user_creds.txt", std::ios::app|std::ios::in|std::ios::out);

    while (user_creds >> username >> password) {
        if (username.compare(client_username) == 0) {
            user_exist = true;
            break;
        }
    }

    // Inform user if username exist
    if (user_exist)
        reply = std::string("Welcome back, please enter password");
    else
        reply = std::string("Creating new user");

    if (send_string(sockfd, reply) < 0) {
        std::cerr << "Server fails to reply client" << std::endl;
        user_creds.close();
        return false;
    }

    // Get password from user
    if (recv_string(sockfd, client_password) < 0) {
        std::cerr << "Server fails to receive password" << std::endl;
        user_creds.close();
        return false;
    }

    // Append crediential or verify password
    if (!user_exist) {
        user_creds.seekp(std::ios_base::end);
        user_creds.clear();
        user_creds << client_username << " " << client_password << std::endl;
        auth_success = true;
    } else {
        if (!password.compare(client_password)) {
            auth_success = true;
        } else {
            auth_success = false;
        }

    }

    // check for authentication success
    if (!auth_success) {
        send_string(sockfd, "Wrong password, authentication failed");
        user_creds.close();
        return false;
    }

    // send authentication to client
    if (send_string(sockfd, "Authentication succeeded") < 0) {
        std::cerr << "Server failed to send ack" << std::endl;
        user_creds.close();
        return false;
    }

    // Get public key from client
    if (recv_string(sockfd, pubkey_string) < 0) {
        fprintf(stderr, "Failed to receive pubkey string");
        user_creds.close();
        return false;
    }

    // set client's info in the client map
    ClientInfo info { sockfd, pubkey_string };
    client_map->set_info(client_username, info);

    user_creds.close();
    return auth_success;
}
