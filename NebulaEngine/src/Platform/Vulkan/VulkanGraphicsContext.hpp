#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

#include "NebulaEngine/Renderer/TextureSampler.hpp"
#include "VK/CommandBuffer.hpp"
#include "VK/CommandPool.hpp"
#include "VK/DebugMessenger.hpp"
#include "VK/DescriptorPool.hpp"
#include "VK/Fence.hpp"
#include "VK/FrameBuffer.hpp"
#include "VK/ImageView.hpp"
#include "VK/Instance.hpp"
#include "VK/LogicalDevice.hpp"
#include "VK/PhysicalDevice.hpp"
#include "VK/RenderPass.hpp"
#include "VK/Semaphore.hpp"
#include "VK/Surface.hpp"
#include "VK/SwapChain.hpp"
#include "VK/TextureImage.hpp"
#include "VK/TextureSampler.hpp"

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

        struct SwapChainFrame
        {
            Vulkan::ImageView ImageView;
            Vulkan::FrameBuffer FrameBuffer;

            RefPtr<Vulkan::TextureImage> GraphicsTextureImage;
            Vulkan::FrameBuffer GraphicsFrameBuffer;

            RefPtr<Vulkan::TextureImage> UITextureImage;
            Vulkan::FrameBuffer UIFrameBuffer;

            SwapChainFrame() = default;
        };

        struct FrameData
        {
            Vulkan::CommandBuffer CommandBuffer;
            Vulkan::Semaphore ImageAvailableSemaphore;
            Vulkan::Semaphore RenderFinishedSemaphore;
            Vulkan::Fence InFlightFence;

            FrameData() = default;
        };

        bool Init() override;
        void Shutdown() override;
        void BeginFrame() override;
        void EndFrame() override;

        bool SkipFrame() override { return m_SkipFrame; }

        void RecreateSwapChain();

        bool IsInitialized() const override { return m_IsInitialized; }

        std::uint32_t GetCurrentFrameIndex() const override { return m_CurrentFrame; }

        std::uint32_t GetImageIndex() const override { return m_ImageIndex; }

        FrameData& GetCurrentFrame() { return m_Frames[m_CurrentFrame]; }

        static RefPtr<VulkanGraphicsContext> Create(RawRef<GLFWwindow*> window);

        RawRef<GLFWwindow*> m_Window;
        Vulkan::Instance m_Instance;
        Vulkan::DebugMessenger m_DebugMessenger;
        Vulkan::Surface m_Surface;
        Vulkan::PhysicalDevice m_PhysicalDevice;
        Vulkan::LogicalDevice m_LogicalDevice;
        Vulkan::CommandPool m_CommandPool;
        Vulkan::SwapChain m_SwapChain;
        Vulkan::DescriptorPool m_DescriptorPool;

        Vulkan::RenderPass m_RenderPass;
        Vulkan::RenderPass m_GraphicsRenderPass;
        Vulkan::RenderPass m_UIRenderPass;

        std::vector<SwapChainFrame> m_SwapChainFrames;
        std::vector<FrameData> m_Frames;
        VkOffset2D m_GraphicsOffset {0, 0};
        VkOffset2D m_UIOffset {0, 0};
        VkExtent2D m_GraphicsExtent {0, 0};
        VkExtent2D m_UIExtent {0, 0};

        std::uint32_t m_CurrentFrame      = 0;
        std::uint32_t m_ImageIndex        = 0;
        std::uint32_t m_MaxFramesInFlight = 2;

        // Internal
        VkSwapchainKHR m_OldSwapChain = VK_NULL_HANDLE;

        bool m_Debug         = true;
        bool m_IsInitialized = false;
        bool m_NeedsResize   = false;
        bool m_SkipFrame     = false;
    };
} // namespace Nebula
