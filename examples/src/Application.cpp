#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Geometry/Transform.hpp"
#include "NebulaGraphicsCore/Mesh/2DStaticMesh.hpp"
#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsRenderer/Renderer.hpp"
#include "NebulaGraphicsRenderer/Window.hpp"

#include <fstream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    Nebula::Log::Init();
    NEB_CORE_LOG_INFO("Hello from NebulaCore!");

#define EXAMPLES_USE_OPENGL
#ifdef EXAMPLES_USE_OPENGL
    Nebula::WindowProps<Nebula::OpenGLContext> props("Nebula", 1'080, 720, {4, 1});
    const std::string libraryPath = "lib/NebulaGraphicsOpenGL";
    auto result                   = Nebula::CreateWindow(libraryPath, props);
    if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }
#elif defined(EXAMPLES_USE_METAL)
    Nebula::WindowProps<Nebula::MetalContext> props("Nebula", 1'080, 720, {});
    const std::string libraryPath = "lib/NebulaGraphicsMetal";
    auto result                   = Nebula::CreateWindow(libraryPath, props);
    if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }

#else
    #error "No graphics API selected"
#endif

    // !IMPORTANT! There must only be one shared pointer to the window when it is destroyed
    auto window         = std::move(result.Window);
    auto rendererResult = Nebula::CreateRenderer(libraryPath, window, props.Context);
    if (rendererResult.Error != Nebula::RendererCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create renderer");
        return -1;
    }
    auto renderer = std::move(rendererResult.Renderer);
    renderer->SetClearColor(0.1F, 0.1F, 0.1F, 1.0F);

    std::string shaderSrc;
    {
        std::ifstream file("resources/shaders/BasicShader.hlsl");
        if (!file.good())
        {
            NEB_CORE_LOG_ERROR("Failed to open shader file");
            return -1;
        }

        // Just read the whole buffer into a sstream
        std::stringstream buffer;
        buffer << file.rdbuf();
        shaderSrc = buffer.str();
    }

    Nebula::ShaderProgram shaderProgram = {
        "BasicShader",
        Nebula::ShaderConductor::ShaderInput(
            "BasicShader.hlsl", shaderSrc, "VertexShader",
            Nebula::ShaderConductor::TargetProfile(Nebula::ShaderConductor::ShaderStage::Vertex, 6, 0)),
        Nebula::ShaderConductor::ShaderInput(
            "BasicShader.hlsl", shaderSrc, "PixelShader",
            Nebula::ShaderConductor::TargetProfile(Nebula::ShaderConductor::ShaderStage::Pixel, 6, 0))};
    Nebula::RenderPass renderPass = {"2DRenderPass", shaderProgram};
    renderer->AddRenderPass(renderPass);

    auto mesh = std::make_shared<Nebula::Static2DMesh>();
    mesh->Vertices() = {
        -0.5F, -0.5F, 0.5F, -0.5F, 0.5F, 0.5F, -0.5F, 0.5F,
    };
    mesh->Indices() = {0, 1, 2, 0, 2, 3};
    Nebula::RenderableObject renderableObject(Nebula::Transform(), mesh);
    Nebula::RenderableObject renderableObject2(Nebula::Transform(), mesh);

    renderer->AddRenderableObject(renderableObject, 0);
    renderer->AddRenderableObject(renderableObject2, 0);

    window->SetVSync(false);
    //window->SetEventCallback([](Nebula::Event& event) { NEB_CORE_LOG_TRACE("{0}", event.ToString()); });

    while (!window->ShouldClose())
    {
        renderer->Render();
        auto& transform = renderer->GetRenderableObject(0).GetTransform();
        transform.Rotate(0.01F, Nebula::Vector3f(0.0F, 0.0F, 1.0F));
        auto& transform2 = renderer->GetRenderableObject(1).GetTransform();
        transform2.Rotate(-0.01F, Nebula::Vector3f(0.0F, 0.0F, 1.0F));

        window->PollEvents();
        window->SwapBuffers();
    }

    NEB_CORE_LOG_INFO("Shutting down NebulaCore");

    // Renderer first, since it might depend on the window
    NEB_CORE_LOG_TRACE("Destroying renderer");
    Nebula::DestroyRenderer(renderer);
    NEB_CORE_LOG_TRACE("Destroying window");
    Nebula::DestroyWindow(window);

    return 0;
}
