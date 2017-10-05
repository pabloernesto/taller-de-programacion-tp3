#include "common_net.h"
#include <stdexcept>

extern "C" {
#include "socket.h"
}

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
