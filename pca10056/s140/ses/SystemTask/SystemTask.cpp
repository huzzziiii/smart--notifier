#include "SystemTask.h"

QueueHandle_t SystemTask::mTaskQueue;

SystemTask::SystemTask(UART& uart) : mUART(uart)
{
     mTaskQueue =  xQueueCreate(10, 1);
}

void SystemTask::Start()
{
    // TODO: think about stack size!
    if (xTaskCreate(SystemTask::Process, "Run", 350, this, 0, &taskHandle) != pdPASS)   
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
    // mMcp9808.Start();

    while(true)
    {

    }
}
