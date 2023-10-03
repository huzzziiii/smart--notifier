#pragma once

#include "uart.h"
#include "Fifo.h"
#include "SystemTask.h"

#include <cstdint>

class UARTApp
{
    public:
    UARTApp(SystemTask& systemTask);
    static void UARTCallback(Fifo<DataUnit> fifoRx);
    
    private:
    void Parse(Fifo<DataUnit> fifo, DataUnit* dst);

    SystemTask& mSystemTask;
    static UARTApp* mInstance;
};
