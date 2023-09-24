#pragma once

#include "uart.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

class SystemTask
{
    public:
    SystemTask(UART& uart);
    void Start();

    private:

    static void Process(void* instance);
    void Run();


    UART& mUART;

    static QueueHandle_t mTaskQueue;
    TaskHandle_t taskHandle{NULL};
};
