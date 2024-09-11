#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/IO/File.hpp"
#include "Astrelis/Renderer/Camera.hpp"
#include "Astrelis/Renderer/RenderSystem.hpp"
#include "Astrelis/Renderer/Renderer2D.hpp"

using Astrelis::RefPtr;

struct Renderer2DTestParams
{
    std::string Name;
    //mutable RefPtr<Astrelis::Scene2D> Scene;
};

Astrelis::RefPtr<Astrelis::Window> g_Window;

// Parameterized test fixture
class Renderer2DTest : public ::testing::TestWithParam<Renderer2DTestParams>
{
public:
    Renderer2DTest()
    {
        static bool initialized = false;
        if (!initialized)
        {
            Astrelis::Log::Init(Astrelis::Log::LogMode::CoreOnly, spdlog::level::err);
            Astrelis::GlobalConfig::SetDebugMode(false);
            Astrelis::RendererAPI::SetBufferingMode(Astrelis::RendererAPI::BufferingMode::Single);
            Astrelis::WindowProps props("TestWindow", {100, 100}, false);
            auto res = Astrelis::Window::Create(props);
            if (res.IsErr())
            {
                throw std::runtime_error("Failed to create Window");
            }
            g_Window = res.Unwrap();

            initialized = true;
        }
        m_Window       = g_Window;
        m_RenderSystem = Astrelis::RenderSystem::Create(m_Window);
        if (!m_RenderSystem->Init())
        {
            throw std::runtime_error("Failed to initialize RenderSystem");
        }

        float windowHeight     = 100.0F;
        float renderableHeight = static_cast<float>(m_RenderSystem->GetRenderBounds().Height());

        if (renderableHeight < windowHeight)
        {
            m_PixelThreshold = static_cast<std::int32_t>(renderableHeight / windowHeight * 25.0F);
        }
        else
        {
            m_PixelThreshold = static_cast<std::int32_t>(windowHeight / renderableHeight * 25.0F);
        }
    }

    ~Renderer2DTest() override { m_RenderSystem->Shutdown(); }

    Renderer2DTest(const Renderer2DTest&)            = delete;
    Renderer2DTest& operator=(const Renderer2DTest&) = delete;
    Renderer2DTest(Renderer2DTest&&)                 = delete;
    Renderer2DTest& operator=(Renderer2DTest&&)      = delete;

    void SetUp() override
    {
        m_Renderer2D = RefPtr<Astrelis::Renderer2D>::Create(m_Window, m_Window->GetViewportBounds());
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
    RefPtr<Astrelis::Window> m_Window;
    RefPtr<Astrelis::RenderSystem> m_RenderSystem;
    std::int32_t m_PixelThreshold;

    // These persist for each test case
    RefPtr<Astrelis::Renderer2D> m_Renderer2D;
};

void CompareImages(const Astrelis::InMemoryImage& actual,
                   const Astrelis::InMemoryImage& expected,
                   std::int32_t threshold)
{
    (void)threshold;
    ASSERT_EQ(actual.GetWidth(), expected.GetWidth());
    ASSERT_EQ(actual.GetHeight(), expected.GetHeight());
    ASSERT_EQ(actual.GetChannels(), expected.GetChannels());

    // We use a SSIM threshold of 0.80 to compare the images
    float similarity = actual.Similarity(expected, Astrelis::InMemoryImage::CompareMethod::SSIM);

    // In the future we will write our own CPU downscaler and avoid using Vulkan blit, for more accurate downscaling, and higher SSIM threshold
    EXPECT_GE(similarity, 0.80F) << "Images are not similar enough, SSIM: " << similarity;
}

// Parameterized test case
TEST_P(Renderer2DTest, TestDrawScene)
{
    const auto& params = GetParam();

    Astrelis::FrameCaptureProps props {100, 100};
    auto future = m_RenderSystem->CaptureFrame(props);
    m_Window->BeginFrame();
    m_RenderSystem->StartGraphicsRenderPass();
    m_Renderer2D->BeginFrame();
    Astrelis::Camera camera;
    m_Renderer2D->EndFrame();
    m_RenderSystem->EndGraphicsRenderPass();
    m_RenderSystem->BlitSwapchain();
    m_RenderSystem->EndFrame();
    m_Window->EndFrame();
    m_Window->OnUpdate();

    ASSERT_TRUE(future.valid());
    ASSERT_EQ(future.wait_for(std::chrono::milliseconds(0)), std::future_status::ready);

    auto image = future.get();
    Astrelis::File file("tests/Renderer2DTest_" + params.Name + ".png");
    if (!file.Exists())
    {
        std::cout << "First run, saving image to " << file.GetPath() << std::endl;
        image.Save(file.GetPath());
        return;
    }

    Astrelis::InMemoryImage referenceImage = Astrelis::InMemoryImage(file);
    CompareImages(image, referenceImage, m_PixelThreshold);
}

INSTANTIATE_TEST_SUITE_P(Renderer2DTest,
                         Renderer2DTest,
                         ::testing::Values(Renderer2DTestParams {"EmptyScene"},
                                           Renderer2DTestParams {"SingleSprite"},
                                           Renderer2DTestParams {"MultipleSprites"},
                                           Renderer2DTestParams {"DepthTest"}));


