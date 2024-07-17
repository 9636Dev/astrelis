#include <gtest/gtest.h>

#include <iostream>

#include "NebulaEngine/Core/GlobalConfig.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/IO/File.hpp"
#include "NebulaEngine/Renderer/Camera.hpp"
#include "NebulaEngine/Renderer/RenderSystem.hpp"
#include "NebulaEngine/Renderer/Renderer2D.hpp"
#include "NebulaEngine/Scene/TransformComponent.hpp"
#include "glm/ext/matrix_transform.hpp"

using Nebula::RefPtr;

struct Renderer2DTestParams
{
    std::string Name;
    mutable RefPtr<Nebula::Scene2D> Scene;
};

// Parameterized test fixture
class Renderer2DTest : public ::testing::TestWithParam<Renderer2DTestParams>
{
public:
    Renderer2DTest()
    {
        Nebula::Log::Init(Nebula::Log::LogMode::CoreOnly, spdlog::level::err);
        Nebula::GlobalConfig::SetDebugMode(false);
        Nebula::RendererAPI::SetBufferingMode(Nebula::RendererAPI::BufferingMode::Single);
        Nebula::WindowProps props("TestWindow", 100, 100, 1);
        auto res = Nebula::Window::Create(props);
        if (res.IsErr())
        {
            throw std::runtime_error("Failed to create Window");
        }
        m_Window       = res.Unwrap();
        m_RenderSystem = Nebula::RenderSystem::Create(m_Window);
        if (!m_RenderSystem->Init())
        {
            throw std::runtime_error("Failed to initialize RenderSystem");
        }
    }

    ~Renderer2DTest() override
    {
        m_RenderSystem->Shutdown();
        m_Window.Reset();
    }

    Renderer2DTest(const Renderer2DTest&)            = delete;
    Renderer2DTest& operator=(const Renderer2DTest&) = delete;
    Renderer2DTest(Renderer2DTest&&)                 = delete;
    Renderer2DTest& operator=(Renderer2DTest&&)      = delete;

    void SetUp() override
    {
        m_Renderer2D = RefPtr<Nebula::Renderer2D>::Create(m_Window, m_Window->GetViewportBounds());
        if (!m_Renderer2D->Init())
        {
            throw std::runtime_error("Failed to initialize Renderer2D");
        }
    }

    void TearDown() override
    {
        m_Renderer2D->Shutdown();
        m_Renderer2D.Reset();
    }

    // These persist for the entire test suite
    RefPtr<Nebula::Window> m_Window;
    RefPtr<Nebula::RenderSystem> m_RenderSystem;

    // These persist for each test case
    RefPtr<Nebula::Renderer2D> m_Renderer2D;
};

// Parameterized test case
TEST_P(Renderer2DTest, TestDrawScene)
{
    const auto& params = GetParam();

    m_Window->BeginFrame();
    m_RenderSystem->StartGraphicsRenderPass();
    m_Renderer2D->BeginFrame();
    Nebula::Camera camera;
    m_Renderer2D->RenderScene(*params.Scene, camera);
    m_Renderer2D->EndFrame();
    m_RenderSystem->EndGraphicsRenderPass();
    m_RenderSystem->BlitSwapchain();
    m_RenderSystem->EndFrame();
    m_Window->EndFrame();
    m_Window->OnUpdate();
    auto future = m_RenderSystem->CaptureFrame();

    // 1 frame in flight, just render next frame
    m_Window->BeginFrame();
    m_RenderSystem->StartGraphicsRenderPass();
    m_Renderer2D->BeginFrame();
    m_Renderer2D->RenderScene(*params.Scene, camera);
    m_Renderer2D->EndFrame();
    m_RenderSystem->EndGraphicsRenderPass();
    m_RenderSystem->BlitSwapchain();
    m_RenderSystem->EndFrame();
    m_Window->EndFrame();
    m_Window->OnUpdate();

    ASSERT_TRUE(future.valid());
    ASSERT_EQ(future.wait_for(std::chrono::milliseconds(0)), std::future_status::ready);

    auto image = future.get();
    Nebula::File file("tests/Renderer2DTest_" + params.Name + ".png");
    if (!file.Exists())
    {
        std::cout << "First run, saving image to " << file.GetPath() << std::endl;
        image.Save(file.GetPath());
        return;
    }

    Nebula::InMemoryImage referenceImage = Nebula::InMemoryImage(file);
    ASSERT_TRUE(image == referenceImage);
}

INSTANTIATE_TEST_SUITE_P(Renderer2DTest,
                         Renderer2DTest,
                         ::testing::Values(Renderer2DTestParams {"EmptyScene", RefPtr<Nebula::Scene2D>::Create()},
                                           Renderer2DTestParams {"SingleSprite",
                                                                 []() {
                                                                     auto scene  = RefPtr<Nebula::Scene2D>::Create();
                                                                     auto entity = scene->CreateEntity();
                                                                     scene->AddComponent(entity,
                                                                                         Nebula::TransformComponent());
                                                                     return scene;
                                                                 }()},
                                           Renderer2DTestParams {"MultipleSprites", []() {
                                                                     auto scene = RefPtr<Nebula::Scene2D>::Create();
                                                                     for (int i = 0; i < 10; i++)
                                                                     {
                                                                         auto entity = scene->CreateEntity();
                                                                         Nebula::TransformComponent transform;
                                                                         transform.Transform = glm::translate(
                                                                             glm::mat4(1.0F),
                                                                             glm::vec3((i - 5) * 0.1F, 0.0F, 0.0F));
                                                                         scene->AddComponent(entity, transform);
                                                                     }
                                                                     return scene;
                                                                 }()},
                                           Renderer2DTestParams {"DepthTest", []() {
                                                                     auto scene = RefPtr<Nebula::Scene2D>::Create();
                                                                     for (int i = 0; i < 10; i++)
                                                                     {
                                                                         auto entity = scene->CreateEntity();
                                                                         Nebula::TransformComponent transform;
                                                                         transform.Transform = glm::translate(
                                                                             glm::mat4(1.0F),
                                                                             glm::vec3((i - 5) * 0.1F, 0.0F, -1 + 0.1F * i));
                                                                         scene->AddComponent(entity, transform);
                                                                     }
                                                                     return scene;
                                                                 }()}
                                           ));


