#include "VulkanGraphicsContext.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"
#include "Astrelis/Renderer/RendererAPI.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Platform/Vulkan/VK/RenderPass.hpp"
#include "VK/Utils.hpp"
#include "VK/VulkanExt.hpp"

namespace Astrelis {
    VulkanGraphicsContext::VulkanGraphicsContext(RawRef<GLFWwindow*> window)
        : m_Window(std::move(window)), m_MaxFramesInFlight(RendererAPI::GetBufferingCount()) {
    }

    VulkanGraphicsContext::~VulkanGraphicsContext() = default;

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    Result<EmptyType, std::string> VulkanGraphicsContext::Init() {
        ASTRELIS_PROFILE_FUNCTION();
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        if (m_IsInitialized) {
            return "Vulkan Graphics Context already initialized!";
        }

        const auto& appSpec   = Application::Get().GetSpecification();
        const bool  debugMode = GlobalConfig::IsDebugMode();

        if (!Vulkan::CheckValidationLayerSupport()) {
            return "Validation layers requested, but not available!";
        }
        ASTRELIS_CORE_LOG_DEBUG("Validation layers available!");
        if (!m_Instance.Init(appSpec.Name.c_str(),
                Vulkan::Version(
                    appSpec.Version.Major, appSpec.Version.Minor, appSpec.Version.Patch),
                Vulkan::Version(1, 0, 0), Vulkan::GetRequiredExtensions(debugMode),
                debugMode ? Vulkan::GetValidationLayers() : std::vector<const char*>())) {
            return "Failed to initialize Vulkan Instance!";
        }
        if (debugMode) {
            Vulkan::Ext::Init(m_Instance);
            if (!Vulkan::CheckValidationLayerSupport()) {
                return "Validation layers requested, but not available!";
            }

            if (!m_DebugMessenger.Init(m_Instance)) {
                return "Failed to initialize Vulkan Debug Messenger!";
            }
        }
        else {
            ASTRELIS_CORE_LOG_WARN("Running in release mode, validation layers are disabled!");
        }

        if (!m_Surface.Init(m_Instance, m_Window)) {
            return "Failed to initialize Vulkan Surface!";
        }
        m_PhysicalDevice.PickBestDevice(m_Instance, m_Surface.GetHandle());
        if (!m_PhysicalDevice.IsFound()) {
            return "Failed to find a suitable Vulkan Physical Device!";
        }

        m_MSAASamples = m_PhysicalDevice.GetMaxUsableSampleCount();

        if (!m_LogicalDevice.Init(m_PhysicalDevice, m_Surface, Vulkan::GetDeviceExtensions(),
                debugMode ? Vulkan::GetValidationLayers() : std::vector<const char*>())) {
            return "Failed to initialize Vulkan Logical Device!";
        }

        if (!m_CommandPool.Init(m_LogicalDevice)) {
            return "Failed to initialize Vulkan Command Pool!";
        }

        auto result = CreateSwapchain();
        if (result.IsErr()) {
            return result.UnwrapErr();
        }
        m_Frames.resize(
            m_Swapchain.ImageCount()); // TODO(Feature): We can have custom resource pool sizes


        if (m_GraphicsExtent.width == 0 || m_GraphicsExtent.height == 0) {
            m_GraphicsExtent = m_Swapchain.GetExtent();
        }

        result = CreateMSAATextureImage();
        if (result.IsErr()) {
            return result.UnwrapErr();
        }

        result = CreateDepthTextureImage();
        if (result.IsErr()) {
            return result.UnwrapErr();
        }

        std::vector<VkAttachmentDescription> renderPassAttachments(3);
        VkAttachmentDescription&             colorAttachment = renderPassAttachments[0];
        colorAttachment.samples                              = m_MSAASamples;
        colorAttachment.storeOp                              = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp                        = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp                       = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        VkAttachmentDescription& depthAttachment = renderPassAttachments[1];
        depthAttachment.format                   = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples                  = m_MSAASamples;
        depthAttachment.loadOp                   = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp                  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        VkAttachmentDescription& colorAttachmentResolve = renderPassAttachments[2];
        colorAttachmentResolve.samples                  = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp                   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        {
            // Main render pass
            Vulkan::RenderPassInfo renderPassInfo {};
            renderPassInfo.Attachments = renderPassAttachments; // Copy the attachments

            renderPassInfo.Attachments[0].format        = m_Swapchain.ImageFormat();
            renderPassInfo.Attachments[0].loadOp        = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPassInfo.Attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            renderPassInfo.Attachments[0].finalLayout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            renderPassInfo.Attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPassInfo.Attachments[1].finalLayout =
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            renderPassInfo.Attachments[2].format        = m_Swapchain.ImageFormat();
            renderPassInfo.Attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPassInfo.Attachments[2].finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            renderPassInfo.Subpasses = {
                Vulkan::RenderSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS,
                    {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
                    {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
                    {{2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}
                    )
            };

            if (!m_RenderPass.Init(m_LogicalDevice, renderPassInfo)) {
                return "Failed to initialize Vulkan Render Pass!";
            }
        }

        {
            // Graphics render pass
            Vulkan::RenderPassInfo renderPassInfo {};

            renderPassInfo.Attachments = renderPassAttachments; // Copy the attachments

            renderPassInfo.Attachments[0].format        = m_Swapchain.ImageFormat();
            renderPassInfo.Attachments[0].loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR;
            renderPassInfo.Attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPassInfo.Attachments[0].finalLayout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            renderPassInfo.Attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPassInfo.Attachments[1].finalLayout =
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            renderPassInfo.Attachments[2].format        = m_Swapchain.ImageFormat();
            renderPassInfo.Attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPassInfo.Attachments[2].finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            renderPassInfo.Subpasses = {
                Vulkan::RenderSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS,
                    {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
                    {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
                    {{2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}}
                    ),
            };

            if (!m_GraphicsRenderPass.Init(m_LogicalDevice, renderPassInfo)) {
                return "Failed to initialize Vulkan Graphics Render Pass!";
            }
        }

        Vulkan::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo.poolSizes = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         256},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 256},
        };
        descriptorPoolCreateInfo.maxSets = 256;
        if (!m_DescriptorPool.Init(m_LogicalDevice, descriptorPoolCreateInfo)) {
            return "Failed to initialize Vulkan Descriptor Pool!";
        }

        result = CreateImageViewsAndFramebuffers();
        if (result.IsErr()) {
            return result.UnwrapErr();
        }

        for (std::size_t i = 0; i < m_Frames.size(); ++i) {
            auto& frame = m_Frames[i];
            if (!frame.CommandBuffer.Init(m_LogicalDevice, m_CommandPool)) {
                return "Failed to initialize Vulkan Command Buffer!";
            }
            if (!frame.ImageAvailableSemaphore.Init(m_LogicalDevice)) {
                return "Failed to initialize Vulkan Image Available Semaphore!";
            }
            if (!frame.RenderFinishedSemaphore.Init(m_LogicalDevice)) {
                return "Failed to initialize Vulkan Render Finished Semaphore!";
            }
            if (!frame.InFlightFence.Init(m_LogicalDevice)) {
                return "Failed to initialize Vulkan In Flight Fence!";
            }

            // TODO(ImportantFeat): We are not detecting changes in the window size or the graphics extent
            if (!frame.GraphicsTextureImage.Init(m_LogicalDevice, m_CommandPool, m_PhysicalDevice,
                    m_GraphicsExtent, m_Swapchain.ImageFormat(), VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
                        | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SAMPLE_COUNT_1_BIT)) {
                return "Failed to initialize Vulkan Graphics Texture Image!";
            }

            if (!frame.GraphicsFrameBuffer.Init(m_LogicalDevice, m_GraphicsRenderPass,
                    {m_MSAATextureImage.GetImageView(), m_DepthTextureImage.GetImageView(),
                        frame.GraphicsTextureImage.GetImageView()},
                    m_GraphicsExtent)) {
                return "Failed to initialize Vulkan Graphics Frame Buffer!";
            }
        }

        ASTRELIS_CORE_LOG_INFO("Vulkan Graphics Context initialized!");
        m_IsInitialized = true;

        ASTRELIS_PROFILE_VULKAN(
            if (m_ProfileCommandBuffer.Init(m_LogicalDevice, m_CommandPool)) {
                if (m_LogicalDevice.GetGraphicsQueue() == m_LogicalDevice.GetPresentQueue()) {
                    m_TracyVkCtx.emplace_back(TracyVkContext(m_PhysicalDevice.GetHandle(),
                        m_LogicalDevice.GetHandle(), m_LogicalDevice.GetGraphicsQueue(),
                        m_ProfileCommandBuffer.GetHandle()));
                }
                else {
                    m_TracyVkCtx.emplace_back(
                        TracyVkContext(m_PhysicalDevice.GetHandle(), m_LogicalDevice.GetHandle(),
                            m_LogicalDevice.GetGraphicsQueue() m_ProfileCommandBuffer.GetHandle()));
                    m_TracyVkCtx.emplace_back(
                        TracyVkContext(m_PhysicalDevice.GetHandle(), m_LogicalDevice.GetHandle(),
                            m_LogicalDevice.GetPresentQueue(), m_ProfileCommandBuffer.GetHandle()));
                }
            } else {
                ASTRELIS_CORE_LOG_WARN("Failed to initialize command buffer for Profiling!");
            })

        return EmptyType {};
    }

