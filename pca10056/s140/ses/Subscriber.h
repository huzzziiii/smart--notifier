#pragma once
#include "Publisher.h"

class Publisher;

class Subscriber
{
    public:
    virtual ~Subscriber() = default;
    virtual void Update(Publisher*) = 0;    // TODO HUZZI: use const
};
