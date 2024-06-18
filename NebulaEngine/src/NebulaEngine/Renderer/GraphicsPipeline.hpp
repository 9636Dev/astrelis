#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include "CommandBuffer.hpp"

namespace Nebula
{
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
