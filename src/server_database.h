#include <map>
#include <limits.h>

#ifndef DATABASE_H
#define DATABASE_H

class DB {
    std::map<unsigned int, int> cards{{0, INT_MAX}};
    public:
    int agregarMonto(unsigned int card, int sum);
    int forzarAgregarMonto(unsigned int card, int sum);
    int consultarMonto(unsigned int card);
    void registrarTarjeta(unsigned int card);
    void asignarMonto(unsigned int card, int sum);
};

class BadCard : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class BadSum : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif
