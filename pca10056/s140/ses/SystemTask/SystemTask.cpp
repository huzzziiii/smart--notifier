#include "SystemTask.h"
#include "uart.h"
#include "mcp9808.h"

QueueHandle_t SystemTask::mTaskQueue;

SystemTask::SystemTask(MCP9808& mcp9808, UART& uart) : 
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
        int m = 0;
        m++;
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

    while(true)
    {
        if (xQueueReceive(mTaskQueue, &rcvdMsg, portMAX_DELAY) == pdPASS)      // wait for the user input over UART (for now!)
        {
	  curMsg = static_cast<Message>(rcvdMsg);	
	  const char* msgStr = msgLookup[static_cast<uint8_t>(rcvdMsg)].msgStr;
            // mUart.PrintUart("Received System message: %s", msgStr);

	  switch (curMsg)
	  {
	      case Message::SUBSCRIBE_TEMP_NOTIFICATIONS:
	      {
		break;
	      }

	      case Message::UNSUBSCRIBE_TEMP_NOTIFICATIONS:
	      {
		break;
	      }

	      default:
		break;
	  }
        }
    }
}

void SystemTask::PushMessage(SystemTask::Message message)
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(mTaskQueue, &message, &xHigherPriorityTaskWoken);
}

SystemTask::Message SystemTask::GetMessage(const char* str) 
{
    if (!strcmp("tempOn", str))
    {
        return Message::SUBSCRIBE_TEMP_NOTIFICATIONS;
    }

    if (!strcmp("tempOff", str))
    {
        return Message::UNSUBSCRIBE_TEMP_NOTIFICATIONS;
    }
    return Message::INVALID;
}

