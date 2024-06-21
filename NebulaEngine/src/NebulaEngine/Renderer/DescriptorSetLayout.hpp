#pragma once

#include <cstdint>
#include <string>

#include "TextureImage.hpp"
#include "TextureSampler.hpp"

namespace Nebula
{
    struct UniformDescriptor
    {
        std::string Name;
        std::uint32_t Binding;
        std::uint32_t Count; // UBOs are just 1
        std::uint32_t Size;
    };

    struct SamplerDescriptor
    {
        std::string Name;
        std::uint32_t Binding;
        std::uint32_t Count;
        RefPtr<TextureImage> Texture;
        RefPtr<TextureSampler> Sampler;
    };

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout() = default;
        virtual ~DescriptorSetLayout() = default;
        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout(DescriptorSetLayout&&) = delete;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&) = delete;
    };
} // namespace Nebula
