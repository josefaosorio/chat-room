/*
 * File: network_utils.h
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 */

#include <string>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*
 * Send and receive any std::string.
 * It first sends the length of string then the actualy
 * msg.
 * recv_string function will store the string in msg that
 * is passed by reference
 */
int send_string(int sockfd, std::string msg);
int recv_string(int sockfd, std::string &msg);

/*
 * Send and receive char* pubkey
 * recv_pubkey returns a malloc'd pointer
 */
int send_pubkey(int sockfd, char* pubkey);
char* recv_pubkey(int sockfd);
