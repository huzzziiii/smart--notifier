#pragma once

#include "Subscriber.h"

#include <cstdint>

class Publisher
{
    public:

    enum class Category : uint8_t
    {
        TEMPERATURE,
        // other categories...
        MAX
    };

    Publisher(Category category);
    bool Subscribe(Subscriber* subscriber); // TODO HUZZI: use const ref
    bool Unsubscribe(Subscriber* subscriberToUnsubscribe);
    void Notify();
    Publisher::Category GetCategory();

    private:
    bool SubscriberExists(Subscriber* subscriber);

    static constexpr int MAX_SUBSCRIBERS = 10;
    Subscriber* mSubscribers[MAX_SUBSCRIBERS] = {0};
    uint8_t mSubscriberCount = 0;
    Category mCategory;
};
