#include <iostream>
#include <fstream>

#include "common_net.h"
#include "common_message.h"

using namespace std;

int main(int argc, char** argv) {
    short port = 8080;
    if (argv[1]) sscanf(argv[1], "%hd", &port);
    TCPSocket sock("127.0.0.1", port);
    ifstream file("data.bin", ios::in | ios::binary);

    while (file.peek() != ifstream::traits_type::eof()) {
        Transaction t;
        file >> t;
        sock << t;
    }
}
