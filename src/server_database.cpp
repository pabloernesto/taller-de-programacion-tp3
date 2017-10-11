#include "server_database.h"
#include <exception>

using namespace std;

int DB::agregarMonto(unsigned int card) {
    return 0;
}

int DB::forzarAgregarMonto(unsigned int card) {
    return 0;
}

int DB::consultarMonto(unsigned int card) {
    auto ptr = this->cards.find(card);
    if (ptr == this->cards.end())
        throw runtime_error("Se intento consultar monto de la tarjeta "
                "no inicializada " + to_string(card));
    return ptr->second;
}

void DB::registrarTarjeta(unsigned int card) {
}

void DB::asignarMonto(unsigned int card) {
}

