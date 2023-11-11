#pragma once

#include "Subscriber.h"

#include <cstdint>

class Subscriber; 

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
    virtual ~Publisher() = default;
    bool Subscribe(Subscriber* subscriber); // TODO HUZZI: use const ref
    bool Unsubscribe(Subscriber* subscriberToUnsubscribe);
    void Notify();
    Publisher::Category GetCategory();
    virtual void OnSubscribeChange(bool subscribeRequest) {}

    private:
    bool SubscriberExists(Subscriber* subscriber);

    Category mCategory;
    static constexpr int MAX_SUBSCRIBERS = 10;
    Subscriber* mSubscribers[MAX_SUBSCRIBERS] = {nullptr};
    uint8_t mSubscriberCount = 0;
};
