#include "common_message.h"
#include <exception>

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

class EmptyTransaction : public Specific_Transaction {
    public:
    void send(TCPSocket& s) {
        throw std::runtime_error("transaction is empty");
    }

    void receive(TCPSocket& s) {
        throw std::runtime_error("transaction is empty");
    }

    void deserialize(std::ifstream& s) {
        throw std::runtime_error("transaction is empty");
    }

    void print(std::ostream& s) {
        throw std::runtime_error("transaction is empty");
    }

    int getErrcode() { throw std::runtime_error("transaction is empty"); }
    unsigned int getCard() { throw std::runtime_error("transaction is empty"); }
    int getSum() { throw std::runtime_error("transaction is empty"); }
};

class ShortTransaction : public Specific_Transaction {
    unsigned int card_number;

    public:
    ShortTransaction() : ShortTransaction(0) {}
    ShortTransaction(unsigned int card_number);

    void send(TCPSocket& s);
    void receive(TCPSocket& s);
    void deserialize(std::ifstream& s);
    void print(std::ostream& s);

    int getErrcode() { throw std::runtime_error("is not an error"); }
    unsigned int getCard() { return card_number; }
    int getSum() { throw std::runtime_error("has no sum"); }
};

class LongTransaction : public Specific_Transaction {
    unsigned int card_number;
    int sum;

    public:
    LongTransaction() : LongTransaction(0, 0) {}
    LongTransaction(unsigned int card_number, int sum);

    void send(TCPSocket& s);
    void receive(TCPSocket& s);
    void deserialize(std::ifstream& s);
    void print(std::ostream& s);

    int getErrcode() { throw std::runtime_error("is not an error"); }
    unsigned int getCard() { return card_number; }
    int getSum() { return sum; }
};

class ErrorTransaction : public Specific_Transaction {
    int error_code;

    public:
    ErrorTransaction() : ErrorTransaction(0) {}
    ErrorTransaction(int error_code);

    void send(TCPSocket& s);
    void receive(TCPSocket& s);
    void deserialize(std::ifstream& s);
    void print(std::ostream& s);

    int getErrcode() { return error_code; }
    unsigned int getCard() { throw std::runtime_error("is an error"); }
    int getSum() { throw std::runtime_error("is an error"); }
};

#endif
