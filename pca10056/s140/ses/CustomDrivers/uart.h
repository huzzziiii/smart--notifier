#pragma once

#include "Fifo.h"

#include "nrf_gpio.h"


#include <FreeRTOS.h>
#include <task.h>
#include "semphr.h"


#include <cstdio>
#include <cstring>
#include <string.h>
#include <queue.h>

using DataUnit = uint8_t;

typedef void (*Callback)(Fifo<DataUnit>& fifo, DataUnit delimiter); 


// template<typename T>
//using Callback = void (*)(Fifo<T>);	  // TODO HUZZI : use an alias
 

 
/** @brief UART events. */
typedef enum    // TODO remove typedef
{
    NRF_UART_EVENT_CTS    = offsetof(NRF_UART_Type, EVENTS_CTS),   /**< Event from CTS line activation. */
    NRF_UART_EVENT_NCTS   = offsetof(NRF_UART_Type, EVENTS_NCTS),  /**< Event from CTS line deactivation. */
    NRF_UART_EVENT_RXDRDY = offsetof(NRF_UART_Type, EVENTS_RXDRDY),/**< Event from data ready in RXD. */
    NRF_UART_EVENT_TXDRDY = offsetof(NRF_UART_Type, EVENTS_TXDRDY),/**< Event from data sent from TXD. */
    NRF_UART_EVENT_ERROR  = offsetof(NRF_UART_Type, EVENTS_ERROR), /**< Event from error detection. */
    NRF_UART_EVENT_RXTO   = offsetof(NRF_UART_Type, EVENTS_RXTO)   /**< Event from receiver timeout. */
} nrf_uart_event_t;

/** @brief UART tasks. */
enum nrf_uart_task_t
{
    NRF_UART_TASK_STARTRX = offsetof(NRF_UART_Type, TASKS_STARTRX), /**< Task for starting reception. */
    NRF_UART_TASK_STOPRX  = offsetof(NRF_UART_Type, TASKS_STOPRX),  /**< Task for stopping reception. */
    NRF_UART_TASK_STARTTX = offsetof(NRF_UART_Type, TASKS_STARTTX), /**< Task for starting transmission. */
    NRF_UART_TASK_STOPTX  = offsetof(NRF_UART_Type, TASKS_STOPTX),  /**< Task for stopping transmission. */
    NRF_UART_TASK_SUSPEND = offsetof(NRF_UART_Type, TASKS_SUSPEND), /**< Task for suspending UART. */
};

enum nrf_tx_rx
{
    NRF_UART_RXD  = offsetof(NRF_UART_Type, RXD),
    NRF_UART_TXD  = offsetof(NRF_UART_Type, TXD)
};


 /** @brief UART interrupts. */
typedef enum
{
    NRF_UART_INT_MASK_CTS	      = UART_INTENCLR_CTS_Msk,    /**< CTS line activation interrupt. */
    NRF_UART_INT_MASK_NCTS	      = UART_INTENCLR_NCTS_Msk,   /**< CTS line deactivation interrupt. */
    NRF_UART_INT_MASK_RXDRDY	  = UART_INTENCLR_RXDRDY_Msk, /**< Data ready in RXD interrupt. */
    NRF_UART_INT_MASK_TXDRDY	= UART_INTENCLR_TXDRDY_Msk, /**< Data sent from TXD interrupt. */
    NRF_UART_INT_MASK_ERROR	 = UART_INTENCLR_ERROR_Msk,  /**< Error detection interrupt. */
    NRF_UART_INT_MASK_RXTO	  = UART_INTENCLR_RXTO_Msk    /**< Receiver timeout interrupt. */
} nrf_uart_int_mask_t;

