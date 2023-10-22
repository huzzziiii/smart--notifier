#pragma once

#include "Publisher.h"
#include "Fifo.h"

class Publisher;

class Subscriber
{
    public:
    virtual ~Subscriber() = default;

    void Subscribe(Publisher*);
    void Unsubscribe(Publisher* publisher);

    virtual void Update(Publisher*) = 0;    // TODO HUZZI: use const

    private:
    
    static constexpr uint8_t MAX_PUBLISHERS = 10;
    uint8_t mCount = 0;
    Publisher* mPublishers[MAX_PUBLISHERS] = {0};
};