    Result<EmptyType, std::string> VulkanGraphicsContext::CreateSwapchain() {
        std::uint32_t swapChainFrameCount = m_MaxFramesInFlight;
        if (!m_Swapchain.Init(m_Window, m_PhysicalDevice, m_LogicalDevice, m_Surface,
                swapChainFrameCount, m_VSync)) {
            return "Failed to initialize Vulkan Swap Chain!";
        }
        if (swapChainFrameCount != m_MaxFramesInFlight) {
            ASTRELIS_CORE_LOG_WARN(
                "Requested frame count is not supported, using {0} (instead of {1})",
                swapChainFrameCount, m_MaxFramesInFlight);
        }
        m_SwapChainFrames.resize(m_Swapchain.ImageCount());

        VkCommandBuffer commandBuffer =
            Vulkan::BeginSingleTimeCommands(m_LogicalDevice.GetHandle(), m_CommandPool.GetHandle());
        for (std::uint32_t i = 0; i < m_SwapChainFrames.size(); i++) {
            // Transition swapchain to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            Vulkan::TransitionImageLayout(commandBuffer, m_Swapchain[i], m_Swapchain.ImageFormat(),
                VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        }
        Vulkan::EndSingleTimeCommands(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(), commandBuffer);

        return EmptyType {};
    }

