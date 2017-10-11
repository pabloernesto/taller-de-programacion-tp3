#include "server_database.h"
#include <exception>

using namespace std;

int DB::agregarMonto(unsigned int card, int sum) {
    auto ptr = this->cards.find(card);
    if (ptr == this->cards.end())
        throw runtime_error("Se intento modificar tarjeta no inicializada "
                + to_string(card));
    if (sum < 0 && (ptr->second + sum) < 0)
        throw runtime_error("Dinero insuficiente. Monto: " + to_string(sum)
                + " Saldo: " + to_string(ptr->second));
    return ptr->second += sum;
}

int DB::forzarAgregarMonto(unsigned int card, int sum) {
    auto ptr = this->cards.find(card);
    if (ptr == this->cards.end())
        throw runtime_error("Se intento modificar tarjeta no inicializada "
                + to_string(card));
    return ptr->second += sum;
}

int DB::consultarMonto(unsigned int card) {
    auto ptr = this->cards.find(card);
    if (ptr == this->cards.end())
        throw runtime_error("Se intento consultar monto de la tarjeta "
                "no inicializada " + to_string(card));
    return ptr->second;
}

void DB::registrarTarjeta(unsigned int card) {
    auto ptr = this->cards.find(card);
    if (ptr != this->cards.end())
        throw runtime_error("Se intento registrar tarjeta existente "
                + to_string(card));
    this->cards[card] = 0;
}

void DB::asignarMonto(unsigned int card, int sum) {
    auto ptr = this->cards.find(card);
    if (ptr == this->cards.end())
        throw runtime_error("Se intento modificar tarjeta no inicializada "
                + to_string(card));
    ptr->second = sum;
}

