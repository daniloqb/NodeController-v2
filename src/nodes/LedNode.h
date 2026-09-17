#pragma once

#include "nodes/INode.h"
#include "core/Commands.h"

namespace node
{

    class LedNode : public node::INode
    {
    public:
        explicit LedNode(uint8_t pin);
        void begin() override;
        void update() override;
        void reset() override;

        const char* getId() const override;
        bool accepts(const Command &command) const override;

        CommandResult handleCommand(const Command &command) override;
        bool pollEvent(NodeEvent& event) override;

    private:
        bool m_status;
        uint8_t m_pin = false;
        void SetStatus(const bool status);
    };

}