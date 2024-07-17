#pragma once

#include <vector>

#include "Layer.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

namespace Nebula
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();
        LayerStack(const LayerStack&)            = delete;
        LayerStack& operator=(const LayerStack&) = delete;
        LayerStack(LayerStack&&)                 = delete;
        LayerStack& operator=(LayerStack&&)      = delete;

        void PushLayer(OwnedPtr<Layer*> layer);
        void PushOverlay(OwnedPtr<Layer*> overlay);
        OwnedPtr<Layer*> PopLayer(RawRef<Layer*>&& layer);
        OwnedPtr<Layer*> PopOverlay(RawRef<Layer*>&& overlay);

        std::vector<OwnedPtr<Layer*>>::iterator begin() { return m_Layers.begin(); }

        std::vector<OwnedPtr<Layer*>>::iterator end() { return m_Layers.end(); }
    private:
        std::int64_t m_LayerInsertIndex = 0;
        std::vector<OwnedPtr<Layer*>> m_Layers;
    };
} // namespace Nebula
