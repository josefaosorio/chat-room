/*
File: chatclient.cpp
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1, jhardey
*/

#include "chatclient.h"

int socket_connect(char *host, int port) {
    int s;
    struct hostent *hp;
    struct sockaddr_in sin;
    hp = gethostbyname(host);
    if (!hp)
    {
        fprintf(stderr, "unknown host: %s\n", host);
        return -1;
    }

    // Build address data structure
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(port);

    // active open
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error\n");
        return -1;
    }

    printf("Connecting to %s\n", host);
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("connection error\n");
        close(s);
        return -1;
    }
    printf("Connection established\n");

    return s;
}

int main(int argc, char* argv[])
{
    int sockfd;
    int port;
    char *host;
    char *username;
    bool running = true;
    std::string input;

    if (argc != 4)
    {
        std::cerr << "usage: wrong number of arguments" << std::endl;
        exit(1);
    }
    host = argv[1];
    port = std::stoi(argv[2]);
    username = argv[3];

    if ((sockfd = socket_connect(host, port)) < 0) {
        exit(1);
    }

    while (running) {
        std::cin >> input;
        send_string(sockfd, input);
    }
    close(sockfd);
}
