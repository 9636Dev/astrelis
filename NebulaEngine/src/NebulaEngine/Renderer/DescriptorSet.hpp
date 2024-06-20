#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "GraphicsContext.hpp"

namespace Nebula
{
    class DescriptorSet
    {
    public:
        DescriptorSet() = default;
        virtual ~DescriptorSet() = default;
        DescriptorSet(const DescriptorSet &other) = delete;
        DescriptorSet &operator=(const DescriptorSet &other) = delete;
        DescriptorSet(DescriptorSet &&other) = delete;
        DescriptorSet &operator=(DescriptorSet &&other) = delete;

        virtual void Bind(RefPtr<GraphicsContext>& context) const = 0;
    };
} // namespace Nebula
