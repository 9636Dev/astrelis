#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Renderer/GraphicsPipeline.hpp"

#include "DescriptorSetLayout.hpp"
#include "GraphicsContext.hpp"

namespace Astrelis
{
    class DescriptorSets
    {
    public:
        DescriptorSets()                                       = default;
        virtual ~DescriptorSets()                              = default;
        DescriptorSets(const DescriptorSets& other)            = default;
        DescriptorSets& operator=(const DescriptorSets& other) = default;
        DescriptorSets(DescriptorSets&& other)                 = default;
        DescriptorSets& operator=(DescriptorSets&& other)      = default;

        [[nodiscard]] virtual bool Init(RefPtr<GraphicsContext>& context,
                                        RefPtr<DescriptorSetLayout>& layout,
                                        const std::vector<BindingDescriptor>& descriptors)            = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) const                                  = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context, RefPtr<GraphicsPipeline>& pipeline) const = 0;
    };
} // namespace Astrelis
