#include "common_net.h"
#include <stdexcept>

#include "common_socket.h"

using namespace std;

TCPSocket::TCPSocket(const char* host, unsigned short port) {
    int errcode = socket_create(&(this->socket));
    if (errcode < 0) throw runtime_error("Failed to create port "
            "socket_create failed with code " + to_string(errcode));

    errcode = socket_connect(&(this->socket), host, port);
    if (errcode < 0) {
        socket_destroy(&(this->socket));
        throw runtime_error("Could not connect to port " +
                to_string(port) + " on host " + host + " "
                "socket_connect failed with code " + to_string(errcode));
    }
}

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
