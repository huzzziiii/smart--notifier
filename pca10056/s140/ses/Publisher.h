#pragma once

#include "Subscriber.h"

#include <cstdint>

class Publisher
{
    public:
    bool Subscribe(Subscriber* subscriber); // TODO HUZZI: use const ref
    bool Unsubscribe(Subscriber* subscriberToUnsubscribe);
    void Notify();

    private:

    bool SubscriberExists(Subscriber* subscriber);

    static constexpr int MAX_SUBSCRIBERS = 10;
    Subscriber* mSubscribers[MAX_SUBSCRIBERS] = {0};
    uint8_t mSubscriberCount = 0;
};
