#include "common_transaction.h"

#include <stdlib.h>
#include <fstream>
#include <bitset>
#include <exception>

#include <arpa/inet.h>

using namespace std;

Transaction::Transaction() {}

Transaction::Transaction(char op, unsigned int card) : command(op), card(card),
        sum(0) {}

Transaction::Transaction(char op, unsigned int card, int sum) : command(op),
        card(card), sum(sum) {}

/* Parses metadata stored in input, and returns it in out.
 * out will contain, in order: card checksum, sum checksum, and operation number
 */
static void parse_meta(short input, unsigned short *out) {
    /* 65431 09876 543 210 <- index
     * 00000 00000 001 000 <- value (0x8)
     * card  sum   op  ---
     */

    input = ntohs(input);

    // Extract card checksum
    bitset<16> meta = input;
    out[0] = (meta >> 11).to_ulong();

    // Extract sum checksum
    // Keep last five bits
    bitset<16> mask = 0x1f;
    out[1] = ((meta >> 6) & mask).to_ulong();

    // Extract operation
    // Keep last three bits
    mask = 0x7;
    out[2] = ((meta >> 3) & mask).to_ulong();
}

ifstream& operator>>(ifstream& s, Transaction& val) {
    short raw_meta;
    s.read((char*)&raw_meta, 2);

    unsigned short meta[3];
    parse_meta(raw_meta, meta);

    if (meta[2] == 0) val.command = 'A';
    else if (meta[2] == 1) val.command = 'F';
    else if (meta[2] == 2) val.command = 'P';
    else if (meta[2] == 3) val.command = 'R';
    else if (meta[2] == 4) val.command = 'S';
    else throw runtime_error("unknown operation " + to_string(meta[2]));

    s.read((char*) &(val.card), 4);
    val.card = ntohl(val.card);
    bitset<32> card = val.card;
    if (card.count() != meta[0])
        throw runtime_error("bad card " + to_string(val.card) + " cheksum "
                + to_string(meta[0]));

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
