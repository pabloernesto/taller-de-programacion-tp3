#include <iostream>
#include <fstream>

#include "common_net.h"
#include "common_transaction.h"

using namespace std;

int main(int argc, char** argv) {
    ifstream file("data.bin", ios::in | ios::binary);
    Transaction t;
    file >> t;
    cout << t.getCommand() << endl;
}
