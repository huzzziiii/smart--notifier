#pragma once

#include "uart.h"
#include "mcp9808.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

class SystemTask
{
    public:

    SystemTask(MCP9808& mcp9808, UART& uart);
    void Start();
    
    enum class Message : uint8_t
    {
        SUBSCRIBE_TEMP_NOTIFICATIONS,
        UNSUBSCRIBE_TEMP_NOTIFICATIONS,
        INVALID
    };

    private:

    static void Process(void* instance);
    void Run();

    
    MCP9808& mMcp9808;
    UART& mUART;

    static QueueHandle_t mTaskQueue;
    TaskHandle_t taskHandle{NULL};
};
