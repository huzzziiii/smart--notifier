#pragma once
#include "Publisher.h"
#include "Fifo.h"

class Publisher;

class Subscriber
{
    public:
    virtual ~Subscriber() = default;
    void Subscribe(Publisher*);
    virtual void Update(Publisher*) = 0;    // TODO HUZZI: use const

    private:

    Fifo<Publisher*> mPublishers;
};
