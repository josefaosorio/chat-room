Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
NetId: ktong1, jhardey, josorio2

Filename: README.md

Purpose: Client and server sides of an online chat room application using TCP in our implementation.

Server Files:
- chatserver.cpp
- chatserver.h
- ClientMap.cpp
- Makefile (for server)
- server_op.cpp

Client Files:
- chatclient.cpp
- chatclient.h
- client_op.cpp
- macros.h
- Makefile
- queue.h

Network Utilities Files:
- network_utils.cpp
- network_utils.h
- pg3lib.h

In order to build the entire solution, use $ ./make_all.sh from the chat-room directory inside program3.
If you would like to build the server and client separately, just move into the chosen directory and call $ make.

To run the server:
   $ ./chatserver [PORT] (i.e. $ ./chatserver 41020)

To run a client:
   $ ./chatclient [HOST] [PORT] [USERNAME] (i.e. $ ./chatclient student00.cse.nd.edu 41020 Jess)
