#include "VulkanGraphicsContext.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VK/RenderPass.hpp"
#include "VK/Utils.hpp"
#include "VK/VulkanExt.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Nebula
{
    VulkanGraphicsContext::VulkanGraphicsContext(RawRef<GLFWwindow*> window) : m_Window(std::move(window)) {}

    VulkanGraphicsContext::~VulkanGraphicsContext() = default;

    bool VulkanGraphicsContext::Init()
    {
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define INIT_COMPONENT(...) \
    if (!__VA_ARGS__)       \
    return false

        if (m_IsInitialized)
        {
            NEBULA_CORE_LOG_WARN("Vulkan Graphics Context already initialized!");
            return false;
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (m_Debug)
        {
            if (!Vulkan::CheckValidationLayerSupport())
            {
                NEBULA_CORE_LOG_ERROR("Validation layers requested, but not available!");
                return false;
            }

            Vulkan::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        }

        INIT_COMPONENT(m_Instance.Init("Nebula", "Nebula Engine", Vulkan::APIVersion(1, 0),
                                       Vulkan::GetRequiredExtensions(m_Debug), Vulkan::GetValidationLayers(),
                                       &debugCreateInfo));
        m_PhysicalDevice.PickBestDevice(m_Instance);
        INIT_COMPONENT(m_PhysicalDevice.IsFound());
        INIT_COMPONENT(m_Surface.Init(m_Instance, m_Window));
        INIT_COMPONENT(m_LogicalDevice.Init(m_PhysicalDevice, m_Surface, Vulkan::GetDeviceExtensions(),
                                            m_Debug ? Vulkan::GetValidationLayers() : std::vector<const char*>()));
        INIT_COMPONENT(m_SwapChain.Init(m_Window, m_PhysicalDevice, m_LogicalDevice, m_Surface));
        INIT_COMPONENT(m_CommandPool.Init(m_LogicalDevice));
        Vulkan::RenderPassInfo renderPassInfo {};
        INIT_COMPONENT(m_RenderPass.Init(m_LogicalDevice, m_SwapChain, renderPassInfo));

        // TODO: Create a descriptor pool manager, right now it is just hardcoded
        INIT_COMPONENT(m_DescriptorPool.Init(m_LogicalDevice, 1'000));

        m_SwapChainFrames.resize(m_SwapChain.GetImageCount());
        for (std::size_t i = 0; i < m_SwapChainFrames.size(); ++i)
        {
            auto& frame = m_SwapChainFrames[i];
            INIT_COMPONENT(
                frame.ImageView.Init(m_LogicalDevice, m_SwapChain.GetImages()[i], m_SwapChain.GetImageFormat()));
            INIT_COMPONENT(frame.FrameBuffer.Init(m_LogicalDevice, m_RenderPass, frame.ImageView,
                                                  m_SwapChain.GetExtent().width, m_SwapChain.GetExtent().height));
        }

        m_Frames.resize(m_SwapChain.GetImageCount());
        for (std::size_t i = 0; i < m_Frames.size(); ++i)
        {
            auto& frame = m_Frames[i];
            frame.CommandBuffer.Init(m_LogicalDevice, m_CommandPool);
            frame.ImageAvailableSemaphore.Init(m_LogicalDevice);
            frame.RenderFinishedSemaphore.Init(m_LogicalDevice);
            frame.InFlightFence.Init(m_LogicalDevice);
        }

        NEBULA_CORE_LOG_INFO("Vulkan Graphics Context initialized!");
        m_IsInitialized = true;
        return true;
#undef INIT_COMPONENT
    }

    void VulkanGraphicsContext::Shutdown()
    {
        vkDeviceWaitIdle(m_LogicalDevice.GetHandle());

        if (!m_IsInitialized)
        {
            NEBULA_CORE_LOG_WARN("Vulkan Graphics Context already shutdown!");
            return;
        }

        for (auto& frame : m_Frames)
        {
            frame.CommandBuffer.Destroy(m_LogicalDevice, m_CommandPool);
            frame.ImageAvailableSemaphore.Destroy(m_LogicalDevice);
            frame.RenderFinishedSemaphore.Destroy(m_LogicalDevice);
            frame.InFlightFence.Destroy(m_LogicalDevice);
        }

        for (auto& frame : m_SwapChainFrames)
        {
            frame.ImageView.Destroy(m_LogicalDevice);
            frame.FrameBuffer.Destroy(m_LogicalDevice);
        }

        m_DescriptorPool.Destroy(m_LogicalDevice);
        m_RenderPass.Destroy(m_LogicalDevice);
        m_CommandPool.Destroy(m_LogicalDevice);
        m_SwapChain.Destroy(m_LogicalDevice);
        m_LogicalDevice.Destroy();
        m_Surface.Destroy(m_Instance);
        // Physical device doesn't need to be destroyed
        m_Instance.Destroy();
    }

    void VulkanGraphicsContext::BeginFrame()
    {
        auto& frame = GetCurrentFrame();
        frame.InFlightFence.Wait(m_LogicalDevice, std::numeric_limits<std::uint64_t>::max());
        VkResult result = vkAcquireNextImageKHR(
            m_LogicalDevice.GetHandle(), m_SwapChain.GetHandle(), std::numeric_limits<std::uint64_t>::max(),
            frame.ImageAvailableSemaphore.GetHandle(), VK_NULL_HANDLE, &m_ImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            NEBULA_CORE_LOG_TRACE("Out of date swap chain! (FrameBegin)");
            m_SkipFrame = true;
            ResizeViewport();
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            NEBULA_CORE_LOG_ERROR("Failed to acquire swap chain image!");
        }

        NEBULA_CORE_LOG_TRACE("Begin frame...");
        frame.CommandBuffer.Reset();
        frame.CommandBuffer.Begin();
        m_RenderPass.Begin(frame.CommandBuffer, m_SwapChainFrames[m_ImageIndex].FrameBuffer, m_SwapChain.GetExtent());
    }

    void VulkanGraphicsContext::EndFrame()
    {
        auto& frame = GetCurrentFrame();
        m_RenderPass.End(frame.CommandBuffer);
        frame.CommandBuffer.End();
        if (m_SkipFrame)
        {
            NEBULA_CORE_LOG_TRACE("Skipping EndFrame...");
            frame.CommandBuffer.Reset();
            m_SkipFrame = false;
            return;
        }
        NEBULA_CORE_LOG_TRACE("Submitting frame...");

        frame.CommandBuffer.Submit(m_LogicalDevice, m_LogicalDevice.GetGraphicsQueue(), frame.ImageAvailableSemaphore,
                                   frame.RenderFinishedSemaphore, frame.InFlightFence);

        std::array<VkSemaphore, 1> presentWaitSemaphores = {frame.RenderFinishedSemaphore.GetHandle()};

        std::array<VkSwapchainKHR, 1> swapChains = {m_SwapChain.GetHandle()};
        VkPresentInfoKHR presentInfo             = {};
        presentInfo.sType                        = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount           = static_cast<std::uint32_t>(presentWaitSemaphores.size());
        presentInfo.pWaitSemaphores              = presentWaitSemaphores.data();
        presentInfo.swapchainCount               = static_cast<std::uint32_t>(swapChains.size());
        presentInfo.pSwapchains                  = swapChains.data();
        presentInfo.pImageIndices                = &m_ImageIndex;

        auto result = vkQueuePresentKHR(m_LogicalDevice.GetPresentQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_NeedsResize)
        {
            NEBULA_CORE_LOG_TRACE("Out of date swap chain! (FrameEnd)");
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to present swap chain image!");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % m_SwapChain.GetImageCount();
    }

    void VulkanGraphicsContext::ResizeViewport()
    {
        NEBULA_CORE_LOG_DEBUG("Recreating swap chain...");

        m_SkipFrame = true;
        RecreateSwapChain();
    }

    void VulkanGraphicsContext::RecreateSwapChain()
    {
        int width  = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(m_Window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_LogicalDevice.GetHandle());

        // We have to do in this order:
        // Framebuffers -> Color resources -> Depth resources -> Image views -> Swap chain
        for (auto& swapchainFrame : m_SwapChainFrames)
        {
            swapchainFrame.FrameBuffer.Destroy(m_LogicalDevice);
            swapchainFrame.ImageView.Destroy(m_LogicalDevice);
        }

        m_SwapChain.Destroy(m_LogicalDevice);

        auto result = m_SwapChain.Init(m_Window, m_PhysicalDevice, m_LogicalDevice, m_Surface);
        NEBULA_CORE_ASSERT(result, "Failed to recreate swap chain!");

        for (auto& swapchainFrame : m_SwapChainFrames)
        {
            result = swapchainFrame.ImageView.Init(m_LogicalDevice, m_SwapChain.GetImages()[m_ImageIndex],
                                                   m_SwapChain.GetImageFormat());
            NEBULA_CORE_ASSERT(result, "Failed to recreate image view!");
            result = swapchainFrame.FrameBuffer.Init(m_LogicalDevice, m_RenderPass, swapchainFrame.ImageView,
                                                     m_SwapChain.GetExtent().width, m_SwapChain.GetExtent().height);
            NEBULA_CORE_ASSERT(result, "Failed to recreate frame buffer!");
        }

        m_NeedsResize = false;
    }

    RefPtr<VulkanGraphicsContext> VulkanGraphicsContext::Create(RawRef<GLFWwindow*> window)
    {
        return RefPtr<VulkanGraphicsContext>::Create(window);
    }
} // namespace Nebula
