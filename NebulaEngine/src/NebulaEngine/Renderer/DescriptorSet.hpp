#pragma once

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
    };
} // namespace Nebula
