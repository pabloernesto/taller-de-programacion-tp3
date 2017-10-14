#include <iostream>

#include "common_net.h"
#include "common_message.h"
#include "server_database.h"

using namespace std;

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

        Response r;
        switch (t.getOpcode()) {
            case 'A':
                try {
                    int sum = db.agregarMonto(t.getCard(), t.getSum());
                    r = Response('A', t.getCard(), sum);
                    socket << r;
                } catch (exception) { r = Response(1); socket << r; }
                break;
            case 'F':
                try {
                    int sum = db.forzarAgregarMonto(t.getCard(), t.getSum());
                    r = Response('F', t.getCard(), sum);
                    socket << r;
                } catch (exception) { r = Response(2); socket << r; }
                break;
            case 'R':
                try {
                    db.registrarTarjeta(t.getCard());
                    r = Response('R', t.getCard());
                    socket << r;
                } catch (exception) { r = Response(3); socket << r; }
                break;
            case 'P':
                try {
                    int sum = db.consultarMonto(t.getCard());
                    r = Response('P', t.getCard(), sum);
                    socket << r;
                } catch (exception) { r = Response(4); socket << r; }
                break;
            case 'S':
                try {
                    db.asignarMonto(t.getCard(), t.getSum());
                    r = Response('S', t.getCard(), t.getSum());
                    socket << r;
                } catch (exception) { r = Response(5); socket << r; }
                break;
        }
    }
}
