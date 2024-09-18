#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

namespace Astrelis {
    enum class DescriptorType {
        Uniform,
        TextureSampler
    };

    class GraphicsPipeline;

    struct DescriptorSetBinding {
        enum class StageFlags {
            None     = 0,
            Vertex   = 1 << 0,
            Fragment = 1 << 1,
        };

        struct Uniform {
            RawRef<UniformBuffer*> Buffer;

            // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
            Uniform(RawRef<UniformBuffer*> buffer) : Buffer(std::move(buffer)) {
            }
        };

        struct TextureSampler {
            RawRef<TextureImage*>             Image;
            RawRef<Astrelis::TextureSampler*> Sampler;

            TextureSampler(RawRef<TextureImage*> image, RawRef<Astrelis::TextureSampler*> sampler)
                : Image(std::move(image)), Sampler(std::move(sampler)) {
            }
        };

        std::string    Name;
        DescriptorType Type;
        std::uint32_t  Binding;
        StageFlags     Flags;
        std::uint32_t  Count = 1;
        std::uint32_t  Size  = 0;

        std::vector<Uniform>        Uniforms;
        std::vector<TextureSampler> Textures;

        DescriptorSetBinding(std::string name, DescriptorType type, std::uint32_t binding,
            StageFlags flags, std::uint32_t size, std::vector<Uniform> uniforms)
            : Name(std::move(name)), Type(type), Binding(binding), Flags(flags), Size(size),
              Uniforms(std::move(uniforms)) {
        }

        DescriptorSetBinding(std::string name, DescriptorType type, std::uint32_t binding,
            StageFlags flags, std::vector<TextureSampler> textures)
            : Name(std::move(name)), Type(type), Binding(binding), Flags(flags),
              Textures(std::move(textures)) {
        }
    };

    inline DescriptorSetBinding::StageFlags operator|(
        DescriptorSetBinding::StageFlags lhs, DescriptorSetBinding::StageFlags rhs) {
        return static_cast<DescriptorSetBinding::StageFlags>(
            static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
    }

    inline DescriptorSetBinding::StageFlags operator&(
        DescriptorSetBinding::StageFlags lhs, DescriptorSetBinding::StageFlags rhs) {
        return static_cast<DescriptorSetBinding::StageFlags>(
            static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));
    }

    /// @brief A wrapper for a binding descriptor, used to describe a binding in a descriptor set.
    /// @details In vulkan, this represents both the descriptor set layout and the descriptor sets (one per frame).
    class BindingDescriptorSet {
    public:
        enum class Mode {
            PerFrame, // One descriptor set per frame
            One, // One descriptor set for all frames
        };
        BindingDescriptorSet()                                             = default;
        virtual ~BindingDescriptorSet()                                    = default;
        BindingDescriptorSet(const BindingDescriptorSet& other)            = default;
        BindingDescriptorSet& operator=(const BindingDescriptorSet& other) = default;
        BindingDescriptorSet(BindingDescriptorSet&& other)                 = default;
        BindingDescriptorSet& operator=(BindingDescriptorSet&& other)      = default;

        [[nodiscard]] virtual bool Init(RefPtr<GraphicsContext>& context,
            const std::vector<DescriptorSetBinding>&             descriptors) = 0;

        virtual void Destroy(RefPtr<GraphicsContext>& context) const = 0;

        virtual void Bind(
            RefPtr<GraphicsContext>& context, RefPtr<GraphicsPipeline>& pipeline) const = 0;
    };
} // namespace Astrelis
