#include <iostream>
#include <vector>
#include <thread>

#include "common_net.h"
#include "common_message.h"
#include "server_database.h"

using namespace std;

static void agregarMonto(DB &db, Transaction &t, TCPSocket &s);
static void forzarAgregarMonto(DB &db, Transaction &t, TCPSocket &s);
static void registrarTarjeta(DB &db, Transaction &t, TCPSocket &s);
static void consultarMonto(DB &db, Transaction &t, TCPSocket &s);
static void asignarMonto(DB &db, Transaction &t, TCPSocket &s);

class Spinner {
    TCPAcceptor &acceptor;
    DB &db;

    public:
    Spinner(TCPAcceptor &acceptor, DB &db);
    void operator()();
};

class Doorman {
    TCPSocket socket;
    DB &db;

    public:
    Doorman(TCPSocket socket, DB &db);
    void operator()();
};

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "server <port>" << endl;
        return 0;
    }

    short port = 8080;
    if (argv[1]) sscanf(argv[1], "%hd", &port);

    TCPAcceptor acceptor(port);
    DB db;
    Spinner spinner(acceptor, db);
    thread thread_spinner(spinner);

    while (true) {
        if (cin.peek() == EOF) {
            acceptor.shutdown();
            thread_spinner.join();
            break;
        }
        char c;
        cin >> c;
        if (c == 'q') {
            acceptor.shutdown();
            thread_spinner.join();
            break;
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

Spinner::Spinner(TCPAcceptor &acceptor, DB &db) : acceptor(acceptor), db(db) {}

void Spinner::operator()() {
    vector<std::thread> threads;
    while (true) {
        try {
            TCPSocket s = acceptor.accept();
            Doorman dm(move(s), db);
            threads.emplace_back(move(dm));
        } catch (std::exception) { break; }
    }
    for (unsigned int i = 0; i < threads.size(); i++) threads[i].join();
}

Doorman::Doorman(TCPSocket socket, DB &db) : socket(move(socket)), db(db) {}

void Doorman::operator()() {
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
