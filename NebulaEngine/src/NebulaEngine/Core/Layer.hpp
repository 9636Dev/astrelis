#pragma once

#include <string>

#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Events/Event.hpp"

namespace Nebula
{
    class Layer
    {
    public:
        explicit Layer(std::string debugName = "Layer") : m_DebugName(std::move(debugName)) {}

        virtual ~Layer()               = default;
        Layer(const Layer&)            = delete;
        Layer& operator=(const Layer&) = delete;
        Layer(Layer&&)                 = delete;
        Layer& operator=(Layer&&)      = delete;

        virtual void OnUpdate() {}

        virtual void OnAttach() {}

        virtual void OnDetach() {}

        /**
        * @brief Called when an event is dispatched to the layer.
        * @param event The event that was dispatched.
        */
        virtual void OnEvent(Event& event) { NEBULA_UNUSED(event); }

        virtual void OnUIRender() {}

        [[nodiscard]] const std::string& GetName() const { return m_DebugName; }
    private:
        std::string m_DebugName;
    };
} // namespace Nebula
