#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

namespace Nebula
{
    struct BindingDescriptor
    {
        enum class Type
        {
            Uniform,
            TextureSampler
        };
        std::string Name;
        std::uint32_t Binding;
        std::uint32_t Count; // UBOs are just 1
        std::uint32_t Size = 0;
        RefPtr<UniformBuffer> Buffer;
        RefPtr<TextureImage> Texture;
        RefPtr<TextureSampler> Sampler;
        Type DescriptorType;

        BindingDescriptor(std::string name,
                          std::uint32_t binding,
                          std::uint32_t count,
                          std::uint32_t size,
                          RefPtr<UniformBuffer> buffer) :
            Name(std::move(name)),
            Binding(binding),
            Count(count),
            Size(size),
            Buffer(std::move(buffer)),
            DescriptorType(Type::Uniform)
        {
        }

        BindingDescriptor(std::string name,
                          std::uint32_t binding,
                          std::uint32_t count,
                          RefPtr<TextureImage> texture,
                          RefPtr<TextureSampler> sampler) :
            Name(std::move(name)),
            Binding(binding),
            Count(count),
            Texture(std::move(texture)),
            Sampler(std::move(sampler)),
            DescriptorType(Type::TextureSampler)
        {
        }

        static BindingDescriptor
            Uniform(std::string name, std::uint32_t binding, std::uint32_t size, RefPtr<UniformBuffer> buffer)
        {
            return BindingDescriptor(std::move(name), binding, 1, size, std::move(buffer));
        }

        static BindingDescriptor TextureSampler(std::string name,
                                                std::uint32_t binding,
                                                RefPtr<TextureImage> texture,
                                                RefPtr<TextureSampler> sampler)
        {
            return BindingDescriptor(std::move(name), binding, 1, std::move(texture), std::move(sampler));
        }
    };

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout()                                      = default;
        virtual ~DescriptorSetLayout()                             = default;
        DescriptorSetLayout(const DescriptorSetLayout&)            = default;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;
        DescriptorSetLayout(DescriptorSetLayout&&)                 = default;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&)      = default;

        [[nodiscard]] virtual bool Init(RefPtr<GraphicsContext>& context,
                                        const std::vector<BindingDescriptor>& descriptors) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) const                       = 0;
    };
} // namespace Nebula
