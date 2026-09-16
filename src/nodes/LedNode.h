#pragma once

namespace node
{

    class LedNode
    {
    public:
        void begin();
        void update();
        void setStatus(bool status);
        bool getStatus() const;

    private:
        bool m_status;
    };

}