#pragma once

#include "bsp.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_bas.h"
#include "ble_hrs.h"
#include "ble_dis.h"

#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"

#include "NotifierService.h"	  
#include "SystemTask.h"


#define DEVICE_NAME				    "Smart-Notifier"                                /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                   "NordicSemiconductor"                   /**< Manufacturer. Will be passed to Device Information Service. */

#define APP_BLE_OBSERVER_PRIO               3                                       /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG                1                                       /**< A tag identifying the SoftDevice BLE configuration. */

#define APP_ADV_INTERVAL                      300                                     /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_DURATION                    18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */


#define MIN_CONN_INTERVAL                   MSEC_TO_UNITS(400, UNIT_1_25_MS)        /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL                   MSEC_TO_UNITS(650, UNIT_1_25_MS)        /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY                       0                                       /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    MSEC_TO_UNITS(4000, UNIT_10_MS)         /**< Connection supervisory time-out (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY      5000                                    /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY       30000                                   /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT        3                                       /**< Number of attempts before giving up the connection parameter negotiation. */



NRF_BLE_GATT_DEF(m_gatt);                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                            /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                          /**< Advertising module instance. */



void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context);
void ble_stack_init();
void bsp_event_handler(bsp_event_t event);
void advertising_start(void * p_erase_bonds);
void conn_params_init(void);
void gap_params_init(void);
void gatt_init(void);
void advertising_init(void);
void services_init(const SystemTask& systemTask);
void ble_init();


class BLEController
{
    public:

    BLEController(NotifierService& notifierSrv, SystemTask& systemTask);
    void Init();
    void ServicesInit();
    //static void DataCallbackAdapter(CustomEvent* customEvent, void* context);
    void DataCallback(CustomEvent* customEvent);

    private:

    NotifierService& mNotifierSrv;
     SystemTask& mSystemTask;
};
