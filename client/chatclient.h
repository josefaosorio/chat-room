/*
File: chatclient.h
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1, jhardey
*/

#include "../network_utils/network_utils.h"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <typeinfo>
#include <sys/time.h>

enum Operation {
  P,
  D,
  Q
};

int socket_connect(char *host, int port);
Operation parse_input(std::string argument);

bool user_login(int sockfd, std::string username);
void public_message(int sockfd);
void quit(int sockfd);
