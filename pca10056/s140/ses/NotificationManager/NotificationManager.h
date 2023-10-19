#pragma once

#include "Publisher.h"
#include "Subscriber.h"
#include "uart.h"
#include "NotifierService.h"

struct Notification
{
    char msg[100];
    Publisher::Category category;
};

class NotificationManager : public Subscriber
{
    public:

    template<typename ...Args>
    NotificationManager(NotifierService& bleNotifierSrv, UART& uart, Args ...publishers) : mBLENotifierSrv(bleNotifierSrv), 
													  	    mUART(uart)
    {
        ((mPublishers[idx] = publishers), ...);
        mPublishers[idx++]->Subscribe(this);
    }
    
    void Update(Publisher*) override;
    Notification MakeNotification(char* msg, Publisher::Category category);
    void PushNotification(Notification notification);

    private:

    NotifierService& mBLENotifierSrv;
    UART& mUART;
    
    uint8_t idx = 0;
    Fifo<Notification> mNotifications;
    static constexpr uint8_t PUBLISHERS_MAX_SIZE = 10;
    Publisher* mPublishers[PUBLISHERS_MAX_SIZE] = {nullptr};
};