    Result<EmptyType, std::string> VulkanGraphicsContext::CreateDepthTextureImage() {
        const auto depthFormat = VK_FORMAT_D32_SFLOAT;
        if (!m_DepthTextureImage.Init(m_LogicalDevice, m_CommandPool, m_PhysicalDevice,
                m_Swapchain.GetExtent(), depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_MSAASamples)) {
            return "Failed to initialize Vulkan Depth Texture Image!";
        }

        // TODO(Cleanup): We can use one command buffer for all transitions
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(),
            m_DepthTextureImage.GetImage(), VK_FORMAT_D32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        return EmptyType {};
    }

    Result<EmptyType, std::string> VulkanGraphicsContext::CreateMSAATextureImage() {
        if (!m_MSAATextureImage.Init(m_LogicalDevice, m_CommandPool, m_PhysicalDevice,
                m_Swapchain.GetExtent(), m_Swapchain.ImageFormat(), VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_MSAASamples)) {
            return "Failed to initialize Vulkan MSAA Texture Image!";
        }

        return EmptyType {};
    }

    Result<EmptyType, std::string> VulkanGraphicsContext::CreateImageViewsAndFramebuffers() {
        for (std::size_t i = 0; i < m_SwapChainFrames.size(); ++i) {
            auto& frame = m_SwapChainFrames[i];
            if (!frame.ImageView.Init(m_LogicalDevice, m_Swapchain[i], m_Swapchain.ImageFormat())) {
                return "Failed to initialize Vulkan Image View!";
            }
            if (!frame.FrameBuffer.Init(m_LogicalDevice, m_RenderPass,
                    {m_MSAATextureImage.GetImageView(), m_DepthTextureImage.GetImageView(),
                        frame.ImageView.GetHandle()},
                    m_Swapchain.GetExtent())) {
                return "Failed to initialize Vulkan Frame Buffer!";
            }
        }

        return EmptyType {};
    }

