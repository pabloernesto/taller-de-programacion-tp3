#include "common_net.h"
#include <stdexcept>

#include "common_socket.h"

using namespace std;

TCPSocket::TCPSocket(socket_t socket) {
    this->socket = socket;
}

TCPSocket::~TCPSocket() {
    socket_destroy(&(this->socket));
}

int TCPSocket::send(const char* buffer, int lenght) {
    return socket_send(&(this->socket), buffer, lenght);
}

int TCPSocket::receive(char* buffer, int lenght) {
    return socket_receive(&(this->socket), buffer, lenght);
}

TCPSocket& operator<<(TCPSocket& os, const char val) {
    os.send((char*)&val, sizeof(val));
    return os;
}

TCPSocket& operator<<(TCPSocket& os, const int val) {
    os.send((char*)&val, sizeof(val));
    return os;
}

TCPSocket& operator<<(TCPSocket& os, const unsigned int val) {
    os.send((char*)&val, sizeof(val));
    return os;
}

TCPSocket& operator>>(TCPSocket& os, char& val) {
    os.receive(&val, sizeof(char));
    return os;
}

TCPSocket& operator>>(TCPSocket& os, int& val) {
    os.receive((char*)&val, sizeof(int));
    return os;
}

TCPSocket& operator>>(TCPSocket& os, unsigned int& val) {
    os.receive((char*)&val, sizeof(unsigned int));
    return os;
}
