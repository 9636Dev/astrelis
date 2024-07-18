#include "VulkanGraphicsContext.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Core/GlobalConfig.hpp"
#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "Platform/Vulkan/VK/RenderPass.hpp"
#include "VK/Utils.hpp"
#include "VK/VulkanExt.hpp"

#include <GLFW/glfw3.h>
#include <array>
#include <vulkan/vulkan.h>

// TODO: Remove
#include <stb_image_write.h>

namespace Nebula
{
    VulkanGraphicsContext::VulkanGraphicsContext(RawRef<GLFWwindow*> window) :
        m_Window(std::move(window)),
        m_MaxFramesInFlight(RendererAPI::GetBufferingCount())
    {
    }

    VulkanGraphicsContext::~VulkanGraphicsContext() = default;

    bool VulkanGraphicsContext::Init()
    {
        NEBULA_PROFILE_SCOPE("Nebula::VulkanGraphicsContext::Init");
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
        if (GlobalConfig::IsDebugMode())
        {
            if (!Vulkan::CheckValidationLayerSupport())
            {
                NEBULA_CORE_LOG_ERROR("Validation layers requested, but not available!");
                return false;
            }

            Vulkan::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        }

        INIT_COMPONENT(m_Instance.Init("Nebula", "Nebula Engine", Vulkan::APIVersion(1, 0),
                                       Vulkan::GetRequiredExtensions(GlobalConfig::IsDebugMode()), GlobalConfig::IsDebugMode() ? Vulkan::GetValidationLayers() : std::vector<const char*>(),
                                       &debugCreateInfo));
        INIT_COMPONENT(m_Surface.Init(m_Instance, m_Window));
        m_PhysicalDevice.PickBestDevice(m_Instance, m_Surface.GetHandle());
        INIT_COMPONENT(m_PhysicalDevice.IsFound());
        INIT_COMPONENT(m_LogicalDevice.Init(m_PhysicalDevice, m_Surface, Vulkan::GetDeviceExtensions(),
                                            GlobalConfig::IsDebugMode() ? Vulkan::GetValidationLayers() : std::vector<const char*>()));
        std::uint32_t frameCount = m_MaxFramesInFlight;
        INIT_COMPONENT(m_SwapChain.Init(m_Window, m_PhysicalDevice, m_LogicalDevice, m_Surface, frameCount, m_VSync));
        if (frameCount != m_MaxFramesInFlight)
        {
            NEBULA_CORE_LOG_WARN("Requested frame count is not supported, using {0} (instead of {1})", frameCount,
                                 m_MaxFramesInFlight);
        }
        INIT_COMPONENT(m_CommandPool.Init(m_LogicalDevice));

        if (m_GraphicsExtent.width == 0 || m_GraphicsExtent.height == 0)
        {
            m_GraphicsExtent = m_SwapChain.GetExtent();
        }

        m_SwapChainFrames.resize(m_SwapChain.GetImageCount());

        {
            // Main render pass
            Vulkan::RenderPassInfo renderPassInfo {};
            VkAttachmentDescription& colorAttachment = renderPassInfo.Attachments.emplace_back();
            colorAttachment.format                   = m_SwapChain.GetImageFormat();
            colorAttachment.samples                  = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp                   = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout              = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


            renderPassInfo.Subpasses = {
                {VK_PIPELINE_BIND_POINT_GRAPHICS, {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}},
            };

            INIT_COMPONENT(m_RenderPass.Init(m_LogicalDevice, renderPassInfo));
        }

        {
            m_GraphicsTextureImage = RefPtr<Vulkan::TextureImage>::Create();
            m_GraphicsTextureImage->Init(m_LogicalDevice, m_CommandPool, m_PhysicalDevice, m_GraphicsExtent,
                                         VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                                         VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                                             VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            // Graphics render pass
            Vulkan::RenderPassInfo renderPassInfo {};
            VkAttachmentDescription& colorAttachment = renderPassInfo.Attachments.emplace_back();
            colorAttachment.format                   = VK_FORMAT_R8G8B8A8_SRGB;
            colorAttachment.samples                  = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp                   = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout              = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            renderPassInfo.Subpasses = {
                {VK_PIPELINE_BIND_POINT_GRAPHICS, {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}},
            };

            INIT_COMPONENT(m_GraphicsRenderPass.Init(m_LogicalDevice, renderPassInfo));

            if (!m_GraphicsFrameBuffer.Init(m_LogicalDevice, m_GraphicsRenderPass,
                                            m_GraphicsTextureImage->GetImageView(), m_GraphicsExtent))
            {
                return false;
            }
        }

        // TODO: Create a descriptor pool manager, right now it is just hardcoded
        INIT_COMPONENT(m_DescriptorPool.Init(m_LogicalDevice, 1'000));

        for (std::size_t i = 0; i < m_SwapChainFrames.size(); ++i)
        {
            auto& frame = m_SwapChainFrames[i];
            INIT_COMPONENT(
                frame.ImageView.Init(m_LogicalDevice, m_SwapChain.GetImages()[i], m_SwapChain.GetImageFormat()));
            INIT_COMPONENT(frame.FrameBuffer.Init(m_LogicalDevice, m_RenderPass, frame.ImageView.m_ImageView,
                                                  m_SwapChain.GetExtent()));
        }

        m_Frames.resize(m_SwapChain.GetImageCount());
        for (std::size_t i = 0; i < m_Frames.size(); ++i)
        {
            auto& frame = m_Frames[i];
            INIT_COMPONENT(frame.CommandBuffer.Init(m_LogicalDevice, m_CommandPool));
            INIT_COMPONENT(frame.ImageAvailableSemaphore.Init(m_LogicalDevice));
            INIT_COMPONENT(frame.RenderFinishedSemaphore.Init(m_LogicalDevice));
            INIT_COMPONENT(frame.InFlightFence.Init(m_LogicalDevice));
        }

        NEBULA_CORE_LOG_INFO("Vulkan Graphics Context initialized!");
        m_IsInitialized = true;
        return true;
#undef INIT_COMPONENT
    }

    void VulkanGraphicsContext::Shutdown()
    {
        NEBULA_PROFILE_SCOPE("Nebula::VulkanGraphicsContext::Shutdown");
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

        m_GraphicsFrameBuffer.Destroy(m_LogicalDevice);
        m_GraphicsTextureImage->Destroy(m_LogicalDevice);

        m_DescriptorPool.Destroy(m_LogicalDevice);
        m_RenderPass.Destroy(m_LogicalDevice);
        m_GraphicsRenderPass.Destroy(m_LogicalDevice);
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

        if (result == VK_ERROR_OUT_OF_DATE_KHR || m_SwapchainRecreation)
        {
            VkSubmitInfo submitInfo                        = {};
            submitInfo.sType                               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            std::array<VkSemaphore, 1> waitSemaphores      = {frame.ImageAvailableSemaphore.GetHandle()};
            submitInfo.waitSemaphoreCount                  = static_cast<std::uint32_t>(waitSemaphores.size());
            submitInfo.pWaitSemaphores                     = waitSemaphores.data();
            std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            static_assert(waitSemaphores.size() == waitStages.size());
            submitInfo.pWaitDstStageMask    = waitStages.data();
            submitInfo.commandBufferCount   = 0;
            submitInfo.pCommandBuffers      = nullptr;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pSignalSemaphores    = nullptr;
            vkQueueSubmit(m_LogicalDevice.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(m_LogicalDevice.GetGraphicsQueue());

            m_SkipFrame = true;
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            NEBULA_CORE_LOG_ERROR("Failed to acquire swap chain image!");
        }
        else
        {
            frame.InFlightFence.Reset(m_LogicalDevice);
        }

        frame.CommandBuffer.Reset();
        frame.CommandBuffer.Begin();
    }

    void VulkanGraphicsContext::EndFrame()
    {
        auto& frame = GetCurrentFrame();
        frame.CommandBuffer.End();

        if (m_SkipFrame)
        {
            m_SkipFrame = false;
            frame.CommandBuffer.Reset();
            return;
        }

        frame.CommandBuffer.Submit(m_LogicalDevice, m_LogicalDevice.GetGraphicsQueue(), frame.ImageAvailableSemaphore,
                                   frame.RenderFinishedSemaphore, frame.InFlightFence);

        std::array<VkSemaphore, 1> presentWaitSemaphores = {frame.RenderFinishedSemaphore.GetHandle()};
        std::array<VkSwapchainKHR, 1> swapChains         = {m_SwapChain.GetHandle()};
        VkPresentInfoKHR presentInfo                     = {};
        presentInfo.sType                                = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount                   = 1;
        presentInfo.pWaitSemaphores                      = presentWaitSemaphores.data();
        presentInfo.swapchainCount                       = static_cast<std::uint32_t>(swapChains.size());
        presentInfo.pSwapchains                          = swapChains.data();
        presentInfo.pImageIndices                        = &m_ImageIndex;

        if (m_CaptureNextFrame)
        {
            frame.InFlightFence.Wait(m_LogicalDevice, std::numeric_limits<std::uint64_t>::max());

            m_CapturePromise.set_value(CaptureScreen());
            m_CaptureNextFrame = false;
        }

        auto result = vkQueuePresentKHR(m_LogicalDevice.GetPresentQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_SwapchainRecreation)
        {
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to present swap chain image!");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
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

        vkDestroySwapchainKHR(m_LogicalDevice.GetHandle(), m_SwapChain.GetHandle(), nullptr);

        std::uint32_t frameCount = m_MaxFramesInFlight;
        auto result = m_SwapChain.Init(m_Window, m_PhysicalDevice, m_LogicalDevice, m_Surface, frameCount, m_VSync);
        (void)result; // TODO: Better error handling here
        NEBULA_CORE_VERIFY(result, "Failed to recreate swap chain!");

        for (std::size_t i = 0; i < m_SwapChainFrames.size(); ++i)
        {
            auto& frame = m_SwapChainFrames[i];

            auto res = frame.ImageView.Init(m_LogicalDevice, m_SwapChain.GetImages()[i], m_SwapChain.GetImageFormat());
            NEBULA_CORE_ASSERT(res, "Failed to recreate image view!");
            res = frame.FrameBuffer.Init(m_LogicalDevice, m_RenderPass, frame.ImageView.m_ImageView,
                                         m_SwapChain.GetExtent());
            NEBULA_CORE_ASSERT(res, "Failed to recreate frame buffer!");
            (void)res; // For release builds
        }

        m_SwapchainRecreation = false;
    }

    InMemoryImage VulkanGraphicsContext::CaptureScreen() const
    {
        constexpr VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
        VkImage image {};
        VkDeviceMemory imageMemory {};
        Vulkan::CreateImage(
            m_PhysicalDevice.GetHandle(), m_LogicalDevice.GetHandle(), m_SwapChain.GetExtent().width,
            m_SwapChain.GetExtent().height, format, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, image, imageMemory);

        // Transition the image layout to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(), m_LogicalDevice.GetGraphicsQueue(),
                                      m_CommandPool.GetHandle(), image, m_SwapChain.GetImageFormat(),
                                      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Transition swapchain to VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(), m_LogicalDevice.GetGraphicsQueue(),
                                      m_CommandPool.GetHandle(), m_SwapChain.GetImages()[m_ImageIndex],
                                      m_SwapChain.GetImageFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);


        // Blit the image to the output
        auto* cmdBuffer = Vulkan::BeginSingleTimeCommands(m_LogicalDevice.GetHandle(), m_CommandPool.GetHandle());

        VkImageBlit blit {};
        blit.srcOffsets[0]                 = {0, 0, 0};
        blit.srcOffsets[1]                 = {static_cast<int32_t>(m_SwapChain.GetExtent().width),
                                              static_cast<int32_t>(m_SwapChain.GetExtent().height), 1};
        blit.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel       = 0;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount     = 1;
        blit.dstOffsets[0]                 = {0, 0, 0};
        blit.dstOffsets[1]                 = {static_cast<int32_t>(m_SwapChain.GetExtent().width),
                                              static_cast<int32_t>(m_SwapChain.GetExtent().height), 1};
        blit.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel       = 0;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount     = 1;
        // TODO(MetalDrawableWarning): Blitting a drawable on MoltenVK returns a metal warning
        vkCmdBlitImage(cmdBuffer, m_SwapChain.GetImages()[m_ImageIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        Vulkan::EndSingleTimeCommands(m_LogicalDevice.GetHandle(), m_LogicalDevice.GetGraphicsQueue(),
                                      m_CommandPool.GetHandle(), cmdBuffer);

        // Transition the image layout to VK_IMAGE_LAYOUT_GENERAL
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(), m_LogicalDevice.GetGraphicsQueue(),
                                      m_CommandPool.GetHandle(), image, m_SwapChain.GetImageFormat(),
                                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

        // Map the image memory and copy the data to an InMemoryImage
        void* data = nullptr;
        vkMapMemory(m_LogicalDevice.GetHandle(), imageMemory, 0,
                    m_SwapChain.GetExtent().width * m_SwapChain.GetExtent().height * 4, 0, &data);

        std::vector<std::byte> imageData(
            static_cast<std::size_t>(m_SwapChain.GetExtent().width * m_SwapChain.GetExtent().height * 4));
        std::memcpy(imageData.data(), data, imageData.size());
        // Cleanup
        vkUnmapMemory(m_LogicalDevice.GetHandle(), imageMemory);
        vkDestroyImage(m_LogicalDevice.GetHandle(), image, nullptr);
        vkFreeMemory(m_LogicalDevice.GetHandle(), imageMemory, nullptr);

        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(), m_LogicalDevice.GetGraphicsQueue(),
                                      m_CommandPool.GetHandle(), m_SwapChain.GetImages()[m_ImageIndex],
                                      m_SwapChain.GetImageFormat(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        return InMemoryImage(m_SwapChain.GetExtent().width, m_SwapChain.GetExtent().height, 4, imageData);
    }

    RefPtr<VulkanGraphicsContext> VulkanGraphicsContext::Create(RawRef<GLFWwindow*> window, ContextProps props)
    {
        auto context     = RefPtr<VulkanGraphicsContext>::Create(window);
        context->m_VSync = props.VSync;
        return context;
    }
} // namespace Nebula
