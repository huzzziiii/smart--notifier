#include "UARTApp.h"

UARTApp* UARTApp::mInstance = nullptr;

UARTApp::UARTApp(SystemTask&  systemTask) : mSystemTask(systemTask)
{
    mInstance = this;
}


void UARTApp::UARTCallback(Fifo<DataUnit>& fifoRx, DataUnit delimiter)
{
    DataUnit parsedInput[100] = {0}; // "hi\r"; /// {0};
    mInstance->Parse(fifoRx, parsedInput, delimiter);
    
     char* ptr2 = reinterpret_cast<char*>(parsedInput, delimiter);
     unsigned char* ptr1 = reinterpret_cast<unsigned char*>(parsedInput);
     uint8_t* ptr = reinterpret_cast< uint8_t*>(parsedInput);

   
    SystemTask::Message msg = SystemTask::GetMessage(parsedInput); 
    mInstance->mSystemTask.PushMessage(msg);
}

void UARTApp::Parse(Fifo<DataUnit>& fifo, DataUnit* dst, DataUnit delimiter)
{
    while (!fifo.IsEmpty())
    {
        DataUnit nextByte = fifo.Peek();
        if (nextByte == delimiter)
        {
	  fifo.Read();    // consume the delimiter
	  break;
        }

        *dst++ = fifo.Read();
    }
}
