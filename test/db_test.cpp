#include "server_database.h"

#include <iostream>

using namespace std;

static void pass();

#define fail(message) cerr << __FUNCTION__ << " failed: " << message << endl

static void emptyThrows();
static void newCardIsEmpty();
static void repeatedRegistrationThrows();
static void addPositiveSumOk();
static void addNegativeSumOk();
static void setSumOk();

int main(int argc, char** argv) {
    emptyThrows();
    newCardIsEmpty();
    repeatedRegistrationThrows();
    addPositiveSumOk();
    addNegativeSumOk();
    setSumOk();
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

static void addPositiveSumOk() {
    DB db;
    db.registrarTarjeta(1);
    db.agregarMonto(1, 100);
    db.agregarMonto(1, 100);
    int m = db.consultarMonto(1);
    if (m == 200) pass();
    else fail("wrong sum. Expected 200, got " + to_string(m));
}

static void addNegativeSumOk() {
    DB db;
    db.registrarTarjeta(1);
    db.agregarMonto(1, 100);
    db.agregarMonto(1, -58);
    int m = db.consultarMonto(1);
    if (m == 42) pass();
    else fail("wrong sum. Expected the meaning of life, the universe, "
            "and everything, got " + to_string(m));
}

static void setSumOk() {
    DB db;
    db.registrarTarjeta(1);
    db.asignarMonto(1, 1337);
    int m = db.consultarMonto(1);
    if (m == 1337) pass();
    else fail("wrong sum. Expected 1337 got " + to_string(m));
}
