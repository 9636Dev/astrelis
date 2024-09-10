#include "LayerStack.hpp"

#include <algorithm>

#include "Profiler.hpp"

namespace Astrelis
{
    LayerStack::~LayerStack()
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::LayerStack::~LayerStack");
        for (auto& layer : m_Layers)
        {
            layer.Reset();
        }
    }

    void LayerStack::PushLayer(OwnedPtr<Layer*> layer)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::LayerStack::PushLayer");
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(OwnedPtr<Layer*> overlay)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::LayerStack::PushOverlay");
        m_Layers.emplace_back(std::move(overlay));
    }

    OwnedPtr<Layer*> LayerStack::PopLayer(RawRef<Layer*>&& layer)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::LayerStack::PopLayer");
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (iter != m_Layers.end())
        {
            m_Layers.erase(iter);
            m_LayerInsertIndex--;
            return std::move(*iter);
        }

        return nullptr;
    }

    OwnedPtr<Layer*> LayerStack::PopOverlay(RawRef<Layer*>&& overlay)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::LayerStack::PopOverlay");
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (iter != m_Layers.end())
        {
            m_Layers.erase(iter);
            return std::move(*iter);
        }

        return nullptr;
    }
} // namespace Astrelis
