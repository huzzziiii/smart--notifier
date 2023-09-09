#pragma once

#include "ble_srv_common.h"

#include <cstdint>

// TODO HUZZI: move elsewhere
template <typename Ret, typename... Args>
using FnPtr = Ret(*)(Args...);


static constexpr uint8_t CUSTOM_SERVICE_UUID_BASE[] = {0xBC, 0x8A, 0xBF, 0x45, 0xCA, 0x05, 0x50, 0xBA, 0x40, 0x42, 0xB0, 0x00, 0xC9, 0xAD, 0x64, 0xF3};
static constexpr uint16_t  CUSTOM_SERVICE_UUID	  =  0x1400;
static constexpr uint16_t  CUSTOM_VALUE_CHAR_UUID    =  0x1401;

#define BLE_UUID_TYPE_VENDOR_BEGIN	         0x02		


static ble_uuid_t m_adv_uuids_cust[]          =                                          /**< Universally unique service identifier. */
{
    {CUSTOM_SERVICE_UUID, BLE_UUID_TYPE_VENDOR_BEGIN}
};


//    static ble_uuid_t m_adv_uuids[] =                                   /**< Universally unique service identifiers. */
//{
//    {BLE_UUID_HEART_RATE_SERVICE, BLE_UUID_TYPE_BLE},
//    {BLE_UUID_BATTERY_SERVICE, BLE_UUID_TYPE_BLE},
//    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
//};

/**@brief   Macro for defining a ble_cus instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
//#define BLE_CUS_DEF(_name)                                                                          \
//static StatusInfo _name;  



#define BLE_CUS_DEF(_name)                                                                          \
static StatusInfo _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_HRS_BLE_OBSERVER_PRIO,                                                     \
                     ble_cus_on_ble_evt, &_name)


enum EventType 
{
    BLE_CUS_EVT_DISCONNECTED,
    BLE_CUS_EVT_CONNECTED
};

struct CustomEvent
{
    EventType eventType;
};



/**@brief Custom Service init structure. This contains all options and data needed for initialization of the service. */
struct CustInitChar
{
    uint8_t				  initCustomValue;      /**< Initial custom value */
    ble_srv_cccd_security_mode_t  customCharAttr;	      /**< Initial security level for Custom characteristics attribute */
};

 /**@brief Custom Service structure. This contains various status information for the service. */
struct StatusInfo
{
    uint8_t			     uuidType;
    uint16_t			    serviceHandle;		     /**< Handle of the custom service. */
    ble_gatts_char_handles_t   customValueHandle;	       /**< Handles related to the Custom Value characteristic. */
    uint16_t			   connectionHandle;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    FnPtr<void, StatusInfo*, CustomEvent*> dataHandler;       /**< Callback for data handling */
};


class NotifierService
{
    public:

    uint32_t Init(StatusInfo* statusInfo, CustInitChar* customInitChar, FnPtr<void, StatusInfo*, CustomEvent*> fnPtr);

    private:
    //FnPtr<void, StatusInfo*, CustomEvent*> mDataCallback;


};


/* This code belongs in ble_cus.h*/

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);