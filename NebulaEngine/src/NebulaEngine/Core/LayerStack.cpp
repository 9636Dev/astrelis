#include "LayerStack.hpp"

#include "NebulaEngine/Core/Profiler.hpp"

#include <algorithm>

namespace Nebula
{
    LayerStack::~LayerStack()
    {
        NEBULA_PROFILE_SCOPE("Nebula::LayerStack::~LayerStack");
        for (auto& layer : m_Layers)
        {
            layer.Reset();
        }
    }

    void LayerStack::PushLayer(OwnedPtr<Layer*> layer)
    {
        NEBULA_PROFILE_SCOPE("Nebula::LayerStack::PushLayer");
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(OwnedPtr<Layer*> overlay)
    {
        NEBULA_PROFILE_SCOPE("Nebula::LayerStack::PushOverlay");
        m_Layers.emplace_back(std::move(overlay));
    }

    OwnedPtr<Layer*> LayerStack::PopLayer(RawRef<Layer*>&& layer)
    {
        NEBULA_PROFILE_SCOPE("Nebula::LayerStack::PopLayer");
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
        NEBULA_PROFILE_SCOPE("Nebula::LayerStack::PopOverlay");
        auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (iter != m_Layers.end())
        {
            m_Layers.erase(iter);
            return std::move(*iter);
        }

        return nullptr;
    }
} // namespace Nebula
