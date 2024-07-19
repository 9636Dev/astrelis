#include <cstdlib>
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

using Nebula::RefPtr;

struct Renderer2DTestParams
{
    std::string Name;
    mutable RefPtr<Nebula::Scene2D> Scene;
};

Nebula::RefPtr<Nebula::Window> g_Window;

// Parameterized test fixture
class Renderer2DTest : public ::testing::TestWithParam<Renderer2DTestParams>
{
public:
    Renderer2DTest()
    {
        static bool initialized = false;
        if (!initialized)
        {
            Nebula::Log::Init(Nebula::Log::LogMode::CoreOnly, spdlog::level::err);
            Nebula::GlobalConfig::SetDebugMode(false);
            Nebula::RendererAPI::SetBufferingMode(Nebula::RendererAPI::BufferingMode::Single);
            Nebula::WindowProps props("TestWindow", 100, 100, false);
            auto res = Nebula::Window::Create(props);
            if (res.IsErr())
            {
                throw std::runtime_error("Failed to create Window");
            }
            g_Window    = res.Unwrap();

            initialized = true;
        }
        m_Window       = g_Window;
        m_RenderSystem = Nebula::RenderSystem::Create(m_Window);
        if (!m_RenderSystem->Init())
        {
            throw std::runtime_error("Failed to initialize RenderSystem");
        }

        float windowHeight = 100.0F;
        float renderableHeight = static_cast<float>(m_RenderSystem->GetRenderBounds().Height);

        if (renderableHeight < windowHeight)
        {
            m_PixelThreshold = static_cast<std::int32_t>(renderableHeight / windowHeight * 25.0F);
        }
        else
        {
            m_PixelThreshold = static_cast<std::int32_t>(windowHeight / renderableHeight * 25.0F);
        }

    }

    ~Renderer2DTest() override {
        m_RenderSystem->Shutdown();
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
    std::int32_t m_PixelThreshold;

    // These persist for each test case
    RefPtr<Nebula::Renderer2D> m_Renderer2D;
};

void CompareImages(const Nebula::InMemoryImage& actual, const Nebula::InMemoryImage& expected, std::int32_t threshold)
{
    ASSERT_EQ(actual.GetWidth(), expected.GetWidth());
    ASSERT_EQ(actual.GetHeight(), expected.GetHeight());
    ASSERT_EQ(actual.GetChannels(), expected.GetChannels());

    // We use a SSIM threshold of 0.80 to compare the images
    float similarity = actual.Similarity(expected, Nebula::InMemoryImage::CompareMethod::SSIM);

    // In the future we will write our own CPU downscaler and avoid using Vulkan blit, for more accurate downscaling, and higher SSIM threshold
    EXPECT_GE(similarity, 0.80F) << "Images are not similar enough, SSIM: " << similarity;
}


// Parameterized test case
TEST_P(Renderer2DTest, TestDrawScene)
{
    const auto& params = GetParam();

    Nebula::FrameCaptureProps props { 100, 100 };
    auto future = m_RenderSystem->CaptureFrame(props);
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
    CompareImages(image, referenceImage, m_PixelThreshold);
}

INSTANTIATE_TEST_SUITE_P(
    Renderer2DTest,
    Renderer2DTest,
    ::testing::Values(Renderer2DTestParams {"EmptyScene", RefPtr<Nebula::Scene2D>::Create()},
                      Renderer2DTestParams {"SingleSprite",
                                            []() {
                                                auto scene  = RefPtr<Nebula::Scene2D>::Create();
                                                auto entity = scene->CreateEntity();
                                                scene->AddComponent(entity, Nebula::TransformComponent());
                                                return scene;
                                            }()},
                      Renderer2DTestParams {"MultipleSprites",
                                            []() {
                                                auto scene = RefPtr<Nebula::Scene2D>::Create();
                                                for (int i = 0; i < 10; i++)
                                                {
                                                    auto entity = scene->CreateEntity();
                                                    Nebula::TransformComponent transform;
                                                    transform.Transform = Nebula::Math::Translate(
                                                        Nebula::Mat4f(1.0F), Nebula::Vec3f((i - 5) * 0.1F, 0.0F, 0.0F));
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
                                                    transform.Transform = Nebula::Math::Translate(
                                                        Nebula::Mat4f(1.0F),
                                                        Nebula::Vec3f((i - 5) * 0.1F, 0.0F, -1 + 0.1F * i));
                                                    scene->AddComponent(entity, transform);
                                                }
                                                return scene;
                                            }()}));


