#include <iostream>

#include "common_net.h"
#include "common_message.h"

using namespace std;

int main(int argc, char** argv) {
    //if (argc != 2) cerr << "server <port>" << endl;
    TCPAcceptor acceptor(8080);
    TCPSocket socket = acceptor.accept();

    Transaction t;
    while (true) {
        try { socket >> t; }
        catch (exception) { break; }
        cout << t;
    }
}
