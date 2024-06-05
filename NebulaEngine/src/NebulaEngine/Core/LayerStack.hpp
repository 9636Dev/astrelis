#pragma once

#include <vector>

#include "Layer.hpp"

namespace Nebula
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();
        LayerStack(const LayerStack&) = delete;
        LayerStack& operator=(const LayerStack&) = delete;
        LayerStack(LayerStack&&) = delete;
        LayerStack& operator=(LayerStack&&) = delete;

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::int64_t m_LayerInsertIndex = 0;
        std::vector<Layer*> m_Layers;
    };
} // namespace Nebula
