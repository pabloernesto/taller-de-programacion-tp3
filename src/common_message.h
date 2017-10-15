#include "common_net.h"
#include <iostream>
#include <memory>

#ifndef MESSAGE_H
#define MESSAGE_H

class Message;
class Response;
class Transaction;
class Specific_Transaction;

class Specific_Transaction {
    public:
    virtual void send(TCPSocket& s) = 0;
    virtual void receive(TCPSocket& s) = 0;
    virtual void deserialize(std::ifstream& s) = 0;
    virtual void print(std::ostream& s) = 0;

    virtual int getErrcode() = 0;
    virtual unsigned int getCard() = 0;
    virtual int getSum() = 0;
};

class Message {
    protected:
    char op;
    std::unique_ptr<Specific_Transaction> s;

    private:
    void print(std::ostream& s);
    void send(TCPSocket& s);

    virtual void receive(TCPSocket& s) = 0;

    public:
    ~Message();

    char getOpcode() { return op; }
    int getErrcode() { return s->getErrcode(); }
    unsigned int getCard() { return s->getCard(); }
    int getSum() { return s->getSum(); }

    // Print to cout
    friend std::ostream& operator<<(std::ostream& s, Message& val);

    // Move through a TCPSocket
    friend TCPSocket& operator>>(TCPSocket& s, Message& val);
    friend TCPSocket& operator<<(TCPSocket& s, Message& val);
};

class Transaction : public Message {
    public:
    Transaction();
    explicit Transaction(int error_code);
    Transaction(char opcode, unsigned int card_number);
    Transaction(char opcode, unsigned int card_number, int sum);

    void receive(TCPSocket& s);
    void deserialize(std::ifstream& s);

    // Read from binary file
    friend std::ifstream& operator>>(std::ifstream& s, Transaction& val);
};

class Response : public Message {
    public:
    Response();
    explicit Response(int error_code);
    Response(char opcode, unsigned int card_number);
    Response(char opcode, unsigned int card_number, int sum);

    void receive(TCPSocket& s);
};

#endif
