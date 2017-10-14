#include <iostream>

#include "common_net.h"
#include "common_message.h"
#include <stdio.h> // sscanf

using namespace std;

int main(int argc, char** argv) {
    //if (argc != 2) cerr << "server <port>" << endl;
    short port = 8080;
    if (argv[1]) sscanf(argv[1], "%hd", &port);
    TCPAcceptor acceptor(port);
    TCPSocket socket = acceptor.accept();

    Transaction t;
    while (true) {
        try { socket >> t; }
        catch (exception) { break; }
        cout << t << endl;
    }
}
