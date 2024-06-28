#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include "DescriptorSetLayout.hpp"
#include "GraphicsContext.hpp"

#include <vector>

namespace Nebula
{
    struct VertexInput
    {
        enum class VertexType
        {
            Float,
            Int,
            UInt,
        };

        struct Element
        {
            VertexType Type;
            std::size_t Offset;
            /**
            * @brief The number of components in the element, so for example a vec3 would have 3 components.
            **/
            std::size_t Count;
            std::size_t Location;
        };

        std::size_t Stride;
        std::vector<Element> Elements;
    };

    struct BufferBinding
    {
        std::size_t Binding;
        std::size_t Stride;
        std::vector<VertexInput::Element> Elements;
        bool Instanced;
    };

    struct PipelineShaders
    {
        // TODO: Something better than this, current just paths to the files.
        std::string Vertex;
        std::string Fragment;
    };

    class GraphicsPipeline
    {
    public:
        GraphicsPipeline()                                   = default;
        virtual ~GraphicsPipeline()                          = default;
        GraphicsPipeline(const GraphicsPipeline&)            = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&)                 = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&&)      = delete;

        virtual bool Init(RefPtr<GraphicsContext>& context,
                          PipelineShaders& shaders,
                          std::vector<BufferBinding>& bindings,
                          std::vector<RefPtr<DescriptorSetLayout>>& layouts) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context)               = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context)                  = 0;
    };
} // namespace Nebula
