#include <iostream>
#include <fstream>

#include "common_net.h"
#include "common_message.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr << "client <host> <port> <filename>" << endl;
        return 0;
    }

    short port = 8080;
    if (argv[1]) sscanf(argv[2], "%hd", &port);

    TCPSocket sock(argv[1], port);
    ifstream file(argv[3], ios::in | ios::binary);

    while (file.peek() != ifstream::traits_type::eof()) {
        Transaction t;
        file >> t;
        sock << t;

        Response r;
        sock >> r;
        cout << t << " -> " << r << endl;
    }
}
