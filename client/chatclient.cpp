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

    printf("Connecting to %s on port %i\n", host, port);
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("connection error\n");
        close(s);
        return -1;
    }
    printf("Connection established\n");

    return s;
}

Operation parse_input(){
    std::string input;
    std::cout << "Please enter a command: P (Public Message), D (Direct Messaging), Q (Quit)\n";
    std::cout << "> ";
    Operation op;

    std::getline(std::cin, input);
    if(!input.compare("P"))
        op = P;
    else if(!input.compare("D"))
        op = D;
    else if(!input.compare("Q"))
        op = Q;
    else
        op = U; //Unknown

    return op;
}

int main(int argc, char* argv[])
{
    int sockfd;
    int port;
    char *host;
    char *username;
    bool running = true;
    pthread_t t_id;
    std::string input;
    Operation op;
    bool logged_in = false;
    Queue<std::string> *messages;

    if (argc != 4)
    {
        fprintf(stderr, "usage: %s server_name port username\n", argv[0]);
        exit(1);
    }
    host = argv[1];
    port = std::stoi(argv[2]);
    username = argv[3];

    if ((sockfd = socket_connect(host, port)) < 0) {
        exit(1);
    }

    // Checck if log in was successfully
    if (!user_login(sockfd, std::string(username))) {
        std::cout << "Log in failed" << std::endl;
        close(sockfd);
        exit(1);
    }

    // Creates receiving thread
    messages = new Queue<std::string>();
    ThreadArgs *args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
    args->sockfd = sockfd;
    args->msg_queue = messages;
    if (pthread_create(&t_id, NULL, message_recv_thread, (void*)args) < 0) {
        perror("Failed to create message recv thread");
        exit(1);
    }

    // Parse input and carry out operation
    while (running) {
        op = parse_input();
        switch(op){
            case P:
                public_message(sockfd, messages);
                break;
            case D:
                direct_message(sockfd, messages);
                break;
            case Q:
                quit(sockfd);
                running = false;
                break;
            case U:
                std::cout << "Invalid input, please try again" << std::endl;
                break;
        }
    }
    close(sockfd);

}
