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

std::tuple<Operation, std::string> parse_input(std::string& args){
  std::string input;
  std::string message;
  bool flag = false;
  std::cout << "Please enter a command: P (Public Message), D (Direct Messaging), Q (Quit)\n";
  std::cout << "> ";
  Operation op;

  while(!flag){
    getline(std::cin, input);
    message = std::string();
    if(!input.compare("P")){
      op = P;
      std::cout << "Enter the public message: ";
      getline(std::cin, message);
      flag = true;
    }
    else if(!input.compare("D")){
      op = D;
      flag = true;
    }
    else if(!input.compare("Q")){
      op = Q;
      flag = true;
    }
    else {
      op = UNKNOWN;
      flag = false;
    }
  }

  return std::make_tuple(op, message);
}

int main(int argc, char* argv[]) {
    int sockfd;
    int port;
    char *host;
    char *username;
    bool running = true;
    std::string input;
    Operation op;
    bool logged_in = false;
    std::string message;

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

    user_login(sockfd, std::string(username));
    while (running) {
            std::tie(op, message) = parse_input(message);
            switch(op){
              case P:
                public_message(sockfd, message);
                break;
              case D:
                break;
              case Q:
                quit(sockfd);
                running = false;
                break;
              case UNKNOWN:
                break;
            }
    }
    close(sockfd);

}

/*
main thread - stuff inside the while running
second thread - listening from the server
  - any public messages or direct messages that people send you
*/
