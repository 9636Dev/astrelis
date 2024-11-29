#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

namespace Astrelis {
    /// @brief The type of the Binding Descriptor, @see DescriptorSetBinding.
    /// @details This is used to determine the type of the descriptor, and how it should be bound.
    enum class DescriptorType {
        Uniform,
        TextureSampler
    };

    class GraphicsPipeline;

    /// @brief A descriptor set binding, used to describe a binding in a descriptor set.
    /// @details This is used to describe a binding in a descriptor set, and is used to create a descriptor set layout.
    /// @note This is not a descriptor set, but a descriptor set binding, which is used to create a descriptor set layout, @see BindingDescriptorSet.
    struct DescriptorSetBinding {
        /// @brief A bitfield of the stages the descriptor is used in.
        /// @details This is used to determine the stages the descriptor is used in, and is used to create a descriptor set layout.
        enum class StageFlags {
            None     = 0,
            Vertex   = 1 << 0,
            Fragment = 1 << 1,
        };

        /// @brief A uniform descriptor, used to describe a uniform in a descriptor set.
        struct Uniform {
            RawRef<UniformBuffer*> Buffer;

            // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
            Uniform(RawRef<UniformBuffer*> buffer) : Buffer(std::move(buffer)) {
            }
        };

        /// @brief A texture sampler descriptor, used to describe a texture sampler in a descriptor set.
        struct TextureSampler {
            RawRef<TextureImage*>             Image;
            RawRef<Astrelis::TextureSampler*> Sampler;

            TextureSampler(RawRef<TextureImage*> image, RawRef<Astrelis::TextureSampler*> sampler)
                : Image(std::move(image)), Sampler(std::move(sampler)) {
            }
        };

        /// @brief The name of the descriptor, may or may not be used.
        /// @details In OpenGL, this is used to get the location of the uniform, in other APIs, this may not be used.
        std::string Name;
        /// @brief The type of the descriptor.
        /// @note This is used to determine the type of the descriptor, ensure that the Uniforms or Textures fields are set correctly.
        DescriptorType Type;
        /// @brief The binding of the descriptor.
        /// @note This would be the texture unit in OpenGL, the binding in Vulkan, etc.
        std::uint32_t Binding;
        /// @brief The stages the descriptor is used in.
        StageFlags Flags;
        /// @brief The number of descriptors, currently only 1 is supported.
        std::uint32_t Count = 1;
        /// @brief The size of the descriptor, used for uniform buffers.
        /// @note This is the size of the uniform buffer, in bytes (including padding), this is used to determine the size of the descriptor.
        std::uint32_t Size = 0;

        /// @brief The uniforms in the descriptor. @see Uniform
        std::vector<Uniform> Uniforms;
        /// @brief The textures in the descriptor. @see TextureSampler
        std::vector<TextureSampler> Textures;

        /// @brief Constructs a descriptor set binding with Uniform type.
        DescriptorSetBinding(std::string name, DescriptorType type, std::uint32_t binding,
            StageFlags flags, std::uint32_t size, std::vector<Uniform> uniforms)
            : Name(std::move(name)), Type(type), Binding(binding), Flags(flags), Size(size),
              Uniforms(std::move(uniforms)) {
        }

        /// @brief Constructs a descriptor set binding with TextureSampler type.
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
    /// @note The mode is not guaranteed to be respected, depending on the implementation.
    class BindingDescriptorSet {
    public:
        enum class Mode {
            PerFrame, // One descriptor set per frame, this may or may not be respected, depending on the implementation
            One, // One descriptor set for all frames
        };
        BindingDescriptorSet()                                             = default;
        virtual ~BindingDescriptorSet()                                    = default;
        BindingDescriptorSet(const BindingDescriptorSet& other)            = default;
        BindingDescriptorSet& operator=(const BindingDescriptorSet& other) = default;
        BindingDescriptorSet(BindingDescriptorSet&& other)                 = default;
        BindingDescriptorSet& operator=(BindingDescriptorSet&& other)      = default;

        /// @brief Initializes the descriptor set.
        /// @param context The graphics context.
        /// @param descriptors The descriptors to bind.
        /// @return True if the descriptor set was successfully initialized, false otherwise.
        [[nodiscard]] virtual bool Init(RefPtr<GraphicsContext>& context,
            const std::vector<DescriptorSetBinding>&             descriptors) = 0;

        /// @brief Destroys the descriptor set.
        virtual void Destroy(RefPtr<GraphicsContext>& context) const = 0;

        /// @brief Binds the descriptor set
        virtual void Bind(
            RefPtr<GraphicsContext>& context, RefPtr<GraphicsPipeline>& pipeline) const = 0;
    };
} // namespace Astrelis
