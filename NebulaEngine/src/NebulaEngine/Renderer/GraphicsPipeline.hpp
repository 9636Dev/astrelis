#pragma once

#include "GraphicsContext.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

#include <vector>

namespace Nebula
{
    struct VertexInput
    {
        std::size_t Stride;

        struct Element
        {
            std::size_t Offset;
            /**
            * @brief The number of components in the element, so for example a vec3 would have 3 components.
            **/
            std::size_t Count;
            std::size_t Location;
        };

        std::vector<Element> Elements;
    };

    struct BufferBinding
    {
        std::size_t Binding;
        std::size_t Stride;
        std::vector<VertexInput::Element> Elements;
        bool Instanced;
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

        virtual void Bind(RefPtr<GraphicsContext>& context) = 0;
    };
} // namespace Nebula
