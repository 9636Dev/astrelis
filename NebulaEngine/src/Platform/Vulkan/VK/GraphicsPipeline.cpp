#include "GraphicsPipeline.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include "CommandBuffer.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "RenderPass.hpp"

#include <fstream>
#include <vulkan/vulkan_core.h>

namespace Nebula::Vulkan
{
    static std::vector<char> ReadFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            NEBULA_CORE_LOG_ERROR("Failed to open file: {0}", filename);
            return {};
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), static_cast<std::int64_t>(fileSize));
        file.close();

        return buffer;
    }

    static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule = nullptr;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create shader module!");
            return VK_NULL_HANDLE;
        }

        return shaderModule;
    }

    static VkFormat InputCountToFormat(std::size_t count)
    {
        switch (count)
        {
        case 1:
            return VK_FORMAT_R32_SFLOAT;
        case 2:
            return VK_FORMAT_R32G32_SFLOAT;
        case 3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            return VK_FORMAT_UNDEFINED;
        }
    }

    bool GraphicsPipeline::Init(LogicalDevice& device, RenderPass& renderPass, SwapChain& swapChain, VertexInput& input)
    {
        auto vertexShaderCode   = ReadFile("shaders/BasicVert.spv");
        auto fragmentShaderCode = ReadFile("shaders/BasicFrag.spv");

        VkShaderModule vertexShaderModule   = CreateShaderModule(device.GetHandle(), vertexShaderCode);
        VkShaderModule fragmentShaderModule = CreateShaderModule(device.GetHandle(), fragmentShaderCode);

        VkPipelineShaderStageCreateInfo vertexShaderStageInfo {};
        vertexShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = vertexShaderModule;
        vertexShaderStageInfo.pName  = "main";

        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo {};
        fragmentShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module = fragmentShaderModule;
        fragmentShaderStageInfo.pName  = "main";

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {vertexShaderStageInfo, fragmentShaderStageInfo};

        std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo {};
        dynamicStateCreateInfo.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateCreateInfo.pDynamicStates    = dynamicStates.data();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkVertexInputBindingDescription bindingDescription {};
        bindingDescription.binding   = 0;
        bindingDescription.stride    = input.Stride;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        attributeDescriptions.resize(input.Elements.size());

        for (std::size_t i = 0; i < input.Elements.size(); i++)
        {
            attributeDescriptions[i].binding  = 0;
            attributeDescriptions[i].location = static_cast<uint32_t>(i);
            attributeDescriptions[i].format   = InputCountToFormat(input.Elements[i].Count);
            attributeDescriptions[i].offset   = input.Elements[i].Offset;
        }

        vertexInputInfo.vertexBindingDescriptionCount   = 1;
        vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
        inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport {};
        viewport.x        = 0.0F;
        viewport.y        = 0.0F;
        viewport.width    = static_cast<float>(swapChain.GetExtent().width);
        viewport.height   = static_cast<float>(swapChain.GetExtent().height);
        viewport.minDepth = 0.0F;
        viewport.maxDepth = 1.0F;

        VkRect2D scissor {};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.GetExtent();

        VkPipelineViewportStateCreateInfo viewportState {};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer {};
        rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable        = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth               = 1.0F;
        rasterizer.cullMode                = VK_CULL_MODE_NONE;
        rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;

        rasterizer.depthBiasEnable         = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0F;
        rasterizer.depthBiasClamp          = 0.0F;
        rasterizer.depthBiasSlopeFactor    = 0.0F;

        VkPipelineMultisampleStateCreateInfo multisampling {};
        multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable  = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending {};
        colorBlending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable   = VK_FALSE;
        colorBlending.logicOp         = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments    = &colorBlendAttachment;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount         = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(device.GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create pipeline layout!");
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
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDynamicState       = &dynamicStateCreateInfo;
        pipelineInfo.layout              = m_PipelineLayout;
        pipelineInfo.renderPass          = renderPass.m_RenderPass;
        pipelineInfo.subpass             = 0;


        if (vkCreateGraphicsPipelines(device.GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) !=
            VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create graphics pipeline!");
            return false;
        }

        vkDestroyShaderModule(device.GetHandle(), fragmentShaderModule, nullptr);
        vkDestroyShaderModule(device.GetHandle(), vertexShaderModule, nullptr);

        return true;
    }

    void GraphicsPipeline::Destroy(LogicalDevice& device)
    {
        vkDestroyPipeline(device.GetHandle(), m_Pipeline, nullptr);
        vkDestroyPipelineLayout(device.GetHandle(), m_PipelineLayout, nullptr);
    }

    void GraphicsPipeline::Bind(RefPtr<Nebula::GraphicsContext>& context)
    {
        auto& cBuffer = context.As<VulkanGraphicsContext>()->GetCurrentFrame().CommandBuffer;
        vkCmdBindPipeline(cBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
    }
} // namespace Nebula::Vulkan
