#include "common_transaction.h"

using namespace std;

ifstream& operator>>(ifstream& s, Transaction& val) {
    return s;
}

ofstream& operator<<(ofstream& s, const Transaction& val) {
    return s;
}

TCPSocket& operator>>(TCPSocket& s, Transaction& val) {
    s >> val.command;
    // get 10 chars
    // set val.card
    if ((val.command == 'A') || (val.command == 'F') ||
        (val.command == 'R'))
        // get 10 chars
        // set this->sum
        ;
    return s;
}

TCPSocket& operator<<(TCPSocket& s, const Transaction& val) {
    s << val.command;
    s << val.card;
    if ((val.command == 'A') || (val.command == 'F') ||
        (val.command == 'R'))
        s << val.sum;
    return s;
}
