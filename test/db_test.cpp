#include "server_database.h"

#include <iostream>

using namespace std;

static void pass();

#define fail(message) cerr << __FUNCTION__ << " failed: " << message << endl

static void emptyThrows();

int main(int argc, char** argv) {
    emptyThrows();
}

static void pass() {
    cout << "test passed" << endl;
}

static void emptyThrows() {
    try {
        DB db;
        int m = db.consultarMonto(1);
        fail("empty db did not throw when queried, returned " + to_string(m));
    } catch (std::exception) {
        pass();
    }
}
