#include "agents/StatusMonitor.h"
#include <Arduino.h>

namespace node
{
void node::StatusMonitor::begin()
{
    m_lastUpdateTime = millis();   
}
void node::StatusMonitor::update()  
{
    unsigned long currentTime = millis();

    if (currentTime - m_lastUpdateTime < MAX_INTERVAL)
    {
        return;
    }

    m_lastUpdateTime = currentTime;
    // Handle the status update event here
    m_eventEmitter.emitEvent(EventType::EVENT_STATUS_UPDATE);
}

bool node::StatusMonitor::hasEvent() const
{
    return m_eventEmitter.hasEvent();
}


Event node::StatusMonitor::getEvent()
{
    return m_eventEmitter.getEvent();
}

}