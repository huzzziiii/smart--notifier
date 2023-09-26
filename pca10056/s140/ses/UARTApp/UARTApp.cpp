#include "UARTApp.h"

UARTApp* UARTApp::mInstance = nullptr;

UARTApp::UARTApp(SystemTask&  systemTask) : mSystemTask(systemTask)
{
    mInstance = this;
}

void UARTApp::UARTCallback(Fifo<DataUnit> fifoRx)
{

    DataUnit parsedInput[7] = {0};
    mInstance->Parse(fifoRx, parsedInput);

    SystemTask::Message msg = SystemTask::GetMessage(reinterpret_cast<const char*>(parsedInput));
    mInstance->mSystemTask.PushMessage(msg);
    int m = 0;
    m++;

    // TODO
}

void UARTApp::Parse(Fifo<DataUnit> fifo, DataUnit* dst)
{
    while (!fifo.IsEmpty())
    {
        *dst++ = fifo.Read();
    }
}
