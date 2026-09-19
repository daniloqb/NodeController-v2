#pragma once

#include "NodeControl/INode.h"
#include "NodeControl/Commands.h"
#include "NodeControl/IStatusWriter.h"

namespace node
{

    class LedNode : public node::INode
    {
    public:
        explicit LedNode(uint8_t pin);
        void begin() override;
        void update() override;
        void reset() override;

        char getId() const override;

        CommandResult handleCommand(const Command &command) override;
        bool pollEvent(NodeEvent& event) override;
        void writeStatus(IStatusWriter& writer) const override;

    private:
        bool m_status;
        uint8_t m_pin = 0;
        void SetStatus(const bool status);
    };

}