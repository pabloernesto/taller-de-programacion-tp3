#include "common_socket.h"

#ifndef NET_H
#define NET_H

class TCPSocket {
    socket_t socket;
    public:
    TCPSocket(const char* host, unsigned short port);
    TCPSocket(socket_t socket);
    ~TCPSocket();
    int send(const char* buffer, int lenght);
    int receive(char* buffer, int lenght);
};

class TCPAcceptor {
    socket_t socket;
    public:
    TCPAcceptor(unsigned short port);
    ~TCPAcceptor();
    TCPSocket accept();
};

#endif
