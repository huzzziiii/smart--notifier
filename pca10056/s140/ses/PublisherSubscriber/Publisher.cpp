#include "Publisher.h"
#include "FreeRTOS.h"
#include "task.h"

Publisher::Publisher(Category category) : mCategory(category)
{
    // no-op
}

bool Publisher::Subscribe(Subscriber* subscriber)
{
    for (int idx = 0; idx < MAX_SUBSCRIBERS; idx++)
    {
        if (!subscriber || SubscriberExists(subscriber))
        {
	  return false;
        }

        if (mSubscribers[idx] == nullptr)
        {
	  mSubscribers[mSubscriberCount++] = subscriber;
	  subscriber->Subscribe(this);	// allow subscriber to track its publisher as well for lifetime management purposes

	  // ensure scheduler has kicked off before
	  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
	  {
	      OnSubscribeChange(true);
	  }
	  return true;
        }
    }
    return false;
}

bool Publisher::Unsubscribe(Subscriber* subscriberToUnsubscribe)
{
    for (auto& subscriber : mSubscribers)
    {
        if (subscriber != nullptr && subscriber == subscriberToUnsubscribe)
        {
	  subscriber = nullptr;
	  OnSubscribeChange(false);
	  return true;
        }
    }
    return false;
}

void Publisher::Notify()
{
    if (mSubscriberCount > 0)
    {
        for (const auto subscriber : mSubscribers)
        {   
            if (subscriber != nullptr)
            {
	      subscriber->Update(this);
            }
        }
    }
}

 bool Publisher::SubscriberExists(Subscriber* subscriber) 
 {
    for (int idx = 0; idx < MAX_SUBSCRIBERS; idx++)
    {
        if (mSubscribers[idx] == subscriber)
        {
	  return true;
       }
   }
   return false;
}

Publisher::Category Publisher::GetCategory()
{
    return mCategory;
}