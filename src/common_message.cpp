#include "common_message.h"
#include "common_transactions.h"

#include <stdlib.h>
#include <fstream>
#include <bitset>
#include <exception>
#include <memory>
#include <arpa/inet.h>

using namespace std;

// Print to cout
std::ostream& operator<<(std::ostream& s, Message& val) {
    val.print(s);
    return s;
}

// Send through a TCPSocket
TCPSocket& operator>>(TCPSocket& s, Message& val) {
    val.receive(s);
    return s;
}

// Receive from a TCPSocket
TCPSocket& operator<<(TCPSocket& s, Message& val) {
    val.send(s);
    return s;
}

Message::~Message() {
}

void Message::print(std::ostream& s) {
    s << op;
    this->s->print(s);
}

void Message::send(TCPSocket& s) {
    s.send((char*) &op, 1);
    this->s->send(s);
}

Transaction::Transaction() {
    op = '\0';
    s = unique_ptr<Specific_Transaction>(new EmptyTransaction());
}

Transaction::Transaction(int error_code) {
    op = 'E';
    s = unique_ptr<Specific_Transaction>(new ErrorTransaction(error_code));
}

Transaction::Transaction(char opcode, unsigned int card_number) {
    //verify op
    op = opcode;
    s = unique_ptr<Specific_Transaction>(new ShortTransaction(card_number));
}

Transaction::Transaction(char opcode, unsigned int card_number, int sum) {
    //verify op
    op = opcode;
    s = unique_ptr<Specific_Transaction>(new LongTransaction(card_number, sum));
}

// Read from binary file
std::ifstream& operator>>(std::ifstream& s, Transaction& val) {
    val.deserialize(s);
    return s;
}

void Transaction::receive(TCPSocket& s) {
    if (s.receive(&op, 1) < 0) throw runtime_error("connection shut down");

    if (op == 'F')
        this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'R')
        this->s = unique_ptr<Specific_Transaction>(new ShortTransaction());
    else if (op == 'A')
        this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'P')
        this->s = unique_ptr<Specific_Transaction>(new ShortTransaction());
    else if (op == 'S')
        this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'E')
        this->s = unique_ptr<Specific_Transaction>(new ErrorTransaction());
    else
        throw runtime_error("unknown operation " + to_string(op));

    this->s->receive(s);
}

/* Parses metadata stored in input, and returns it in out.
 * out will contain, in order: card checksum, sum checksum, and operation number
 */
static void parse_meta(short input, unsigned short *out) {
    /* 65431 09876 543 210 <- index
     * 00000 00000 001 000 <- value (0x8)
     * card  sum   op  ---
     */

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

void Transaction::deserialize(std::ifstream& s) {
    short raw_meta;
    s.read((char*)&raw_meta, 2);
    raw_meta = ntohs(raw_meta);

    unsigned short meta[3];
    parse_meta(raw_meta, meta);

    if (meta[2] == 0) op = 'A';
    else if (meta[2] == 1) op = 'F';
    else if (meta[2] == 2) op = 'P';
    else if (meta[2] == 3) op = 'R';
    else if (meta[2] == 4) op = 'S';
    else throw runtime_error("unknown operation " + to_string(meta[2]));

    switch (op) {
        case 'A': case 'F': case 'S':
            this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
            break;
        case 'R': case 'P':
            this->s = unique_ptr<Specific_Transaction>(new ShortTransaction());
            break;
    }
    this->s->deserialize(s);

    /* This checksum must be loaded modulo-32 because the binary storage
     * has only 5 bits of capacity.
     */
    int checksum;

    bitset<32> card = this->s->getCard();
    checksum = card.count() % 32;
    if (checksum != meta[0])
        throw runtime_error("bad card " + to_string(this->s->getCard())
                + " cheksum " + to_string(meta[0]));

    try {
        bitset<32> sum = this->s->getSum();
        checksum  = sum.count() % 32;
    } catch (exception) { checksum = 0; }
    if (checksum != meta[1]) throw runtime_error("bad sum "
            + to_string(this->s->getSum()) + " checksum " + to_string(meta[1]));
}

Response::Response() {
    op = '\0';
    s = unique_ptr<Specific_Transaction>(new EmptyTransaction());
}

Response::Response(int error_code) {
    op = 'E';
    s = unique_ptr<Specific_Transaction>(new ErrorTransaction(error_code));
}

Response::Response(char opcode, unsigned int card_number) {
    //verify op
    op = opcode;
    s = unique_ptr<Specific_Transaction>(new ShortTransaction(card_number));
}

Response::Response(char opcode, unsigned int card_number, int sum) {
    //verify op
    op = opcode;
    s = unique_ptr<Specific_Transaction>(new LongTransaction(card_number, sum));
}

void Response::receive(TCPSocket& s) {
    if (s.receive(&op, 1) < 0) throw runtime_error("connection shut down");

    if (op == 'F') this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'R') this->s = unique_ptr<Specific_Transaction>(new ShortTransaction());
    else if (op == 'A') this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'P') this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'S') this->s = unique_ptr<Specific_Transaction>(new LongTransaction());
    else if (op == 'E') this->s = unique_ptr<Specific_Transaction>(new ErrorTransaction());
    else throw runtime_error("unknown operation " + to_string(op));

    this->s->receive(s);
}
