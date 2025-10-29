#pragma once
#include "Jade/Core/Core.h"
#include "Jade/Events/Event.h"

namespace Jade
{
    class JADE_API Layer
    {
    public:
        Layer(const std::string& debugName = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}