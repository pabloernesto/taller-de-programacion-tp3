#include "common_net.h"
#include <stdexcept>

#include "common_socket.h"

using namespace std;

TCPAcceptor::TCPAcceptor(unsigned short port) {
    int errcode = socket_create(&(this->socket));
    if (errcode < 0) throw runtime_error("Failed to create socket listening "
            "on port " + to_string(port) + " "
            "socket_create failed with code " + to_string(errcode));

    errcode = socket_bind_and_listen(&(this->socket), port);
    if (errcode < 0) {
        socket_destroy(&(this->socket));
        throw runtime_error("Failed to create socket listening "
                "on port " + to_string(port) + " "
                "socket_bind_and_listen failed with code " +
                to_string(errcode));
    }
}

TCPAcceptor::~TCPAcceptor() {
    socket_destroy(&(this->socket));
}

TCPSocket TCPAcceptor::accept() {
    socket_t accepted_socket;

    int errcode = socket_accept(&(this->socket), &accepted_socket);
    if (errcode < 0) throw runtime_error("Could not accept socket" " "
            "socket_accept failed with code " + to_string(errcode));

    return TCPSocket(accepted_socket);
}
