#pragma once

#include "uart.h"
#include "Fifo.h"
#include "SystemTask.h"

#include <cstdint>

class UARTApp
{
    public:
    UARTApp(SystemTask& systemTask);
    static void UARTCallback(Fifo<DataUnit>& fifoRx, DataUnit delimiter);
    
    private:
    void Parse(Fifo<DataUnit>& fifo, DataUnit* dst, DataUnit delimiter);

    SystemTask& mSystemTask;
    static UARTApp* mInstance;
};
