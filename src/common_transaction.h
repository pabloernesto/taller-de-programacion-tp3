#include "common_net.h"
#include <iostream>

#ifndef TRANSACTION_H
#define TRANSACTION_H

class Transaction {
    char command;
    unsigned int card;
    int sum;

public:
    Transaction();
    Transaction(char op, unsigned int card);
    Transaction(char op, unsigned int card, int sum);

    char getCommand() { return this->command; }

    friend std::ifstream& operator>>(std::ifstream& s, Transaction& val);

    friend TCPSocket& operator>>(TCPSocket& s, Transaction& val);
    friend TCPSocket& operator<<(TCPSocket& s, const Transaction& val);
};

#endif
