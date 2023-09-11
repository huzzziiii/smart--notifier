#include "mcp9808.h"


#include "nrf_drv_twi.h"
#include "nrfx_twi.h"
#include "nrf_delay.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0		// TODO HUZZI

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static bool volatile m_xfer_done = false;        // TODO HUZZI: rmv


MCP9808::MCP9808() : Publisher(Publisher::Category::TEMPERATURE)
{
    // TODO HUZZI
}

 /**
 * @brief TWI events handler. Called within the TWI interrupt handler
 */
void twiHandler(nrf_drv_twi_evt_t const * p_event, void * p_context)	   
{
    MCP9808* mcp9808 = static_cast<MCP9808*>(p_context);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
	  if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX)
	  {
	      m_xfer_done = true;	// TODO HUZZI: use sync method
	  }
	  else if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
	  {
	      //m_rx_done = true;     // TODO HUZZI: use sync method
	      vTaskNotifyGiveFromISR(mcp9808->mTaskHandle, &xHigherPriorityTaskWoken);

	  }
	  break;

        default:
	  break;
    }
}

uint16_t MCP9808::ToCelcius()
{
    uint16_t tempInCelcius = 0;
    uint8_t upperByte = mRawTemp[0] & 0x1F;
    uint8_t lowerByte = mRawTemp[1];
    uint8_t isSigned = upperByte & 0x10;

    if (isSigned)
    {
        upperByte &= 0x0F;
        tempInCelcius = 256 - (upperByte << 4 | lowerByte >> 4);
    }
    else
    {
        tempInCelcius = upperByte << 4 | lowerByte >> 4;
    }
    return tempInCelcius;
}

void MCP9808::Start()
{
    mI2cConfig = 
    {
       .scl                       = TWI_SCLK,
       .sda                     = TWI_SDA,
       .frequency           = static_cast<nrf_drv_twi_frequency_t> (TWI_FREQ),	  
       .interrupt_priority = TWI_IRQ_PRIORITY,
       .clear_bus_init     = false
    };
    
    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &mI2cConfig, twiHandler, this);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);	

    Write(Register::AMBIENT, 1);
    return; // TODO

    // TODO: create a task      --  think about stack size!
    if (xTaskCreate(MCP9808::Process, "Process", 100, this, 0, &mTaskHandle) != pdPASS)	
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    } 
}

void MCP9808::Process(void* instance)
{
    //NRF_LOG_INFO("MCP9808 task started!");
    auto* app = static_cast<MCP9808*>(instance);
    app->Run();
}

void MCP9808::Run()
{
    while(true)
    {
        Read();
        
        uint32_t taskNotify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //uint16_t celc = ToCelcius();

        Notify();
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_PER_READ));
    }
}


void MCP9808::Write(uint8_t reg, uint8_t size)
{
    ret_code_t errorCode = nrf_drv_twi_tx(&m_twi, MCP9808_ADDR, &reg, size, false);
    APP_ERROR_CHECK(errorCode);
    while(!m_xfer_done);	  // TODO HUZZI change to sync
}

void MCP9808::Read()
{
    ret_code_t errorCode = nrf_drv_twi_rx(&m_twi, MCP9808_ADDR, mRawTemp, 2);
    APP_ERROR_CHECK(errorCode);
}
