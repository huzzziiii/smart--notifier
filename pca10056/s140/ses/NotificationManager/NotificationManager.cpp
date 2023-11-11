#include "NotificationManager.h"
#include "mcp9808.h"
#include "Publisher.h"

Notification NotificationManager::MakeNotification(char* msg, Publisher::Category category)
{
    Notification notification = {0};
    strncpy(notification.msg, msg, sizeof(notification.msg));
    notification.category = category;
    return notification; 
}

void NotificationManager::PushNotification(Notification notification)
{
    mNotifications.Write(notification);
}

void NotificationManager::PublishNotification(Notification notification, uint16_t value)
{
    mUART.Print(notification.msg);
    int ret = mBLENotifierSrv.UpdateValue(value);
    if (ret != NRF_SUCCESS)
    {
        int m = 0;
        m++;
    }
}

void NotificationManager::Update(Publisher* publisher) 
{
    // retrieve the publisher's category
    auto category = publisher->GetCategory();

    switch (category)
    {
        case Publisher::Category::TEMPERATURE:
        {
	  MCP9808* mcp9808 = dynamic_cast<MCP9808*>(publisher);
	  if (mcp9808 == nullptr)
	  {
	      return;
	  }

	  uint16_t value = mcp9808->GetTempInC();

	  char msg[30] = {0};
	  snprintf (msg, sizeof(msg) - 1, "Temp = %uC", value);

	  Notification notification = MakeNotification(msg, Publisher::Category::TEMPERATURE);
	  PushNotification(notification);
	  PublishNotification(notification, value);

	  break;
        }
        
        default:
	  break;
    }
}

void NotificationManager::Subscribe(Publisher* publisher)
{
    publisher->Subscribe(this);
}

void NotificationManager::Unsubscribe(Publisher* publisher)
{
    publisher->Unsubscribe(this);
}

