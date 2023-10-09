#include "uart.h"


static UART* pInstance;

UART::UART(NRF_UART_Type& uartInstance, const UartCommParams& uartCommParams, Callback handler, DataUnit delimiter) 
	      :   mUARTx(uartInstance),
		mHandler(handler),
		mDelimiter(delimiter)
{
        mUARTConfig.pselTxd	    = uartCommParams.txPinNo;
        mUARTConfig.pselRxd	    = uartCommParams.rxPinNo;
        mUARTConfig.pselRts	     = uartCommParams.rtsPinNo;
        mUARTConfig.pselCts	     = uartCommParams.ctsPinNo;
        mUARTConfig.baudRate    = uartCommParams.baudRate;
        mUARTConfig.hwFlowCntl = uartCommParams.hwFlowCntl;
        mUARTConfig.irqPriority    = uartCommParams.irqPriority;

        mUARTConfig.interruptFlags = NRF_UART_INT_MASK_TXDRDY | NRF_UART_INT_MASK_RXDRDY;

        pInstance = this;
        

        mSemaphore = xSemaphoreCreateBinary();

        if (!mSemaphore)
        {
	  //throw;
        }
}

void UART::Init()
{
    SetTxRxPins();
    SetBaudRate(mUARTConfig.baudRate);
    ConfigureInterrupts();
    
    if (mHandler)
    {
        ConfigureInterrupts();
    }

    EnableUART();
    SetUARTReg(NRF_UART_TASK_STARTRX, 1);       // start RX
}

void UART::SetTxRxPins()
{
    mUARTx.PSEL.RXD = mUARTConfig.pselRxd;
    mUARTx.PSEL.TXD = mUARTConfig.pselTxd;
}

void UART::ConfigureInterrupts()
{
    SetUARTReg(NRF_UART_EVENT_ERROR, 0);    
    SetUARTReg(NRF_UART_EVENT_RXDRDY, 0);
    SetUARTReg(NRF_UART_EVENT_RXTO, 0);
    SetUARTReg(NRF_UART_EVENT_TXDRDY, 0);

    EnableInterrupt(mUARTConfig.interruptFlags);			          // enable interrupt in an nRF register

    NRFX_IRQ_ENABLE(nrfx_get_irq_number((void *)&mUARTx));	// enable interrupt in an NVIC register 
    NRFX_IRQ_PRIORITY_SET(nrfx_get_irq_number((void *)&mUARTx),  mUARTConfig.irqPriority);
}

void UART::IRQHandler()
{
    bool eventRxdRdy = GetRegValue(NRF_UART_EVENT_RXDRDY);
    bool eventTxdRdy = GetRegValue(NRF_UART_EVENT_TXDRDY);

    bool isRxdRdyIRQSet = GetIRQStatus(NRF_UART_INT_MASK_RXDRDY);
    bool isTxdRdyIRQSet = GetIRQStatus(NRF_UART_INT_MASK_TXDRDY);

    // byte is received
    if (isRxdRdyIRQSet && eventRxdRdy)
    {
        // disable the read interrupt
        SetUARTReg(NRF_UART_EVENT_RXDRDY, 0);

        uint32_t data = ReadRXD();
        mFifoRx.Write(data);
    
         // if end-of-input byte (carriage return) is received, process the FIFO RX
        if (data == mDelimiter
        )
        {
	  mHandler(mFifoRx, mDelimiter);
        }
    }
    
    // byte is transmitted
    if (isTxdRdyIRQSet && eventTxdRdy)
    {
        // disable the write interrupt
        SetUARTReg(NRF_UART_EVENT_TXDRDY, 0);
    
        // continue sending each byte at a time until FIFO TX is empty
        if (mFifoTx.IsEmpty())
        {
	  SetUARTReg(NRF_UART_TASK_STOPTX, 0);  // stop the transmission

	  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	  // unblock the task now the UART transmission is over
	  //vTaskNotifyGiveFromISR(pdTRUE, portMAX_DELAY);
	  //vTaskNotifyGiveFromISR(mTaskHandle, &xHigherPriorityTaskWoken);
	  xSemaphoreGiveFromISR(mSemaphore, &xHigherPriorityTaskWoken);

        }
        else
        {
	  DataUnit value = mFifoTx.Read();
	  WriteTXD(value);
        }
    }
}

void UART::StartTX(DataUnit* buffer, size_t length)
{
    // populate the TX FIFO
    mFifoTx.WriteElements(buffer, length);

    // enable the STARTTX task / UART transmitter	
    SetUARTReg(NRF_UART_TASK_STARTTX, 1);

    // Read the first byte and write to TXD to initiate a UART transmission
    DataUnit value = mFifoTx.Read();
    WriteTXD(value);
}
    
void UART::EnableUART()
{
    mUARTx.ENABLE = UART_ENABLE_ENABLE_Enabled;
}

void UART::EnableInterrupt(uint32_t mask)
{
    mUARTx.INTENSET = mask;
}

 void UART::SetConfigReg()
 {
    mUARTx.CONFIG = mUARTConfig.hwFlowCntl | mUARTConfig.parity;
 }

 void UART::SetBaudRate(UartBaudrate baudRate)
 {
    mUARTx.BAUDRATE = baudRate;
}

void UART::SetUARTReg(uint32_t reg, uint32_t value)
{
    * ( (volatile uint32_t*) ( reinterpret_cast<uint8_t*>(&mUARTx) + reg) ) = value;
}

uint32_t UART::GetRegValue(uint32_t reg) const
{
    //uint32_t value = * (static_cast<volatile uint32_t*> ( reinterpret_cast<uint8_t*>(&mUARTx) + reg ) );
    
    uint32_t value = *( (volatile uint32_t*) ( reinterpret_cast<uint8_t*>(&mUARTx) + reg ) );
    return value;
}

 bool UART::GetIRQStatus(uint32_t mask) const 
{
    return mUARTx.INTENSET & mask;
}

uint32_t UART::ReadRXD() const
{
    return mUARTx.RXD; 
}

void UART::WriteTXD(uint32_t value) 
{
    mUARTx.TXD = value;
}


extern "C"
{
    void UARTE0_UART0_IRQHandler(void)
    {   
        pInstance->IRQHandler(); 
    }
}
