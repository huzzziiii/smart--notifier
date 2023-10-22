#include "SystemTask.h"
#include "uart.h"
#include "mcp9808.h"

QueueHandle_t SystemTask::mTaskQueue;

SystemTask::SystemTask(NotificationManager& notificationManager, MCP9808& mcp9808, UART& uart) : 
    mNotificationManager(notificationManager),
    mMcp9808(mcp9808), 
    mUART(uart)
{
     mTaskQueue =  xQueueCreate(10, 1);
}

void SystemTask::Start()
{
    // TODO: think about stack size!
    if (xTaskCreate(SystemTask::Process, "Run", 100, this, 0, &mTaskHandle) != pdPASS)   
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    } 
}

void SystemTask::Process(void* instance)
{
    auto* app = static_cast<SystemTask*>(instance);
    app->Run();
}

void SystemTask::Run()
{
    // initialize the desired modules
    mUART.Init();
    mMcp9808.Start();
    
    uint8_t rcvdMsg;
    Message curMsg;
    
    //mUART.Print("...Welcome...");

    while(true)
    {
        if (xQueueReceive(mTaskQueue, &rcvdMsg, portMAX_DELAY) == pdPASS)      // wait for the user input over UART (for now!)
        {
	  curMsg = static_cast<Message>(rcvdMsg);	
	  const char* msgStr = msgLookup[static_cast<uint8_t>(curMsg)].msgStr;
	  mUART.Print("Received System message: %s", msgStr);

	  switch (curMsg)
	  {
	      case Message::SUBSCRIBE_TEMP_NOTIFICATIONS:
	      {
		mNotificationManager.Subscribe(&mMcp9808);
		break;
	      }

	      case Message::UNSUBSCRIBE_TEMP_NOTIFICATIONS:
	      {
		mNotificationManager.Unsubscribe(&mMcp9808);
		break;
	      }

	      default:
		break;
	  }
        }
    }
}

void SystemTask::PushMessage(SystemTask::Message message, bool fromISR)
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    if (fromISR)
    {
        xQueueSendFromISR(mTaskQueue, &message, &xHigherPriorityTaskWoken);
    }
    else
    {
        xQueueSend(mTaskQueue, &message, 0);
    }
}

SystemTask::Message SystemTask::GetMessage(DataUnit* value) 
{
    if (!strcmp("tempOn", reinterpret_cast<const char*>(value)))
    {
        return Message::SUBSCRIBE_TEMP_NOTIFICATIONS;
    }

    if (!strcmp("tempOff", reinterpret_cast<const char*>(value)))
    {
        return Message::UNSUBSCRIBE_TEMP_NOTIFICATIONS;
    }

    return Message::INVALID;
}
