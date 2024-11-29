#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Renderer/BindingDescriptor.hpp"
#include "Astrelis/Scene/Material.hpp"

#include <vector>

#include "GraphicsContext.hpp"

namespace Astrelis {
    /// @brief A struct that represents the inputs for a graphics pipeline ( + shaders).
    struct VertexInput {
        /// @brief The type of the vertex element.
        enum class VertexType {
            Float,
            Int,
            UInt,
        };

        /// @brief A struct that represents one element of a vertex input.
        struct Element {
            /// @brief The type of the element.
            VertexType Type;
            /// @brief The offset of the element in the vertex.
            /// @note Can be easily retried using the offsetof macro.
            std::size_t Offset;
            /// @brief The number of components in the element, so for example a vec3 would have 3 components.
            std::size_t Count;
            /// @brief The location of the element in the shader.
            std::size_t Location;
        };
    };

    /// @brief A struct that represents a buffer binding in a graphics pipeline.
    /// @note The binding is only relevant for supported APIs, such as Vulkan.
    struct BufferBinding {
        /// @brief The binding of the buffer.
        std::size_t Binding;
        /// @brief The stride of the buffer.
        std::size_t Stride;
        /// @brief The input rate of the buffer.
        std::vector<VertexInput::Element> Elements;
        bool                              Instanced;
    };

    // We do this so that we dont need to copy the shaders
    struct PipelineShaders {
        /// @brief The vertex shader.
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
        CompiledShader& Vertex;
        /// @brief The fragment shader.
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
        CompiledShader& Fragment;

        PipelineShaders(CompiledShader& vertex, CompiledShader& fragment)
            : Vertex(vertex), Fragment(fragment) {
        }
    };

    /// @brief An enum that represents the type of a pipeline.
    /// @note Some of the pipelines maybe overlapping, depending on the API.
    enum class PipelineType {
        /// @brief A pipeline that is used for rendering the main scene.
        Graphics,
        /// @brief A pipeline that is used for rendering the overlay.
        Overlay,
        /// @brief A pipeline that is used for rendering the blit pass.
        Main,
    };

    /// @brief A class that represents a graphics pipeline.
    class GraphicsPipeline {
    public:
        GraphicsPipeline()                                   = default;
        virtual ~GraphicsPipeline()                          = default;
        GraphicsPipeline(const GraphicsPipeline&)            = default;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = default;
        GraphicsPipeline(GraphicsPipeline&&)                 = default;
        GraphicsPipeline& operator=(GraphicsPipeline&&)      = default;

        virtual bool Init(RefPtr<GraphicsContext>& context, PipelineShaders& shaders,
            std::vector<BufferBinding>&                 bindings,
            std::vector<RawRef<BindingDescriptorSet*>>& descriptors, PipelineType type) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context)                          = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context)                             = 0;
    };
} // namespace Astrelis
