#pragma once

#include "ble_srv_common.h"
#include "SystemTask.h"

#include <cstdint>


static constexpr uint8_t    CUSTOM_SERVICE_UUID_BASE[] = {0xBC, 0x8A, 0xBF, 0x45, 0xCA, 0x05, 0x50, 0xBA, 0x40, 0x42, 0xB0, 0x00, 0xC9, 0xAD, 0x64, 0xF3};
static constexpr uint16_t  CUSTOM_SERVICE_UUID	        =  0x1400;
static constexpr uint16_t  CUSTOM_VALUE_CHAR_UUID    =  0x1401;


/**< Universally unique service identifier. */
static ble_uuid_t m_adv_uuids_cust[]  =                                     
{
    {CUSTOM_SERVICE_UUID, BLE_UUID_TYPE_VENDOR_BEGIN}
};


#define BLE_CUS_DEF(_name)                                                                       \
static StatusInfo _name;								        \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                 \
				         BLE_HRS_BLE_OBSERVER_PRIO,                 \
				         ble_cus_on_ble_evt, &_name)


				         
struct BLEUserData
{
    const uint8_t* buffer;	   /**< pointer to the buffer containing user data */
    uint8_t bytes;		       /**< number of received bytes */
};

enum EventType 
{
    BLE_CUS_EVT_DISCONNECTED,
    BLE_CUS_EVT_CONNECTED,
    BLE_CUS_EVT_USER_DATA_RX
};

struct CustomEvent
{
    EventType eventType;
    BLEUserData userData;
};


template <typename Ret, typename... Args>
using FnPtr = Ret(*)(Args...);
using DataCallbackFn = FnPtr<void, CustomEvent*, void*>;	/**< alias for a function pointer to the user-provided data callback for processing BLE data


/**@brief Custom Service init structure. This contains all options and data needed for initialization of the service. */
struct CustInitChar
{
    uint8_t				  initCustomValue;      /**< Initial custom value */
    ble_srv_cccd_security_mode_t  customCharAttr;	      /**< Initial security level for Custom characteristics attribute */
};

 /**@brief Custom Service structure. This contains various status information for the service. */
struct StatusInfo
{
    uint8_t			       uuidType;
    uint16_t			      serviceHandle;	         /**< Handle of the custom service. */
    ble_gatts_char_handles_t	    customValueHandle;	 /**< Handles related to the Custom Value characteristic. */
    uint16_t			      connectionHandle;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    DataCallbackFn		         dataHandler;		  /**< Callback for data handling */
    void*				       context;			    /**< User-provided context that will be passed back to the callback i.e, an object instance */
};


class NotifierService
{
    public:

    uint32_t Init(StatusInfo* statusInfo, CustInitChar* customInitChar, DataCallbackFn dataCallback, void* context);    
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
