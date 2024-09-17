#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Scene/Material.hpp"

#include <vector>

#include "DescriptorSetLayout.hpp"
#include "GraphicsContext.hpp"

namespace Astrelis {
    struct VertexInput {
        enum class VertexType {
            Float,
            Int,
            UInt,
        };

        struct Element {
            VertexType  Type;
            std::size_t Offset;
            /**
            * @brief The number of components in the element, so for example a vec3 would have 3 components.
            **/
            std::size_t Count;
            std::size_t Location;
        };

        std::size_t          Stride;
        std::vector<Element> Elements;
    };

    struct BufferBinding {
        std::size_t                       Binding;
        std::size_t                       Stride;
        std::vector<VertexInput::Element> Elements;
        bool                              Instanced;
    };

    // We do this so that we dont need to copy the shaders
    struct PipelineShaders {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
        CompiledShader& Vertex;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
        CompiledShader& Fragment;

        PipelineShaders(CompiledShader& vertex, CompiledShader& fragment)
            : Vertex(vertex), Fragment(fragment) {
        }
    };

    enum class PipelineType {
        Graphics,
        Overlay,
        Main, // Blit render pass
    };

    class GraphicsPipeline {
    public:
        GraphicsPipeline()                                   = default;
        virtual ~GraphicsPipeline()                          = default;
        GraphicsPipeline(const GraphicsPipeline&)            = default;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = default;
        GraphicsPipeline(GraphicsPipeline&&)                 = default;
        GraphicsPipeline& operator=(GraphicsPipeline&&)      = default;

        virtual bool Init(RefPtr<GraphicsContext>& context, PipelineShaders& shaders,
            std::vector<BufferBinding>& bindings, std::vector<RefPtr<DescriptorSetLayout>>& layouts,
            PipelineType type)                                 = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context)    = 0;
    };
} // namespace Astrelis
