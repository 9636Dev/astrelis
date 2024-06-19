#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include "CommandBuffer.hpp"

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
            std::size_t Binding;
        };

        std::vector<Element> Elements;
    };

    class GraphicsPipeline
    {
    public:
        GraphicsPipeline() = default;
        virtual ~GraphicsPipeline() = default;
        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&) = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&&) = delete;

        virtual void Bind(RefPtr<CommandBuffer>& commandBuffer) = 0;
    };
} // namespace Nebula
