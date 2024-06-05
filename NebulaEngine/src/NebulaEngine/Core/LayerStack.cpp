#include "LayerStack.hpp"

#include <algorithm>

namespace Nebula
{
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (iter != m_Layers.end())
        {
            m_Layers.erase(iter);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (iter != m_Layers.end())
        {
            m_Layers.erase(iter);
        }
    }
} // namespace Nebula
