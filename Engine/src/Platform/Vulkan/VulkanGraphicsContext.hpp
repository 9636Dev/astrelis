#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Utils/Profiling.hpp"
#include "Astrelis/IO/Image.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

#include <future>
#include <vulkan/vulkan.h>

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

#ifdef ASTRELIS_PROFILE_GPU
    #include <tracy/TracyVulkan.hpp>
#endif

struct GLFWwindow;

namespace Astrelis {
    class VulkanGraphicsContext : public GraphicsContext {
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

        struct SwapChainFrame {
            Vulkan::ImageView   ImageView;
            Vulkan::FrameBuffer FrameBuffer;

            SwapChainFrame() = default;
        };

        struct FrameData {
            Vulkan::CommandBuffer CommandBuffer;
            Vulkan::Semaphore     ImageAvailableSemaphore;
            Vulkan::Semaphore     RenderFinishedSemaphore;
            Vulkan::Fence         InFlightFence;

            Vulkan::TextureImage GraphicsTextureImage;
            Vulkan::FrameBuffer  GraphicsFrameBuffer;

            FrameData() = default;
        };

        Result<EmptyType, std::string> Init() override;
        void                           Shutdown() override;
        void                           BeginFrame() override;
        void                           EndFrame() override;

        bool SkipFrame() override {
            return m_SkipFrame;
        }

        void RecreateSwapChain();

        bool IsInitialized() const override {
            return m_IsInitialized;
        }

        InMemoryImage CaptureScreen() const;

        inline FrameData& GetCurrentFrame() {
            return m_Frames[m_CurrentFrame];
        }

        bool IsVSync() const override {
            return m_VSync;
        }

        void SetVSync(bool enabled) override {
            m_VSync               = enabled;
            m_SwapchainRecreation = true;
        }

        std::uint32_t GetCurrentFrameIndex() const override {
            return m_CurrentFrame;
        }

        std::uint32_t GetImageIndex() const override {
            return m_ImageIndex;
        }

        RawRef<GLFWwindow*>    m_Window;
        Vulkan::Instance       m_Instance;
        Vulkan::DebugMessenger m_DebugMessenger;
        Vulkan::Surface        m_Surface;
        Vulkan::PhysicalDevice m_PhysicalDevice;
        Vulkan::LogicalDevice  m_LogicalDevice;
        Vulkan::CommandPool    m_CommandPool;
        Vulkan::SwapChain      m_Swapchain;
        Vulkan::DescriptorPool m_DescriptorPool;

        std::vector<SwapChainFrame> m_SwapChainFrames;
        std::vector<FrameData>      m_Frames;

        VkOffset2D         m_GraphicsOffset {0, 0};
        VkExtent2D         m_GraphicsExtent {0, 0};
        Vulkan::RenderPass m_GraphicsRenderPass;
        Vulkan::RenderPass m_RenderPass;

        // Depth and MSAA
        Vulkan::TextureImage  m_DepthTextureImage;
        VkFormat              m_DepthFormat = VK_FORMAT_D32_SFLOAT;
        Vulkan::TextureImage  m_MSAATextureImage;
        VkSampleCountFlagBits m_MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ASTRELIS_PROFILE_VULKAN(Vulkan::CommandBuffer   m_ProfileCommandBuffer;
                                std::vector<TracyVkCtx> m_TracyVkCtx;)

        // For screenshotting
        bool                        m_CaptureNextFrame = false;
        std::promise<InMemoryImage> m_CapturePromise;
        VkExtent2D                  m_CaptureOutputExtent {0, 0};

        std::uint32_t       m_CurrentFrame = 0;
        std::uint32_t       m_ImageIndex   = 0;
        bool                m_VSync        = true;
        const std::uint32_t m_MaxFramesInFlight;

        // Internal
        VkSwapchainKHR m_OldSwapChain = VK_NULL_HANDLE;

        bool m_IsInitialized       = false;
        bool m_SwapchainRecreation = false;
        bool m_SkipFrame           = false;

        static RefPtr<VulkanGraphicsContext> Create(RawRef<GLFWwindow*> window, ContextProps props);
    private:
        Result<EmptyType, std::string> CreateSwapchain();
        Result<EmptyType, std::string> CreateDepthTextureImage();
        Result<EmptyType, std::string> CreateMSAATextureImage();
        Result<EmptyType, std::string> CreateImageViewsAndFramebuffers();
    };
} // namespace Astrelis
