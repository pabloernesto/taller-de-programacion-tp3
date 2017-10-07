#include "common_net.h"
#include <iostream>

#ifndef TRANSACTION_H
#define TRANSACTION_H

class Transaction {
    char command;
    unsigned int card;
    int sum;

public:
    friend std::ifstream& operator>>(std::ifstream& s, Transaction& val);
    friend std::ofstream& operator<<(std::ofstream& s, const Transaction& val);

    friend TCPSocket& operator>>(TCPSocket& s, Transaction& val);
    friend TCPSocket& operator<<(TCPSocket& s, const Transaction& val);
};

#endif
