#pragma once
#include "Publisher.h"

class Publisher;

class Subscriber
{
    public:
    virtual ~Subscriber() = default;
    void Subscribe(Publisher* publisher)
    {
        mPublisher = publisher;
    }
    
    void Unsubscribe(Publisher* publisher)
    {
        mPublisher = nullptr;
    }

    virtual void Update(Publisher*) = 0;    // TODO HUZZI: use const
    
    private:
    Publisher* mPublisher = nullptr;
};
