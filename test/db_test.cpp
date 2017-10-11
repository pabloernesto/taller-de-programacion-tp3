#include "server_database.h"

#include <iostream>

using namespace std;

static void pass();

#define fail(message) cerr << __FUNCTION__ << " failed: " << message << endl

static void emptyThrows();
static void newCardIsEmpty();
static void repeatedRegistrationThrows();

int main(int argc, char** argv) {
    emptyThrows();
    newCardIsEmpty();
    repeatedRegistrationThrows();
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

static void newCardIsEmpty() {
    DB db;
    db.registrarTarjeta(1);
    int m = db.consultarMonto(1);
    if (m != 0) fail("new card returned " + to_string(m) + " sum");
    else pass();
}

static void repeatedRegistrationThrows() {
    DB db;
    db.registrarTarjeta(1);
    try { db.registrarTarjeta(1); fail("nothing happened"); }
    catch (std::exception) { pass(); }
}
