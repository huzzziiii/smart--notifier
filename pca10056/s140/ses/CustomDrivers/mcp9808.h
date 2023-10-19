#pragma once

 #include "nrf_drv_twi.h"
#include "Publisher.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

enum I2cConfig 
{
    TWI_SCLK		    =         27,
    TWI_SDA		     =	     26,
    TWI_FREQ		   =         NRF_DRV_TWI_FREQ_100K,
    TWI_IRQ_PRIORITY      =	     APP_IRQ_PRIORITY_HIGHEST,
    TWI_CLR_BUS_INIT      =        false
};

class MCP9808 : public Publisher
{
    public:
    MCP9808(); //= default;

    void Start();

    void ParseTempInC();		
    uint16_t GetTempInC() const;
    uint16_t ToCelcius();
    

    // MCP9808 Registers
    enum Register : uint8_t
    {
        CRITICAL		       = 0x4, 
        AMBIENT		      = 0x5,
        MANUFACTURER_ID  = 0x6
        // ...
    };

    uint16_t mTempInC;
    TaskHandle_t mTaskHandle;


    private:
    void Config();

    void Write(uint8_t reg, uint8_t size);
    void Read();

    static void Process(void* instance);
    void Run();

    


    uint8_t mRawTemp[2] = {0};
    nrf_drv_twi_config_t mI2cConfig;	

    // constants
    static constexpr uint32_t BYTES_PER_TRANSACTION =  2;
    static constexpr uint32_t MCP9808_ADDR		    =  0x18;
    static constexpr uint32_t DELAY_MS_PER_READ	       =  4000;
};
