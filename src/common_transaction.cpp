#include "common_transaction.h"

#include <stdlib.h>

using namespace std;

Transaction::Transaction() {}

Transaction::Transaction(char op, unsigned int card) : command(op), card(card),
        sum(0) {}

Transaction::Transaction(char op, unsigned int card, int sum) : command(op),
        card(card), sum(sum) {}

ifstream& operator>>(ifstream& s, Transaction& val) {
    return s;
}

ofstream& operator<<(ofstream& s, const Transaction& val) {
    return s;
}

TCPSocket& operator>>(TCPSocket& s, Transaction& val) {
    char buf[12];
    s.receive(buf, 11);
    buf[11] = '\0';

    val.command = buf[0];
    val.card = strtoul(buf + 1, nullptr, 10);
    if ((val.command == 'A') || (val.command == 'F') ||
            (val.command == 'R')) {
        s.receive(buf, 10);
        buf[10] = '\0';
        val.sum = strtol(buf, nullptr, 10);
    }
    return s;
}

TCPSocket& operator<<(TCPSocket& s, const Transaction& val) {
    s.send(&(val.command), 1);
    char buf[11];
    snprintf(buf, 11, "%010u", val.card);
    s.send(buf, 10);
    if ((val.command == 'A') || (val.command == 'F') ||
            (val.command == 'R')) {
        snprintf(buf, 11, "%010d", val.sum);
        s.send(buf, 10);
    }
    return s;
}
