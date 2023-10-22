#include "Subscriber.h"

void Subscriber::Subscribe(Publisher* publisher)
{
    mPublishers[mCount++] = publisher;
}

void Subscriber::Unsubscribe(Publisher* publisher)
{
    //publisher->Unsubscribe(this); // TODO HUZZI
}