/** @brief Baudrates supported by UART. */
enum UartBaudrate  
{
    NRF_UART_BAUDRATE_1200    = UART_BAUDRATE_BAUDRATE_Baud1200,    /**< 1200 baud. */
    NRF_UART_BAUDRATE_2400    = UART_BAUDRATE_BAUDRATE_Baud2400,    /**< 2400 baud. */
    NRF_UART_BAUDRATE_4800    = UART_BAUDRATE_BAUDRATE_Baud4800,    /**< 4800 baud. */
    NRF_UART_BAUDRATE_9600    = UART_BAUDRATE_BAUDRATE_Baud9600,    /**< 9600 baud. */
    NRF_UART_BAUDRATE_14400   = UART_BAUDRATE_BAUDRATE_Baud14400,  /**< 14400 baud. */
    NRF_UART_BAUDRATE_19200   = UART_BAUDRATE_BAUDRATE_Baud19200,  /**< 19200 baud. */
    NRF_UART_BAUDRATE_28800   = UART_BAUDRATE_BAUDRATE_Baud28800,  /**< 28800 baud. */
    NRF_UART_BAUDRATE_31250   = UART_BAUDRATE_BAUDRATE_Baud31250,  /**< 31250 baud. */
    NRF_UART_BAUDRATE_38400   = UART_BAUDRATE_BAUDRATE_Baud38400,  /**< 38400 baud. */
    NRF_UART_BAUDRATE_56000   = UART_BAUDRATE_BAUDRATE_Baud56000,  /**< 56000 baud. */
    NRF_UART_BAUDRATE_57600   = UART_BAUDRATE_BAUDRATE_Baud57600,  /**< 57600 baud. */
    NRF_UART_BAUDRATE_76800   = UART_BAUDRATE_BAUDRATE_Baud76800,  /**< 76800 baud. */
    NRF_UART_BAUDRATE_115200  = UART_BAUDRATE_BAUDRATE_Baud115200, /**< 115200 baud. */
    NRF_UART_BAUDRATE_230400  = UART_BAUDRATE_BAUDRATE_Baud230400, /**< 230400 baud. */
    NRF_UART_BAUDRATE_250000  = UART_BAUDRATE_BAUDRATE_Baud250000, /**< 250000 baud. */
    NRF_UART_BAUDRATE_460800  = UART_BAUDRATE_BAUDRATE_Baud460800, /**< 460800 baud. */
    NRF_UART_BAUDRATE_921600  = UART_BAUDRATE_BAUDRATE_Baud921600, /**< 921600 baud. */
    NRF_UART_BAUDRATE_1000000 = UART_BAUDRATE_BAUDRATE_Baud1M       /**< 1000000 baud. */
} ;

/** @brief Types of UART error masks.  TODO -- name*/
enum class UartErrorMask : uint8_t
{
    NRF_UART_ERROR_OVERRUN_MASK = UART_ERRORSRC_OVERRUN_Msk,    /**< Overrun error. */
    NRF_UART_ERROR_PARITY_MASK      = UART_ERRORSRC_PARITY_Msk,	   /**< Parity error. */
    NRF_UART_ERROR_FRAMING_MASK  = UART_ERRORSRC_FRAMING_Msk,      /**< Framing error. */
    NRF_UART_ERROR_BREAK_MASK	     = UART_ERRORSRC_BREAK_Msk	  /**< Break error. */
};

enum UartHwFlow  
{
    UartHwFcDisabled = UART_CONFIG_HWFC_Disabled, /**< Hardware flow control disabled. */
    UartHwFcEnabled  = UART_CONFIG_HWFC_Enabled  /**< Hardware flow control enabled. */
};

enum UartParity 
{
    UartParityExcluded = UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos, /**< Parity excluded. */
    UartParityIncluded = UART_CONFIG_PARITY_Included << UART_CONFIG_PARITY_Pos    /**< Parity included. */
};

struct UartCommParams 
{
    uint32_t                rxPinNo;	    /**< RX pin number. */
    uint32_t                txPinNo;	    /**< TX pin number. */
    uint32_t                rtsPinNo;	    /**< RTS pin number, only used if flow control is enabled. */
    uint32_t                ctsPinNo;	   /**< CTS pin number, only used if flow control is enabled. */
    UartHwFlow	   hwFlowCntl;  /**< Flow control configuration */
    bool		         useParity;	    // TODO
    UartBaudrate       baudRate;     /**< Baud rate configuration. */
    app_irq_priority_t irqPriority;     /**< Interrupt priority. */
};  

