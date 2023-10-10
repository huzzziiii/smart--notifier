#include "NotifierService.h"
#include "sdk_common.h"

#include <string.h>

static uint32_t custom_value_char_add(StatusInfo * p_cus, const CustInitChar * p_cus_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0; 
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL; 
    char_md.p_sccd_md         = NULL;
		
    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_cus_init->customCharAttr.read_perm;
    attr_md.write_perm = p_cus_init->customCharAttr.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_cus->uuidType;
    ble_uuid.uuid = CUSTOM_VALUE_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint8_t) * 10;

    err_code = sd_ble_gatts_characteristic_add(p_cus->serviceHandle, 
							         &char_md,
							         &attr_char_value,
							         &p_cus->customValueHandle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


uint32_t NotifierService::Init(StatusInfo* statusInfo, CustInitChar* customInitChar, FnPtr<void, CustomEvent*, void*> dataCallback, void* context)
{
    if (!customInitChar || !statusInfo)
    {
        // TODO: error handling
        return NRF_ERROR_NULL;
    }

    statusInfo->connectionHandle = BLE_CONN_HANDLE_INVALID;
    statusInfo->dataHandler = dataCallback;
    statusInfo->context = context;

    ble_uuid128_t baseUUID; 
    memcpy(baseUUID.uuid128, CUSTOM_SERVICE_UUID_BASE, sizeof(CUSTOM_SERVICE_UUID_BASE));

    // add a vendor specific base UUID to the BLE stack's table
    uint32_t retCode =  sd_ble_uuid_vs_add(&baseUUID, &statusInfo->uuidType);
    VERIFY_SUCCESS(retCode);
    
    ble_uuid_t bleUuid;
    bleUuid.type = statusInfo->uuidType;
    bleUuid.uuid = CUSTOM_SERVICE_UUID;

    // Add the Custom Service
    retCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &bleUuid, &statusInfo->serviceHandle);
    VERIFY_SUCCESS(retCode);
    //if (err_code != NRF_SUCCESS)
    //{
    //    return err_code;
    //}

    // Add read/write permissions for the characteristic value attribute
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&customInitChar->customCharAttr.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&customInitChar->customCharAttr.write_perm); //(&cus_init.custom_value_char_attr_md.write_perm);


     // Add Custom Value characteristic
    return custom_value_char_add(statusInfo, customInitChar);

    //return retCode;   // TODO HUZZI

}


// void NotifierService::services_init(void)
//{
//    /* YOUR_JOB: Add code to initialize the services used by the application.*/
//    ret_code_t                         err_code;
//    ble_cus_init_t                     cus_init;

//     // Initialize CUS Service init structure to zero.
//    memset(&cus_init, 0, sizeof(cus_init));
	
//    err_code = ble_cus_init(&m_cus, &cus_init);
//    APP_ERROR_CHECK(err_code);	
//}



/* This code belongs in ble_cus.c*/

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(StatusInfo * p_cus, ble_evt_t const * p_ble_evt)
{
    p_cus->connectionHandle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(StatusInfo *p_cus, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_cus->connectionHandle = BLE_CONN_HANDLE_INVALID;
    //nrf_gpio_pin_set(LED_3);
}

/* This code belongs in ble_cus.c*/

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(StatusInfo* p_cus, const ble_evt_t* p_ble_evt)
{
   if (p_cus == NULL || p_ble_evt == NULL)
   {
       return;
   }

    const ble_gatts_evt_write_t* p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;


    // Check if the handle passed with the event matches the Custom Value Characteristic handle ==> NOTE: CCCD characteristic is 2 bytes
    if ( (p_evt_write->handle == p_cus->customValueHandle.cccd_handle) && (p_evt_write->len == 2))
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
	  int m = 0;
	  m++;
        }
        else
        {
	  int m = 1;
	  m++;
        }
    }
    else if ( (p_evt_write->handle == p_cus->customValueHandle.value_handle) && p_cus->dataHandler != NULL)
    {
        CustomEvent customEvent;
        customEvent.userData.buffer = p_evt_write->data;
        customEvent.userData.bytes = p_evt_write->len;
        customEvent.eventType = BLE_CUS_EVT_USER_DATA_RX;
    
        p_cus->dataHandler(&customEvent, p_cus->context);
    }
}


void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context)
{
    StatusInfo * p_cus = (StatusInfo *) p_context;  // TODO: use static cast
    
    if (p_cus == NULL || p_ble_evt == NULL)
    {
        return;
    }
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
	  on_connect(p_cus, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
	  on_disconnect(p_cus, p_ble_evt);
            break;

       case BLE_GATTS_EVT_WRITE:
	  on_write(p_cus, p_ble_evt);
           break;

        default:
            // No implementation needed.
            break;
    }
}
