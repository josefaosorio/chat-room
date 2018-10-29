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

    while (flag) {
        // Authenticate user, skip if fails
        if (!handle_login(sock, client_map))
            continue;
        /*
        for (auto a : client_map->list_clients()) {
            std::cout << "username: " << a << std::endl;
            ClientInfo i = client_map->get(a);
            std::cout << "sock: " << i.sock << std::endl;
            std::cout << "key: " << i.pubkey << std::endl;
            std::cout << "---" << std::endl;
        }
        */

        flag = handle_commands(sock, client_map);
        return NULL;
    }

    return NULL;
}

bool handle_commands(int fd, ClientMap* client_map){
  std::string op;
  std::string message = std::string();
  bool running = true;
  while(running) {
    op = std::string();

    // Receives operation from client
    if (recv_string(fd, op) < 0){
      std::cerr << "Error receiving operation" << std::endl;
      return false;
    }

    if (!op.compare("P")){
      if (recv_string(fd, message) < 0){
        std::cerr << "Error receiving public message" << std::endl;
        return false;
      }
      return true;
    }
    else if(!op.compare("D")){
      //something
      return true;
    }
    else if(!op.compare("Q")){
      close(fd);

      for (auto a : client_map->list_clients()) {
          //std::cout << "username: " << a << std::endl;
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

/* Returns true of successfully authenticate user,
 * false otherwise
 */
bool handle_login(int sockfd, ClientMap* client_map) {
    bool user_exist = false;
    bool auth_success;
    char* pubkey;
    std::fstream user_creds;
    std::string client_username;
    std::string username;
    std::string client_password;
    std::string password;
    std::string reply;

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

    if (!auth_success) {
        send_string(sockfd, "Wrong password, authentication failed");
        user_creds.close();
        return false;
    }

    if (send_string(sockfd, "Authentication succeeded") < 0) {
        std::cerr << "Server failed to send ack" << std::endl;
        user_creds.close();
        return false;
    }

    pubkey = recv_pubkey(sockfd);

    if (pubkey == NULL) {
        std::cerr << "Server failed to receive pub key" << std::endl;
        user_creds.close();
        return false;
    }

    ClientInfo info { sockfd, pubkey };
    client_map->set_info(client_username, info);

    user_creds.close();
    return auth_success;
}
