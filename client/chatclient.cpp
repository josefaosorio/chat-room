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

Operation parse_input(){
  std::string input;
  bool flag = false;
  std::cout << "Please enter a command: P (Public Message), D (Direct Messaging), Q (Quit)\n";
  std::cout << "> ";
  Operation op;

  while(!flag){
    getline(std::cin, input);
    //std::cout << "input: " << input << std::endl;
    if(!input.compare("P")){
      op = P;
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
      flag = false;
    }
  }

  return op;
}

int main(int argc, char* argv[])
{
    int sockfd;
    int port;
    char *host;
    char *username;
    bool running = true;
    std::string input;
    Operation op;
    bool logged_in = false;

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
            op = parse_input();
            switch(op){
              case P:
                public_message(sockfd);
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
            //send_string(sockfd, buf);

    }
    close(sockfd);

}

/*
main thread - stuff isnide the while running
second thread - listening from the server
  - any public messages or direct messages that people send you
