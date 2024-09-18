#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

namespace Astrelis {
    enum class DescriptorType {
        Uniform,
        TextureSampler
    };

    struct DescriptorLayoutBinding {
        enum class StageFlags {
            None     = 0,
            Vertex   = 1 << 0,
            Fragment = 1 << 1,
        };

        DescriptorType Type;
        std::uint32_t  Binding;
        std::uint32_t  Count;
        StageFlags     Flags;

        DescriptorLayoutBinding(
            DescriptorType type, std::uint32_t binding, std::uint32_t count, StageFlags flags)
            : Type(type), Binding(binding), Count(count), Flags(flags) {
        }
    };

    inline DescriptorLayoutBinding::StageFlags operator|(
        DescriptorLayoutBinding::StageFlags lhs, DescriptorLayoutBinding::StageFlags rhs) {
        return static_cast<DescriptorLayoutBinding::StageFlags>(
            static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
    }

    inline DescriptorLayoutBinding::StageFlags operator&(
        DescriptorLayoutBinding::StageFlags lhs, DescriptorLayoutBinding::StageFlags rhs) {
        return static_cast<DescriptorLayoutBinding::StageFlags>(
            static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));
    }

    struct BindingDescriptor {
        struct Uniform {
            RawRef<UniformBuffer*> Buffer;

            explicit Uniform(RawRef<UniformBuffer*> buffer) : Buffer(std::move(buffer)) {
            }
        };

        struct TextureSampler {
            RawRef<TextureImage*>             Image;
            RawRef<Astrelis::TextureSampler*> Sampler;

            TextureSampler(RawRef<TextureImage*> image, RawRef<Astrelis::TextureSampler*> sampler)
                : Image(std::move(image)), Sampler(std::move(sampler)) {
            }
        };

        std::string                   Name;
        std::uint32_t                 Binding;
        std::uint32_t                 Count; // UBOs are just 1
        std::uint32_t                 Size = 0;
        std::optional<Uniform>        Uniform;
        std::optional<TextureSampler> Texture;

        BindingDescriptor(std::string name, std::uint32_t binding, std::uint32_t count)
            : Name(std::move(name)), Binding(binding), Count(count) {
        }

        // We use templates and a variadic function that passes to Uniform or Texture
        static BindingDescriptor CreateUniform(std::string name, std::uint32_t binding,
            std::uint32_t size, RawRef<UniformBuffer*> buffer) {
            BindingDescriptor descriptor(std::move(name), binding, 1);
            descriptor.Size = size;
            descriptor.Uniform.emplace(std::move(buffer));
            return descriptor;
        }

        static BindingDescriptor CreateTextureSampler(std::string name, std::uint32_t binding,
            RawRef<TextureImage*> image, RawRef<Astrelis::TextureSampler*> sampler) {
            BindingDescriptor descriptor(std::move(name), binding, 1);
            descriptor.Texture.emplace(std::move(image), std::move(sampler));
            return descriptor;
        }
    };

    class DescriptorSetLayout {
    public:
        DescriptorSetLayout()                                      = default;
        virtual ~DescriptorSetLayout()                             = default;
        DescriptorSetLayout(const DescriptorSetLayout&)            = default;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;
        DescriptorSetLayout(DescriptorSetLayout&&)                 = default;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&)      = default;

        [[nodiscard]] virtual bool Init(RefPtr<GraphicsContext>& context,
            const std::vector<DescriptorLayoutBinding>&          descriptors)               = 0;
        virtual void               Destroy(RefPtr<GraphicsContext>& context) const = 0;
    };
} // namespace Astrelis
