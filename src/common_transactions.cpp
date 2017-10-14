#define _XOPEN_SOURCE 500 // snprintf

#include "common_transactions.h"

#include <fstream>
#include <arpa/inet.h>

using namespace std;

ShortTransaction::ShortTransaction(unsigned int card_number)
        : card_number(card_number) {}

LongTransaction::LongTransaction(unsigned int card_number, int sum)
        : card_number(card_number), sum(sum) {}

ErrorTransaction::ErrorTransaction(short error_code) : error_code(error_code) {}

void ErrorTransaction::send(TCPSocket& s) {
    char buf[6];
    snprintf(buf, 6, "%05hd", error_code);
    if (s.send(buf, 5) < 0) throw runtime_error("connection shut down "
            "mid transmission");
}

void ErrorTransaction::receive(TCPSocket& s) {
    char buf[6];
    buf[5] = '\0';
    if (s.receive(buf, 5) < 0) throw runtime_error("connection shut down");
    if (sscanf(buf, "%hd", &error_code) != 1)
        throw runtime_error("bad format: \"" + string(buf) + "\"");
}

void ErrorTransaction::deserialize(std::ifstream& s) {
    throw runtime_error("errors are not stored in binary form");
}

void ErrorTransaction::print(std::ostream& s) {
    char buf[6];
    snprintf(buf, 6, "%05hd", error_code);
    s << buf;
}

void ShortTransaction::send(TCPSocket& s) {
    char buf[11];
    snprintf(buf, 11, "%010u", card_number);
    if (s.send(buf, 10) < 0) throw runtime_error("connection shut down "
            "mid transmission");
}

void ShortTransaction::receive(TCPSocket& s) {
    char buf[11];
    buf[10] = '\0';
    if (s.receive(buf, 10) < 0) throw runtime_error("connection shut down");
    if (sscanf(buf, "%u", &card_number) != 1)
        throw runtime_error("bad format: \"" + string(buf) + "\"");
}

void ShortTransaction::deserialize(std::ifstream& s) {
    s.read((char*) &(card_number), 4);
    card_number = ntohl(card_number);
}

void ShortTransaction::print(std::ostream& s) {
    char buf[11];
    snprintf(buf, 11, "%010u", card_number);
    s << buf;
}

void LongTransaction::send(TCPSocket& s) {
    char buf[21];
    snprintf(buf, 21, "%010u%010d", card_number, sum);
    if (s.send(buf, 20) < 0) throw runtime_error("connection shut down "
            "mid transmission");
}

void LongTransaction::receive(TCPSocket& s) {
    char buf[21];
    buf[20] = '\0';
    if (s.receive(buf, 20) < 0) throw runtime_error("connection shut down");
    if (sscanf(buf, "%010u%010d", &card_number, &sum) != 2)
        throw runtime_error("bad format: \"" + string(buf) + "\"");
}

void LongTransaction::deserialize(std::ifstream& s) {
    s.read((char*) &(card_number), 4);
    card_number = ntohl(card_number);

    s.read((char*) &(sum), 4);
    sum = ntohl(sum);
}

void LongTransaction::print(std::ostream& s) {
    char buf[21];
    snprintf(buf, 21, "%010u%010d", card_number, sum);
    s << buf;
}
