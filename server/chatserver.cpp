/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
 * NetId: ktong1, jhardey, josorio2
 *
 * chatserver.cpp
*/

#include "chatserver.h"

int socket_bind_listen(int port) {
    int sockfd;
    int opt;
    struct sockaddr_in sin;

    /* Create socket */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR opening socket");
        return -1;
    }

    /* Call setsockopt for port reuse */
    opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int));

    /* Set server addr struct */
    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons((unsigned short)port);

    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr*)&sin, sizeof(sin))) < 0) {
        perror("ERROR binding socket");
        return -1;
    }

    /* Listen on socket */
    if ((listen(sockfd, BACKLOG)) < 0) {
        perror("ERROR listening");
        return -1;
    }

    return sockfd;

}

// this will neet to handle the multithreading - TODO
int accept_connection(int sockfd) {
    int newfd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    sin_size = sizeof(their_addr);
    pthread_t thread_id;
    while (newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) {

        printf("Received connection\n");
        if (pthread_create(&thread_id, NULL, client_handler, (void*)&newfd) < 0) {
            perror("Could not create thread");
            return -1;
        }

        // do we need pthread_join here? The sample code has it commented out...
        //pthread_join(thread_id, NULL);
        printf("Handler assigned to client\n");
    }

    if (newfd == -1) {
        perror("ERROR in accepting");
        return -1;
    }

    return 0;
}

void *client_handler(void *socket_desc) {
    printf("in client_handler\n");
    //Get the socket descriptor
    int socket = *(int*)socket_desc;

    handle_login(socket_desc);

    // add it to data structure of current users
}

int main(int argc, char** argv) {
    int port;
    int sockfd, newfd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    if (argc != 2) {
        fprintf(stderr, "usage: %s port\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    if ((sockfd = socket_bind_listen(port)) < 0)
        exit(1);

    while (1) {
        printf("Waiting for connection...\n");
        if ((newfd = accept_connection(sockfd)) < 0)
            continue;
        // TODO:handle commands here
        close(newfd);
    }

    close(sockfd);
}
