#include "GraphicsPipeline.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Renderer/GraphicsPipeline.hpp"

#include "CommandBuffer.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "RenderPass.hpp"

namespace Astrelis::Vulkan {
    static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule = nullptr;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create shader module!");
            return VK_NULL_HANDLE;
        }

        return shaderModule;
    }

    static VkFormat InputCountToFormat(std::size_t count, VertexInput::VertexType type) {
        switch (type) {
        case VertexInput::VertexType::Float:
        case VertexInput::VertexType::Int:
        case VertexInput::VertexType::UInt:
            break;
        default:
            ASTRELIS_CORE_ASSERT(false, "Invalid VertexType!");
        }

        switch (count) {
        case 1:
            switch (type) {
            case VertexInput::VertexType::Float:
                return VK_FORMAT_R32_SFLOAT;
            case VertexInput::VertexType::Int:
                return VK_FORMAT_R32_SINT;
            case VertexInput::VertexType::UInt:
                return VK_FORMAT_R32_UINT;
            }
        case 2:
            switch (type) {
            case VertexInput::VertexType::Float:
                return VK_FORMAT_R32G32_SFLOAT;
            case VertexInput::VertexType::Int:
                return VK_FORMAT_R32G32_SINT;
            case VertexInput::VertexType::UInt:
                return VK_FORMAT_R32G32_UINT;
            }
        case 3:
            switch (type) {
            case VertexInput::VertexType::Float:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case VertexInput::VertexType::Int:
                return VK_FORMAT_R32G32B32_SINT;
            case VertexInput::VertexType::UInt:
                return VK_FORMAT_R32G32B32_UINT;
            }
        case 4:
            switch (type) {
            case VertexInput::VertexType::Float:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VertexInput::VertexType::Int:
                return VK_FORMAT_R32G32B32A32_SINT;
            case VertexInput::VertexType::UInt:
                return VK_FORMAT_R32G32B32A32_UINT;
            }
        default:
            return VK_FORMAT_UNDEFINED;
        }
    }

    bool GraphicsPipeline::Init(LogicalDevice& device, VkExtent2D extent, RenderPass& renderPass,
        PipelineShaders& shaders, std::vector<BufferBinding>& bindings,
        std::vector<DescriptorSetLayout>& layouts, VkSampleCountFlagBits samples) {
        VkShaderModule vertexShaderModule =
            CreateShaderModule(device.GetHandle(), shaders.Vertex.Vulkan.Data);
        VkShaderModule fragmentShaderModule =
            CreateShaderModule(device.GetHandle(), shaders.Fragment.Vulkan.Data);

        VkPipelineShaderStageCreateInfo vertexShaderStageInfo {};
        vertexShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = vertexShaderModule;
        vertexShaderStageInfo.pName  = shaders.Vertex.Vulkan.Entrypoint;

        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo {};
        fragmentShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module = fragmentShaderModule;
        fragmentShaderStageInfo.pName  = shaders.Fragment.Vulkan.Entrypoint;

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
            vertexShaderStageInfo, fragmentShaderStageInfo};

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo {};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateCreateInfo.pDynamicStates    = dynamicStates.data();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        std::vector<VkVertexInputBindingDescription>   bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        bindingDescriptions.resize(bindings.size());
        for (std::size_t i = 0; i < bindings.size(); i++) {
            bindingDescriptions[i].binding = static_cast<uint32_t>(i);
            bindingDescriptions[i].stride  = bindings[i].Stride;
            bindingDescriptions[i].inputRate =
                bindings[i].Instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;

            attributeDescriptions.reserve(bindings[i].Elements.size());
            for (std::size_t j = 0; j < bindings[i].Elements.size(); j++) {
                VkVertexInputAttributeDescription attributeDescription {};
                attributeDescription.binding  = static_cast<uint32_t>(i);
                attributeDescription.location = bindings[i].Elements[j].Location;
                attributeDescription.format =
                    InputCountToFormat(bindings[i].Elements[j].Count, bindings[i].Elements[j].Type);
                attributeDescription.offset = bindings[i].Elements[j].Offset;
                attributeDescriptions.push_back(attributeDescription);
            }
        }

        vertexInputInfo.vertexBindingDescriptionCount =
            static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
        inputAssembly.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport {};
        viewport.x        = 0.0F;
        viewport.y        = 0.0F;
        viewport.width    = static_cast<float>(extent.width);
        viewport.height   = static_cast<float>(extent.height);
        viewport.minDepth = 0.0F;
        viewport.maxDepth = 1.0F;

        VkRect2D scissor {};
        scissor.offset = {0, 0};
        scissor.extent = extent;

        VkPipelineViewportStateCreateInfo viewportState {};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer {};
        rasterizer.sType            = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth               = 1.0F;
        rasterizer.cullMode                = VK_CULL_MODE_NONE;
        rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;

        rasterizer.depthBiasEnable         = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0F;
        rasterizer.depthBiasClamp          = 0.0F;
        rasterizer.depthBiasSlopeFactor    = 0.0F;

        VkPipelineDepthStencilStateCreateInfo depthStencil {};
        depthStencil.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable  = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp   = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable     = VK_FALSE;
        depthStencil.minDepthBounds        = 0.0F;
        depthStencil.maxDepthBounds        = 1.0F;

        VkPipelineMultisampleStateCreateInfo multisampling {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable  = VK_TRUE;
        multisampling.rasterizationSamples = samples;
        multisampling.minSampleShading     = 0.2F;

        VkPipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
            | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending {};
        colorBlending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable   = VK_FALSE;
        colorBlending.logicOp         = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments    = &colorBlendAttachment;

        std::vector<VkDescriptorSetLayout> vulkanLayouts;
        vulkanLayouts.resize(layouts.size());
        for (std::size_t i = 0; i < vulkanLayouts.size(); i++) {
            vulkanLayouts[i] = layouts[i].m_Layout;
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount         = static_cast<uint32_t>(vulkanLayouts.size());
        pipelineLayoutInfo.pSetLayouts            = vulkanLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(
                device.GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create pipeline layout!");
            return false;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount          = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages             = shaderStages.data();
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pDepthStencilState  = &depthStencil;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDynamicState       = &dynamicStateCreateInfo;
        pipelineInfo.layout              = m_PipelineLayout;
        pipelineInfo.renderPass          = renderPass.m_RenderPass;
        pipelineInfo.subpass             = 0;


        if (vkCreateGraphicsPipelines(
                device.GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create graphics pipeline!");
            return false;
        }

        vkDestroyShaderModule(device.GetHandle(), fragmentShaderModule, nullptr);
        vkDestroyShaderModule(device.GetHandle(), vertexShaderModule, nullptr);

        return true;
    }

    static RenderPass& GetCorrectRenderPass(
        RefPtr<VulkanGraphicsContext>& context, PipelineType type) {
        switch (type) {
        case PipelineType::Graphics:
            return context->m_GraphicsRenderPass;
        case PipelineType::Overlay:
        case PipelineType::Main:
            return context->m_RenderPass;
        }
    }

    bool GraphicsPipeline::Init(RefPtr<GraphicsContext>& context, PipelineShaders& shaders,
        std::vector<Astrelis::BufferBinding>&                 bindings,
        std::vector<RawRef<Astrelis::BindingDescriptorSet*>>& layouts, PipelineType type) {
        auto                             ctx = context.As<VulkanGraphicsContext>();
        std::vector<DescriptorSetLayout> vulkanLayouts;
        vulkanLayouts.reserve(layouts.size());
        for (auto& layout : layouts) {
            vulkanLayouts.push_back(layout.As<BindingDescriptorSet*>()->m_Layout);
        }
        return Init(ctx->m_LogicalDevice, ctx->m_Swapchain.GetExtent(),
            GetCorrectRenderPass(ctx, type), shaders, bindings, vulkanLayouts, ctx->m_MSAASamples);
    }

    void GraphicsPipeline::Destroy(RefPtr<GraphicsContext>& context) {
        auto& device = context.As<VulkanGraphicsContext>()->m_LogicalDevice;
        vkDestroyPipeline(device.GetHandle(), m_Pipeline, nullptr);
        vkDestroyPipelineLayout(device.GetHandle(), m_PipelineLayout, nullptr);
    }

    void GraphicsPipeline::Bind(RefPtr<Astrelis::GraphicsContext>& context) {
        auto& cBuffer = context.As<VulkanGraphicsContext>()->GetCurrentFrame().CommandBuffer;

        vkCmdBindPipeline(cBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
    }
} // namespace Astrelis::Vulkan
