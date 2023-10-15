#include "Subscriber.h"

void Subscriber::Subscribe(Publisher* publisher)
{
    mPublishers.Write(publisher);
}
