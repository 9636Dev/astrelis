#pragma once

#include <array>
#include <vulkan/vulkan.h>

namespace Astrelis::Vulkan
{
    VKAPI_ATTR VkBool32 VKAPI_CALL
        DebugCallback([[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                      [[maybe_unused]] void* pUserData);

    class Ext
    {
    public:
        Ext() = delete;
        static void Init(VkInstance instance);

        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static void BeginDebugLabel(VkCommandBuffer commandBuffer, const char* label, std::array<float, 4> color);
        static void EndDebugLabel(VkCommandBuffer commandBuffer);
        static void InsertDebugLabel(VkCommandBuffer commandBuffer, const char* label, std::array<float, 4> color);

        static PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
        static PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
        static PFN_vkCmdBeginDebugUtilsLabelEXT CmdBeginDebugUtilsLabelEXT;
        static PFN_vkCmdEndDebugUtilsLabelEXT CmdEndDebugUtilsLabelEXT;
        static PFN_vkCmdInsertDebugUtilsLabelEXT CmdInsertDebugUtilsLabelEXT;
    };
} // namespace Astrelis::Vulkan
