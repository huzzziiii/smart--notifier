


#include "ran.h"	        // TODO HUZZI
#include "mcp9808.h"
#include "SystemTask.h"
#include "UartApp.h"


#include <stdint.h>
#include <string.h>

// ======== APP START ========

static void log_init(void)
{
    //ret_code_t err_code = NRF_LOG_INIT(NULL);
    //APP_ERROR_CHECK(err_code);

    //NRF_LOG_DEFAULT_BACKENDS_INIT();
}

UartCommParams commParams = 
{
    .rxPinNo       = RX_PIN_NUMBER,
    .txPinNo       = TX_PIN_NUMBER,
    .rtsPinNo      = RTS_PIN_NUMBER,
    .ctsPinNo      = CTS_PIN_NUMBER,
    .hwFlowCntl  = UartHwFcDisabled,
    .useParity     = false,
    .baudRate    = NRF_UART_BAUDRATE_115200,
    .irqPriority	 = APP_IRQ_PRIORITY_LOWEST
};

// Temperature sensor instance
MCP9808 tempSensor;

// UART instance
UART uart {*NRF_UART0, commParams, UARTApp::UARTCallback};

// System task
SystemTask systemTask{tempSensor, uart};

BLEController bleController{systemTask};


/**@brief Function for application main entry.
 */
int main(void)
{
    bleController.Init();
    //ble_init();

    systemTask.Start();

    // Start FreeRTOS scheduler
    vTaskStartScheduler();
}
