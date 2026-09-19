#pragma once
#include <Arduino.h>
#include <NodeControl/NodeControl.h>

namespace node
{
    class ButtonNode : public INode
    {
    public:
        ButtonNode(uint8_t pin, bool pullDown = false);
        void begin() override;
        void update() override;
        void reset() override;

        char getId() const override;

        CommandResult handleCommand(const Command &command) override;

        bool pollEvent(NodeEvent &event) override;

        void writeStatus(IStatusWriter &statusWriter) const override;

    private:
        uint8_t m_pin;
        bool m_status;
        bool m_eventPending = false;
        bool m_pullDown;
    };
}
