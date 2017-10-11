#include "server_database.h"

#include <iostream>

using namespace std;

static void pass();
static void fail(string message);

int main(int argc, char** argv) {
    try {
        DB db;
        int m = db.consultarMonto(1);
        fail("empty db did not throw when queried, returned " + to_string(m));
    } catch (std::exception) {
        pass();
    }
}

static void pass() {
    cout << "test passed" << endl;
}

static void fail(string message) {
    cerr << "test failed: " << message << endl;
}
