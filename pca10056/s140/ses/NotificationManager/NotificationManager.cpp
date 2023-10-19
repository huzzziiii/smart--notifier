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
    mUART.Print(notification.msg);
   
    //int ret = mBLENotifierSrv.UpdateValue(reinterpret_cast<uint8_t*>(notification.msg));
    //if (ret != NRF_SUCCESS)
    //{
    //    int m = 0;
    //    m++;
    //}
    int y=0;
    y++;
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


	  //Notification notification;
	  //strncpy(notification.msg, msg, sizeof(notification.msg) - 1);
	  //notification.category = Publisher::Category::TEMPERATURE;

	  Notification notification = MakeNotification(msg, Publisher::Category::TEMPERATURE);
	  //mUART.Print(notification.msg);
	  PushNotification(notification);
	  break;
        }
        
        default:
	  break;
    }
}

//Publisher::Category NotificationManager::GetPublisherCategory(Publisher* requestedPublisher)
//{
//    for (auto* publisher : mPublishers)
//    {
//        if (publisher == requestedPublisher)
//        {
//	  return publisher->GetCategory();
//        }
//    }
//    return Publisher::Category::MAX;
//}
