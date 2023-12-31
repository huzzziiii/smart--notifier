#pragma once

#include "NotificationManager.h"
#include "uart.h"
#include "mcp9808.h"
#include "NotifierService.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#define ENUM_ENTRY(x) { SystemTask::Message::x, #x }

class NotificationManager;

class SystemTask
{
    public:

    enum class Message : uint8_t
    {
        SUBSCRIBE_TEMP_NOTIFICATIONS,
        UNSUBSCRIBE_TEMP_NOTIFICATIONS,
        INVALID
    };

    SystemTask(NotificationManager& notificationManager, MCP9808& mcp9808, UART& uart);
    void Start();
    void PushMessage(SystemTask::Message message, bool fromISR = true);
    static Message GetMessage(DataUnit* str);

    private:

    static void Process(void* instance);
    void Run();

    NotificationManager& mNotificationManager;
    MCP9808& mMcp9808;
    UART& mUART;

    static QueueHandle_t mTaskQueue;
    TaskHandle_t mTaskHandle{NULL};

    struct MsgLookup
    {
        Message msg;
        const char* msgStr;
    };

    static constexpr MsgLookup msgLookup[3] = 
    {
        ENUM_ENTRY(SUBSCRIBE_TEMP_NOTIFICATIONS),
        ENUM_ENTRY(UNSUBSCRIBE_TEMP_NOTIFICATIONS),
        ENUM_ENTRY(INVALID),
    };

};
