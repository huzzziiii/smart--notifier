


#include "ran.h"	        // TODO HUZZI
#include "mcp9808.h"
#include "SystemTask.h"

#include <stdint.h>
#include <string.h>

// ======== APP START ========

static void log_init(void)
{
    //ret_code_t err_code = NRF_LOG_INIT(NULL);
    //APP_ERROR_CHECK(err_code);

    //NRF_LOG_DEFAULT_BACKENDS_INIT();
}


void UARTHandler(Fifo<DataUnit> fifo)
{
    int m = 0;
    m++;
}

UartCommParams commParams = 
{
    .rxPinNo = RX_PIN_NUMBER,
    .txPinNo = TX_PIN_NUMBER,
    .rtsPinNo = RTS_PIN_NUMBER,
    .ctsPinNo = CTS_PIN_NUMBER,
    .hwFlowCntl = UartHwFcDisabled,
    .useParity = false,
    .baudRate = NRF_UART_BAUDRATE_115200,
    .irqPriority = APP_IRQ_PRIORITY_LOWEST
};

MCP9808 tempSensor;

// UART instance
UART uart{*NRF_UART0, commParams, UARTHandler}; //, UartApp::UartHandler};

// System task
SystemTask systemTask{tempSensor, uart};


/**@brief Function for application main entry.
 */
int main(void)
{
    //log_init();

    systemTask.Start();

    ble_init();

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    //for (;;)
    //{
    //    APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    //}
}
