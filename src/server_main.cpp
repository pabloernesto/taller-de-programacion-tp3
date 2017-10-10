#include <iostream>

#include "common_net.h"
#include "common_transaction.h"

using namespace std;

int main(int argc, char** argv) {
    //if (argc != 2) cerr << "server <port>" << endl;
    TCPAcceptor ac{8080};
    TCPSocket so = ac.accept();

    Transaction t;
    so >> t;

    cout << t.getCommand();
}
