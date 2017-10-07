#include "common_socket.h"

#ifndef NET_H
#define NET_H

class TCPSocket {
    socket_t socket;
public:
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

TCPSocket& operator<<(TCPSocket& os, const char val);
TCPSocket& operator<<(TCPSocket& os, const int val);
TCPSocket& operator<<(TCPSocket& os, const unsigned int val);

TCPSocket& operator>>(TCPSocket& os, char &val);
TCPSocket& operator>>(TCPSocket& os, int &val);
TCPSocket& operator>>(TCPSocket& os, unsigned int &val);
#endif
