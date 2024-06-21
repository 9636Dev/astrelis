#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "GraphicsContext.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"

namespace Nebula
{
    class DescriptorSets
    {
    public:
        DescriptorSets() = default;
        virtual ~DescriptorSets() = default;
        DescriptorSets(const DescriptorSets &other) = delete;
        DescriptorSets &operator=(const DescriptorSets &other) = delete;
        DescriptorSets(DescriptorSets &&other) = delete;
        DescriptorSets &operator=(DescriptorSets &&other) = delete;

        virtual void Bind(RefPtr<GraphicsContext>& context, RefPtr<GraphicsPipeline>& pipeline) const = 0;
    };
} // namespace Nebula
