#include <iostream>

#include "common_net.h"
#include "common_transaction.h"

using namespace std;

int main(int argc, char** argv) {
    //if (argc != 4) cerr << "client <server ip> <port> <input file>" << endl;
    TCPSocket sock{"127.0.0.1", 8080};

    Transaction t('z', 12);
    sock << t;
}
