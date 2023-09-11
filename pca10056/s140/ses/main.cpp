



#include "ran.h"	        // TODO HUZZI
#include "mcp9808.h"
<<<<<<< HEAD
#include "SystemTask.h"
#include "UartApp.h"
=======
#include "uart.h"
#include "NotificationManager.h"
>>>>>>> 16dca8d (...)

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
<<<<<<< HEAD

// UART instance
UART uart{*NRF_UART0, commParams, UARTApp::UARTCallback};

// System task
SystemTask systemTask{tempSensor, uart};

=======
UART uart;
NotificationManager notificationManager{uart, &tempSensor};
>>>>>>> 16dca8d (...)

/**@brief Function for application main entry.
 */
int main(void)
{
<<<<<<< HEAD
    //log_init();

    systemTask.Start();
=======
    tempSensor.Start();

       // Initialize modules.
    log_init();
    clock_init();

    ble_init();
    while(1);

    /*
    bool erase_bonds;

 

    // Do not start any interrupt that uses system functions before system initialisation.
    // The best solution is to start the OS before any other initalisation.

#if NRF_LOG_ENABLED
    // Start execution.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
#endif

    // Activate deep sleep mode.
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Initialize modules.
    timers_init();
    buttons_leds_init(&erase_bonds);
>>>>>>> 16dca8d (...)

    ble_init();

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    //for (;;)
    //{
    //    APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    //}
}
