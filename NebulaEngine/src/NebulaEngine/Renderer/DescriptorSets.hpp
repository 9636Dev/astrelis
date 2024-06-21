#pragma once

#include "DescriptorSetLayout.hpp"
#include "GraphicsContext.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"

namespace Nebula
{
    class DescriptorSets
    {
    public:
        DescriptorSets()                                       = default;
        virtual ~DescriptorSets()                              = default;
        DescriptorSets(const DescriptorSets& other)            = delete;
        DescriptorSets& operator=(const DescriptorSets& other) = delete;
        DescriptorSets(DescriptorSets&& other)                 = delete;
        DescriptorSets& operator=(DescriptorSets&& other)      = delete;

        [[nodiscard]] virtual bool Init(RefPtr<GraphicsContext>& context,
                                        RefPtr<DescriptorSetLayout>& layout,
                                        const std::vector<BindingDescriptor>& descriptors)            = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) const                                  = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context, RefPtr<GraphicsPipeline>& pipeline) const = 0;
    };
} // namespace Nebula
