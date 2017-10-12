#include <iostream>
#include <fstream>

#include "common_net.h"
#include "common_transaction.h"

using namespace std;

int main(int argc, char** argv) {
    TCPSocket sock("127.0.0.1", 8080);
    ifstream file("data.bin", ios::in | ios::binary);

    while (file) {
        Transaction t;
        file >> t;
        sock << t;
    }
}
