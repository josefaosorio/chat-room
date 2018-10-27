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

int socket_connect(char *host, int port);

void user_login(int sockfd, char *username);