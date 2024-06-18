#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

#include "Platform/Vulkan/VK/LogicalDevice.hpp"
#include "Platform/Vulkan/VK/Surface.hpp"
#include "Platform/Vulkan/VK/SwapChain.hpp"
#include "VK/DebugMessenger.hpp"
#include "VK/Instance.hpp"
#include "VK/PhysicalDevice.hpp"

#include <GLFW/glfw3.h>

namespace Nebula
{
    class VulkanGraphicsContext : public GraphicsContext
    {
    public:
        friend class Vulkan2DRendererAPI;
        friend class Vulkan3DRendererAPI;
        friend class VulkanRayTracerRendererAPI;
        explicit VulkanGraphicsContext(RawRef<GLFWwindow*> window);
        ~VulkanGraphicsContext() override;
        VulkanGraphicsContext(const VulkanGraphicsContext&)            = delete;
        VulkanGraphicsContext& operator=(const VulkanGraphicsContext&) = delete;
        VulkanGraphicsContext(VulkanGraphicsContext&&)                 = delete;
        VulkanGraphicsContext& operator=(VulkanGraphicsContext&&)      = delete;

        bool Init() override;
        void Shutdown() override;
        bool IsInitialized() const override { return m_IsInitialized; }

        static RefPtr<VulkanGraphicsContext> Create(RawRef<GLFWwindow*> window);

        RawRef<GLFWwindow*> m_Window;
        Vulkan::Instance m_Instance;
        Vulkan::DebugMessenger m_DebugMessenger;
        Vulkan::Surface m_Surface;
        Vulkan::PhysicalDevice m_PhysicalDevice;
        Vulkan::LogicalDevice m_LogicalDevice;
        Vulkan::SwapChain m_SwapChain;

        bool m_Debug = true;
        bool m_IsInitialized = false;
    };
} // namespace Nebula