struct UartConfig 
{
    uint32_t	    pselTxd;
    uint32_t	    pselRxd;
    uint32_t	    pselCts;
    uint32_t	    pselRts;
    UartHwFlow     hwFlowCntl;
    UartBaudrate   baudRate;
    UartParity	  parity;
    uint32_t	   interruptFlags;
    app_irq_priority_t irqPriority;
};

// TODO: maybe use template
class UART
{
    public:

    UART(NRF_UART_Type& uartInstance, const UartCommParams& uartCommParams, Callback handle, DataUnit delimiter = '\r');
    void Init();
    void IRQHandler(); // TODO: make it private 


    template<typename ...Args>
    void Print(const char* format, Args... args)
    {
  //      if (xSemaphoreTake(mSemaphore, portMAX_DELAY) != pdTRUE) 
  //      {
  //         // TODO: remove [this code will only be executed when the semaphore is available...]
	 //int m = 4;
	 //m++;
//      }

        char buffer[100] = {0};
        std::snprintf(buffer, sizeof(buffer), format, args...);
        std::strcat(buffer, "\r\n");
        size_t length = strlen(buffer);

        // initiate UART transmission
        StartTX(reinterpret_cast<DataUnit*>(buffer), length);
        
        // block till a UART transmission is done  
        //uint32_t taskNotify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (xSemaphoreTake(mSemaphore, portMAX_DELAY) != pdTRUE) 
        {
            // TODO: remove [this code will only be executed when the semaphore is available...]
	  int m = 4;
	  m++;
	  throw 10;
        }

   //     if (xSemaphoreGive(mSemaphore) != pdTRUE)
   //     {
	  //int y = 0;
	  //y++;
   //     }
    }
   

    private:    

    // methods

    void SetTxRxPins();
    void SetConfigReg();
    void EnableUART();
    void SetBaudRate(UartBaudrate baudRate);
    void EnableInterrupt(uint32_t mask);
    void ConfigureInterrupts();
    bool GetIRQStatus(uint32_t  mask) const;
    uint32_t GetRegValue(uint32_t reg) const;

    uint32_t ReadRXD() const;
    void WriteTXD(uint32_t value) ;

    void StartTX(DataUnit* buffer, size_t length); 

    // variables

    Callback mHandler;
    SemaphoreHandle_t mSemaphore;	// TODO: NEEDED

    NRF_UART_Type& mUARTx;
    UartConfig	         mUARTConfig;

    Fifo<DataUnit> mFifoRx;
    Fifo<DataUnit> mFifoTx;

    DataUnit mDelimiter;
    
    bool mNewInput = false;

    
    //void SetUARTReg(uint32_t reg, uint32_t value) // TODO - change fctn name
    //{
    //    *( (volatile uint32_t*) ( (uint8_t*) mUARTx + (uint32_t) reg) ) = value;

    //    #if __CORTEX_M == 0x04
    //    volatile uint32_t dummy = *( (volatile uint32_t* ) ((uint8_t*) mUARTx + (uint32_t) reg) );
    //    (void)dummy;
    //    #endif
    //}

    void SetUARTReg(uint32_t reg, uint32_t value);  // TODO - change fctn name
    //{
    //    *((volatile uint32_t *) ( (uint8_t*)&mUARTx + (uint32_t)reg) ) = value;

    //    #if __CORTEX_M == 0x04
    //    volatile uint32_t dummy = *( (volatile uint32_t *) ((uint8_t *)&mUARTx + (uint32_t)reg) );
    //    (void)dummy;
    //    #endif
    //}


    void SetUARTTxRx(uint32_t reg, uint32_t value) // TODO - change fctn name
    {
        *((volatile uint32_t *) ( (uint8_t*)&mUARTx + (uint32_t)reg) ) = 1 << value;


        #if __CORTEX_M == 0x04
        volatile uint32_t dummy = *( (volatile uint32_t *) ((uint8_t *)&mUARTx + (uint32_t)reg) );
        (void)dummy;
        #endif
    }
};
