#include <iostream>

#include "common_net.h"
#include "common_message.h"
#include "server_database.h"

using namespace std;

static void agregarMonto(DB &db, Transaction &t, TCPSocket &s);
static void forzarAgregarMonto(DB &db, Transaction &t, TCPSocket &s);
static void registrarTarjeta(DB &db, Transaction &t, TCPSocket &s);
static void consultarMonto(DB &db, Transaction &t, TCPSocket &s);
static void asignarMonto(DB &db, Transaction &t, TCPSocket &s);

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "server <port>" << endl;
        return 0;
    }

    short port = 8080;
    if (argv[1]) sscanf(argv[1], "%hd", &port);
    TCPAcceptor acceptor(port);

    DB db;

    TCPSocket socket = acceptor.accept();

    while (true) {
        Transaction t;
        try { socket >> t; }
        catch (exception) { break; }

        switch (t.getOpcode()) {
            case 'A': agregarMonto(db, t, socket); break;
            case 'F': forzarAgregarMonto(db, t, socket); break;
            case 'R': registrarTarjeta(db, t, socket); break;
            case 'P': consultarMonto(db, t, socket); break;
            case 'S': asignarMonto(db, t, socket); break;
        }
    }
}

static void agregarMonto(DB &db, Transaction &t, TCPSocket &s) {
    try {
        int m = db.agregarMonto(t.getCard(), t.getSum());
        Response r('A', t.getCard(), m);
        cout << t << " -> " << r << endl;
        s << r;
    } catch (BadCard) {
        Response r(2);
        cerr << t << " -> " << r << endl;
        s << r;
    } catch (BadSum) {
        Response r(3);
        cerr << t << " -> " << r << endl;
        s << r;
    }
}

static void forzarAgregarMonto(DB &db, Transaction &t, TCPSocket &s) {
    try {
        int m = db.forzarAgregarMonto(t.getCard(), t.getSum());
        Response r('F', t.getCard(), m);
        cout << t << " -> " << r << endl;
        s << r;
    } catch (BadCard) {
        Response r(2);
        cerr << t << " -> " << r << endl;
        s << r;
    } catch (BadSum) {
        Response r(3);
        cerr << t << " -> " << r << endl;
        s << r;
    }
}

static void registrarTarjeta(DB &db, Transaction &t, TCPSocket &s) {
    try {
        db.registrarTarjeta(t.getCard());
        Response r('R', t.getCard());
        cout << t << " -> " << r << endl;
        s << r;
    } catch (BadCard) {
        Response r(4);
        cerr << t << " -> " << r << endl;
        s << r;
    }
}

static void consultarMonto(DB &db, Transaction &t, TCPSocket &s) {
    try {
        int m = db.consultarMonto(t.getCard());
        Response r('P', t.getCard(), m);
        cout << t << " -> " << r << endl;
        s << r;
    } catch (BadCard) {
        Response r(2);
        cerr << t << " -> " << r << endl;
        s << r;
    }
}

static void asignarMonto(DB &db, Transaction &t, TCPSocket &s) {
    try {
        db.asignarMonto(t.getCard(), t.getSum());
        Response r('S', t.getCard(), t.getSum());
        cout << t << " -> " << r << endl;
        s << r;
    } catch (BadCard) {
        Response r(2);
        cerr << t << " -> " << r << endl;
        s << r;
    }
}
