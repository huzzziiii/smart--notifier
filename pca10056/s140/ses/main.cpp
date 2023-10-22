#include "BLE_Controller.h"	 
#include "mcp9808.h"
#include "SystemTask.h"
#include "UartApp.h"
#include "NotificationManager.h"


#include <stdint.h>
#include <string.h>

// ======== APP START ========

static void log_init(void)
{
    //ret_code_t err_code = NRF_LOG_INIT(NULL);
    //APP_ERROR_CHECK(err_code);

    //NRF_LOG_DEFAULT_BACKENDS_INIT();
}

extern "C"
 {
    void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
    {
        ( void ) pcTaskName;
        ( void ) xTask;

        char buff[100] = {0};
        strncpy(buff, (const char *)pcTaskName, configMAX_TASK_NAME_LEN);

        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    
        while(1);
    }
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

// BLE Notifier Service
NotifierService notifierService;

// Notification Manager
NotificationManager notificationManager{notifierService, uart, &tempSensor};

// System task
SystemTask systemTask{notificationManager, tempSensor, uart};   

// BLE Controller (initializes the Notifier service as well)
BLEController bleController{notifierService, systemTask};    // TODO: use ble service


/**@brief Function for application main entry.
 */
int main(void)
{
    bleController.Init();

    systemTask.Start();

    // Start FreeRTOS scheduler
    vTaskStartScheduler();
}
