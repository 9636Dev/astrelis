#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/TextureImage.hpp"
#include "NebulaEngine/Scene/Scene.hpp"

#include "Camera.hpp"
#include "GraphicsContext.hpp"
#include "RendererAPI.hpp"
#include "RendererStorage.hpp"

#include <glm/glm.hpp>

namespace Nebula
{
    struct UniformBufferObject
    {
        glm::mat4 Model      = glm::mat4(1.0F);
        glm::mat4 View       = glm::mat4(1.0F);
        glm::mat4 Projection = glm::mat4(1.0F);
    };

    class Renderer2D
    {
    public:
        explicit Renderer2D(RefPtr<Window> window, Bounds viewport);
        ~Renderer2D()                            = default;
        Renderer2D(const Renderer2D&)            = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        Renderer2D(Renderer2D&&)                 = delete;
        Renderer2D& operator=(Renderer2D&&)      = delete;

        bool Init();
        void Shutdown();

        void BeginFrame();
        void RenderScene(Scene& scene, Camera& camera);
        void EndFrame();

        void ResizeViewport();

        Renderer2DStorage& GetStorage() { return m_Storage; }
    private:
        RefPtr<Window> m_Window;
        RefPtr<GraphicsContext> m_Context;
        RefPtr<RendererAPI> m_RendererAPI;
        Renderer2DStorage m_Storage;

        UniformBufferObject m_UBO;
        RefPtr<DescriptorSetLayout> m_DescriptorSetLayout;
        RefPtr<DescriptorSets> m_DescriptorSets;
        RefPtr<UniformBuffer> m_UniformBuffer;
        RefPtr<TextureImage> m_TextureImage;
        RefPtr<TextureSampler> m_TextureSampler;
        std::uint32_t m_DescriptorCount;
    };

} // namespace Nebula