    void VulkanGraphicsContext::Shutdown() {
        ASTRELIS_PROFILE_FUNCTION();
        vkDeviceWaitIdle(m_LogicalDevice.GetHandle());

        ASTRELIS_PROFILE_VULKAN(TracyVkDestroy(m_TracyVkCtx);)

        if (!m_IsInitialized) {
            ASTRELIS_CORE_LOG_WARN("Vulkan Graphics Context already shutdown!");
            return;
        }

        for (auto& frame : m_Frames) {
            frame.CommandBuffer.Destroy(m_LogicalDevice, m_CommandPool);
            frame.ImageAvailableSemaphore.Destroy(m_LogicalDevice);
            frame.RenderFinishedSemaphore.Destroy(m_LogicalDevice);
            frame.InFlightFence.Destroy(m_LogicalDevice);
        }

        m_DepthTextureImage.Destroy(m_LogicalDevice);
        m_MSAATextureImage.Destroy(m_LogicalDevice);

        for (auto& frame : m_SwapChainFrames) {
            frame.ImageView.Destroy(m_LogicalDevice);
            frame.FrameBuffer.Destroy(m_LogicalDevice);
        }

        for (auto& frame : m_Frames) {
            frame.GraphicsTextureImage.Destroy(m_LogicalDevice);
            frame.GraphicsFrameBuffer.Destroy(m_LogicalDevice);
        }

        m_DescriptorPool.Destroy(m_LogicalDevice);
        m_RenderPass.Destroy(m_LogicalDevice);
        m_GraphicsRenderPass.Destroy(m_LogicalDevice);
        m_CommandPool.Destroy(m_LogicalDevice);
        m_Swapchain.Destroy(m_LogicalDevice);
        m_LogicalDevice.Destroy();
        m_Surface.Destroy(m_Instance);
        // Physical device doesn't need to be destroyed
        if (GlobalConfig::IsDebugMode()) {
            m_DebugMessenger.Destroy(m_Instance);
        }
        m_Instance.Destroy();
    }

