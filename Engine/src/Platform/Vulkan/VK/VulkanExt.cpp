#include "VulkanExt.hpp"

#include "Astrelis/Core/Base.hpp"

namespace Astrelis::Vulkan
{
    VkBool32 VKAPI_CALL DebugCallback([[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                      [[maybe_unused]] void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            ASTRELIS_CORE_LOG_WARN("Validation layer: {0}", pCallbackData->pMessage);
        }
        else
        {
            ASTRELIS_CORE_LOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    PFN_vkCreateDebugUtilsMessengerEXT Ext::CreateDebugUtilsMessengerEXT   = nullptr;
    PFN_vkDestroyDebugUtilsMessengerEXT Ext::DestroyDebugUtilsMessengerEXT = nullptr;
    PFN_vkCmdBeginDebugUtilsLabelEXT Ext::CmdBeginDebugUtilsLabelEXT       = nullptr;
    PFN_vkCmdEndDebugUtilsLabelEXT Ext::CmdEndDebugUtilsLabelEXT           = nullptr;
    PFN_vkCmdInsertDebugUtilsLabelEXT Ext::CmdInsertDebugUtilsLabelEXT     = nullptr;

    void Ext::Init(VkInstance instance)
    {
        CreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        ASTRELIS_CORE_ASSERT(CreateDebugUtilsMessengerEXT, "Failed to load vkCreateDebugUtilsMessengerEXT");
        DestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        ASTRELIS_CORE_ASSERT(DestroyDebugUtilsMessengerEXT, "Failed to load vkDestroyDebugUtilsMessengerEXT");
        CmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
        ASTRELIS_CORE_ASSERT(CmdBeginDebugUtilsLabelEXT, "Failed to load vkCmdBeginDebugUtilsLabelEXT");
        CmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
        ASTRELIS_CORE_ASSERT(CmdEndDebugUtilsLabelEXT, "Failed to load vkCmdEndDebugUtilsLabelEXT");
        CmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
        ASTRELIS_CORE_ASSERT(CmdInsertDebugUtilsLabelEXT, "Failed to load vkCmdInsertDebugUtilsLabelEXT");
    }

    void Ext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo                 = {};
        createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData       = nullptr;
    }

    void Ext::BeginDebugLabel(VkCommandBuffer commandBuffer, const char* label, std::array<float, 4> color)
    {
        VkDebugUtilsLabelEXT labelInfo = {};
        labelInfo.sType                = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        labelInfo.pLabelName           = label;
        labelInfo.color[0]             = color[0];
        labelInfo.color[1]             = color[1];
        labelInfo.color[2]             = color[2];
        labelInfo.color[3]             = color[3];

        CmdBeginDebugUtilsLabelEXT(commandBuffer, &labelInfo);
    }

    void Ext::EndDebugLabel(VkCommandBuffer commandBuffer) { CmdEndDebugUtilsLabelEXT(commandBuffer); }

    void Ext::InsertDebugLabel(VkCommandBuffer commandBuffer, const char* label, std::array<float, 4> color)
    {
        VkDebugUtilsLabelEXT labelInfo = {};
        labelInfo.sType                = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        labelInfo.pLabelName           = label;
        labelInfo.color[0]             = color[0];
        labelInfo.color[1]             = color[1];
        labelInfo.color[2]             = color[2];
        labelInfo.color[3]             = color[3];
        CmdInsertDebugUtilsLabelEXT(commandBuffer, &labelInfo);
    }
} // namespace Astrelis::Vulkan