    void VulkanGraphicsContext::BeginFrame() {
        ASTRELIS_PROFILE_FUNCTION();
        auto& frame = GetCurrentFrame();

        {
            ASTRELIS_PROFILE_SCOPE("Wait for frame");
            frame.InFlightFence.Wait(m_LogicalDevice, std::numeric_limits<std::uint64_t>::max());
        }

        {
            ASTRELIS_PROFILE_SCOPE("Acquire next image");
            VkResult result = vkAcquireNextImageKHR(m_LogicalDevice.GetHandle(),
                m_Swapchain.GetHandle(), std::numeric_limits<std::uint64_t>::max(),
                frame.ImageAvailableSemaphore.GetHandle(), VK_NULL_HANDLE, &m_ImageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR || m_SwapchainRecreation) {
                VkSubmitInfo submitInfo                   = {};
                submitInfo.sType                          = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                std::array<VkSemaphore, 1> waitSemaphores = {
                    frame.ImageAvailableSemaphore.GetHandle()};
                submitInfo.waitSemaphoreCount = static_cast<std::uint32_t>(waitSemaphores.size());
                submitInfo.pWaitSemaphores    = waitSemaphores.data();
                std::array<VkPipelineStageFlags, 1> waitStages = {
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
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
            else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                ASTRELIS_CORE_LOG_ERROR("Failed to acquire swap chain image!");
            }
            else {
                frame.InFlightFence.Reset(m_LogicalDevice);
            }
        }

        frame.CommandBuffer.Reset();
        frame.CommandBuffer.Begin();
        ASTRELIS_PROFILE_VULKAN(
            TracyVkZone(m_TracyVkCtx, frame.CommandBuffer.GetHandle(), "Frame");)
    }

    void VulkanGraphicsContext::EndFrame() {
        ASTRELIS_PROFILE_FUNCTION();
        auto& frame = GetCurrentFrame();
        ASTRELIS_PROFILE_VULKAN(TracyVkCollect(m_TracyVkCtx, frame.CommandBuffer.GetHandle());)
        frame.CommandBuffer.End();

        if (m_SkipFrame) {
            m_SkipFrame = false;
            frame.CommandBuffer.Reset();
            return;
        }

        {
            ASTRELIS_PROFILE_SCOPE("Submit frame");
            frame.CommandBuffer.Submit(m_LogicalDevice, m_LogicalDevice.GetGraphicsQueue(),
                frame.ImageAvailableSemaphore, frame.RenderFinishedSemaphore, frame.InFlightFence);
        }

        {
            ASTRELIS_PROFILE_SCOPE("Present frame");
            std::array<VkSemaphore, 1> presentWaitSemaphores = {
                frame.RenderFinishedSemaphore.GetHandle()};
            std::array<VkSwapchainKHR, 1> swapChains  = {m_Swapchain.GetHandle()};
            VkPresentInfoKHR              presentInfo = {};
            presentInfo.sType                         = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount            = 1;
            presentInfo.pWaitSemaphores               = presentWaitSemaphores.data();
            presentInfo.swapchainCount = static_cast<std::uint32_t>(swapChains.size());
            presentInfo.pSwapchains    = swapChains.data();
            presentInfo.pImageIndices  = &m_ImageIndex;

            if (m_CaptureNextFrame) {
                frame.InFlightFence.Wait(
                    m_LogicalDevice, std::numeric_limits<std::uint64_t>::max());

                m_CapturePromise.set_value(CaptureScreen());
                m_CaptureNextFrame = false;
            }

            auto result = vkQueuePresentKHR(m_LogicalDevice.GetPresentQueue(), &presentInfo);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
                || m_SwapchainRecreation) {
                RecreateSwapChain();
            }
            else if (result != VK_SUCCESS) {
                ASTRELIS_CORE_LOG_ERROR("Failed to present swap chain image!");
            }
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
    }

    void VulkanGraphicsContext::RecreateSwapChain() {
        ASTRELIS_PROFILE_FUNCTION();
        int width  = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(m_Window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_LogicalDevice.GetHandle());

        // We have to do in this order:
        // Framebuffers -> Color resources -> Depth resources -> Image views -> Swap chain

        for (auto& frame : m_Frames) {
            frame.GraphicsFrameBuffer.Destroy(m_LogicalDevice);
            frame.GraphicsTextureImage.Destroy(m_LogicalDevice);
        }

        for (auto& swapchainFrame : m_SwapChainFrames) {
            swapchainFrame.FrameBuffer.Destroy(m_LogicalDevice);
            swapchainFrame.ImageView.Destroy(m_LogicalDevice);
        }

        m_MSAATextureImage.Destroy(m_LogicalDevice);
        m_DepthTextureImage.Destroy(m_LogicalDevice);

        vkDestroySwapchainKHR(m_LogicalDevice.GetHandle(), m_Swapchain.GetHandle(), nullptr);

        auto result = CreateSwapchain();
        if (result.IsErr()) {
            ASTRELIS_CORE_LOG_ERROR("Failed to recreate swapchain: {0}", result.UnwrapErr());
            return;
        }

        // TODO(Feat): We need to know if the user actually wants it to resize with the window
        m_GraphicsExtent = m_Swapchain.GetExtent();

        result = CreateMSAATextureImage();
        if (result.IsErr()) {
            ASTRELIS_CORE_LOG_ERROR(
                "Failed to recreate MSAA texture image: {0}", result.UnwrapErr());
            return;
        }

        result = CreateDepthTextureImage();
        if (result.IsErr()) {
            ASTRELIS_CORE_LOG_ERROR(
                "Failed to recreate depth texture image: {0}", result.UnwrapErr());
            return;
        }

        result = CreateImageViewsAndFramebuffers();
        if (result.IsErr()) {
            ASTRELIS_CORE_LOG_ERROR(
                "Failed to recreate image views and framebuffers: {0}", result.UnwrapErr());
            return;
        }

        for (std::size_t i = 0; i < m_Frames.size(); ++i) {
            auto& frame = m_Frames[i];
            // TODO(ImportantFeat): We are not detecting changes in the window size or the graphics extent
            if (!frame.GraphicsTextureImage.Init(m_LogicalDevice, m_CommandPool, m_PhysicalDevice,
                    m_GraphicsExtent, m_Swapchain.ImageFormat(), VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
                        | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SAMPLE_COUNT_1_BIT)) {
                ASTRELIS_CORE_LOG_ERROR("Failed to initialize Vulkan Graphics Texture Image!");
            }

            if (!frame.GraphicsFrameBuffer.Init(m_LogicalDevice, m_GraphicsRenderPass,
                    {m_MSAATextureImage.GetImageView(), m_DepthTextureImage.GetImageView(),
                        frame.GraphicsTextureImage.GetImageView()},
                    m_GraphicsExtent)) {
                ASTRELIS_CORE_LOG_ERROR("Failed to initialize Vulkan Graphics Frame Buffer!");
            }
        }

        m_SwapchainRecreation = false;
    }

    InMemoryImage VulkanGraphicsContext::CaptureScreen() const {
        ASTRELIS_PROFILE_FUNCTION();
        constexpr VkFormat    format        = VK_FORMAT_R8G8B8A8_SRGB;
        constexpr std::size_t bytesPerPixel = 4;
        std::int32_t          dstWidth  = static_cast<std::int32_t>(m_CaptureOutputExtent.width);
        std::int32_t          dstHeight = static_cast<std::int32_t>(m_CaptureOutputExtent.height);
        if (dstWidth <= 0 || dstHeight <= 0) {
            dstWidth  = static_cast<std::int32_t>(m_Swapchain.GetExtent().width);
            dstHeight = static_cast<std::int32_t>(m_Swapchain.GetExtent().height);
        }
        VkImage        image {};
        VkDeviceMemory imageMemory {};
        Vulkan::CreateImage(m_PhysicalDevice.GetHandle(), m_LogicalDevice.GetHandle(), dstWidth,
            dstHeight, format, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            VK_SAMPLE_COUNT_1_BIT, image, imageMemory);

        // Transition the image layout to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(), image,
            m_Swapchain.ImageFormat(), VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Transition swapchain to VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(),
            m_Swapchain[m_ImageIndex], m_Swapchain.ImageFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        // Blit the image to the output
        VkImageBlit blit {};
        blit.srcOffsets[0]             = {0, 0, 0};
        blit.srcOffsets[1]             = {static_cast<std::int32_t>(m_Swapchain.GetExtent().width),
                        static_cast<std::int32_t>(m_Swapchain.GetExtent().height), 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel   = 0;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount     = 1;
        blit.dstOffsets[0]                 = {0, 0, 0};
        blit.dstOffsets[1]                 = {dstWidth, dstHeight, 1};
        blit.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel       = 0;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount     = 1;

        auto* cmdBuffer =
            Vulkan::BeginSingleTimeCommands(m_LogicalDevice.GetHandle(), m_CommandPool.GetHandle());
        vkCmdBlitImage(cmdBuffer, m_Swapchain[m_ImageIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        Vulkan::EndSingleTimeCommands(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(), cmdBuffer);

        // Transition the image layout to VK_IMAGE_LAYOUT_GENERAL
        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(), image,
            m_Swapchain.ImageFormat(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_GENERAL);

        // We need to query the Subresource layout to get the row pitch

        VkImageSubresource subResource {};
        subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subResource.mipLevel   = 0;
        subResource.arrayLayer = 0;
        VkSubresourceLayout subResourceLayout {};
        vkGetImageSubresourceLayout(
            m_LogicalDevice.GetHandle(), image, &subResource, &subResourceLayout);

        // Map the image memory and copy the data to an InMemoryImage
        void* data = nullptr;
        vkMapMemory(m_LogicalDevice.GetHandle(), imageMemory, 0, VK_WHOLE_SIZE, 0, &data);

        std::vector<std::byte> imageData(
            bytesPerPixel * m_Swapchain.GetExtent().width * m_Swapchain.GetExtent().height);

        for (std::uint32_t row = 0; row < static_cast<std::uint32_t>(dstHeight); ++row) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            std::memcpy(imageData.data() + bytesPerPixel * row * dstWidth,
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                static_cast<std::byte*>(data) + row * subResourceLayout.rowPitch,
                bytesPerPixel * dstWidth);
        }

        // Cleanup
        vkUnmapMemory(m_LogicalDevice.GetHandle(), imageMemory);
        vkDestroyImage(m_LogicalDevice.GetHandle(), image, nullptr);
        vkFreeMemory(m_LogicalDevice.GetHandle(), imageMemory, nullptr);

        Vulkan::TransitionImageLayout(m_LogicalDevice.GetHandle(),
            m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetHandle(),
            m_Swapchain[m_ImageIndex], m_Swapchain.ImageFormat(),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        return InMemoryImage(dstWidth, dstHeight, 4, imageData);
    }

    RefPtr<VulkanGraphicsContext> VulkanGraphicsContext::Create(
        RawRef<GLFWwindow*> window, ContextProps props) {
        ASTRELIS_PROFILE_FUNCTION();
        ASTRELIS_CORE_ASSERT(window, "Window is nullptr!");
        auto context     = RefPtr<VulkanGraphicsContext>::Create(window);
        context->m_VSync = props.VSync;
        return context;
    }
} // namespace Astrelis
