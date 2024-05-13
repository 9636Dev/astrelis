#include "NebulaGraphicsCore/Basic/Renderer.hpp"
#include "NebulaGraphicsCore/Basic/VisualProps.hpp"
#include "NebulaGraphicsRenderer/BasicRenderer.hpp"
#define NEBULA_USE_BASIC_RENDERER
#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Event/Event.hpp"
#include "NebulaGraphicsCore/Event/MouseEvent.hpp"
#include "NebulaGraphicsCore/Geometry/Transform.hpp"
#include "NebulaGraphicsCore/Mesh/2DStaticMesh.hpp"
#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsRenderer/Renderer.hpp"
#include "NebulaGraphicsRenderer/Window.hpp"
#include "NebulaIO/Image.hpp"


enum class GraphicsAPI
{
    OpenGL,
    Metal,
};

int main(int argc, char** argv)
{
    Nebula::Log::Init();

    GraphicsAPI api = GraphicsAPI::OpenGL;
    if (argc < 2)
    {
        NEB_LOG_INFO("No graphics API specified, defaulting to OpenGL");
    }
    else
    {
        std::string apiString = argv[1]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (apiString == "OpenGL")
        {
            api = GraphicsAPI::OpenGL;
        }
        else if (apiString == "Metal")
        {
            api = GraphicsAPI::Metal;
        }
        else
        {
            NEB_CORE_LOG_ERROR("Invalid graphics API specified");
            return -1;
        }
    }

    std::shared_ptr<Nebula::Window> window;
#ifdef NEBULA_USE_BASIC_RENDERER
    std::shared_ptr<Nebula::Basic::Renderer> renderer;
#else
    std::shared_ptr<Nebula::Renderer> renderer;
#endif

    if (api == GraphicsAPI::OpenGL)
    {
        NEB_CORE_LOG_INFO("Using OpenGL");
        Nebula::WindowProps<Nebula::OpenGLContext> props("Nebula", 1'080, 720, {4, 1});
        const std::string libraryPath = "lib/NebulaGraphicsOpenGL";
        {
            auto result = Nebula::CreateWindow(libraryPath, props);
            if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
            {
                NEB_CORE_LOG_ERROR("Failed to create window");
                return -1;
            }

            // !IMPORTANT! There must only be one shared pointer to the window when it is destroyed
            window = std::move(result.Window);
        }

        {
            #ifdef NEBULA_USE_BASIC_RENDERER
            auto result = Nebula::CreateBasicRenderer(libraryPath, window, props.Context);
            #else
            auto result = Nebula::CreateRenderer(libraryPath, window, props.Context);
            #endif
            if (result.Error != Nebula::RendererCreationResult::ErrorType::None)
            {
                NEB_CORE_LOG_ERROR("Failed to create renderer");
                return -1;
            }

            renderer = std::move(result.Renderer);
            renderer->SetClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        }
    }
    else if (api == GraphicsAPI::Metal)
    {
        NEB_CORE_LOG_INFO("Using Metal");
        Nebula::WindowProps<Nebula::MetalContext> props("Nebula", 1'080, 720, {});
        const std::string libraryPath = "lib/NebulaGraphicsMetal";
        {
            auto result = Nebula::CreateWindow(libraryPath, props);
            if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
            {
                NEB_CORE_LOG_ERROR("Failed to create window");
                return -1;
            }

            // !IMPORTANT! There must only be one shared pointer to the window when it is destroyed
            window = std::move(result.Window);
        }

        {
            #ifdef NEBULA_USE_BASIC_RENDERER
            auto result = Nebula::CreateBasicRenderer(libraryPath, window, props.Context);
            #else
            auto result = Nebula::CreateRenderer(libraryPath, window, props.Context);
            #endif
            if (result.Error != Nebula::RendererCreationResult::ErrorType::None)
            {
                NEB_CORE_LOG_ERROR("Failed to create renderer");
                return -1;
            }

            renderer = std::move(result.Renderer);
            renderer->SetClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        }
    }

    #ifndef NEBULA_USE_BASIC_RENDERER
    if (!renderer->GetAssetLoader().LoadDefaultAssets())
    {
        NEB_CORE_LOG_ERROR("Failed to load default assets");
        return -1;
    }


    /*{
        auto file = Nebula::File::FromPathString("resources/shaders/BasicShader.cnsl");
        bool res  = renderer->GetAssetLoader().LoadShader(file);

        if (!res)
        {
            NEB_CORE_LOG_ERROR("Failed to load shader");
            return -1;
        }
    }*/

    {
        auto file = Nebula::File::FromPathString("resources/assets/textures/DefaultTexture.png");
        auto imageOpt = Nebula::InMemoryImage::LoadFromFile(file);

        if (!imageOpt)
        {
            NEB_CORE_LOG_ERROR("Failed to load image");
            return -1;
        }

        Nebula::Texture texture = {
            "DefaultTexture",
            std::move(imageOpt.value())
        };
        auto res = renderer->GetAssetLoader().LoadTexture(texture);

        if (!res)
        {
            NEB_CORE_LOG_ERROR("Failed to load texture");
            return -1;
        }
    }

    Nebula::RenderPass renderPass = {"2DRenderPass", "BasicShader"};
    renderer->AddRenderPass(renderPass);

    auto mesh        = std::make_shared<Nebula::Static2DMesh>();
    mesh->Vertices() = {
        -0.5F, -0.5F, 0.5F, -0.5F, 0.5F, 0.5F, -0.5F, 0.5F,
    };
    mesh->Indices() = {0, 1, 2, 0, 2, 3};
    Nebula::RenderableObject renderableObject(mesh);

    renderer->AddRenderableObject(renderableObject, 0);

    window->SetVSync(false);

    float amount       = 0.0F;
    auto onMouseScroll = [&amount](Nebula::MouseScrollEvent& event) {
        amount += event.GetOffsetY() * 0.005F;
        return true;
    };

    window->SetEventCallback([&onMouseScroll](Nebula::Event& event) {
        Nebula::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Nebula::MouseScrollEvent>(onMouseScroll);
        //NEB_CORE_LOG_TRACE("{0}", event.ToString());
    });

    while (!window->ShouldClose())
    {
        renderer->Render();
        auto& transform = renderer->GetRenderableObject(0).GetTransform();
        transform.Rotate(amount, Nebula::Vector3f(0.0F, 0.0F, 1.0F));

        window->PollEvents();
        window->SwapBuffers();
    }

    NEB_CORE_LOG_INFO("Shutting down NebulaCore");

    // Renderer first, since it might depend on the window
    NEB_CORE_LOG_TRACE("Destroying renderer");
    Nebula::DestroyRenderer(renderer);
    NEB_CORE_LOG_TRACE("Destroying window");
    Nebula::DestroyWindow(window);
    #else

    auto mesh = std::make_shared<Nebula::Basic::Mesh>();
    mesh->GetVertices() = {
        {{-0.5F, -0.5F, 0.0F}, {0.0F, 0.0F}},
        {{0.5F, -0.5F, 0.0F}, {1.0F, 0.0F}},
        {{0.5F, 0.5F, 0.0F}, {1.0F, 1.0F}},
        {{-0.5F, 0.5F, 0.0F}, {0.0F, 1.0F}},
    };
    mesh->GetIndices() = {0, 1, 2, 2, 3, 0};

    Nebula::Basic::VisualProps props;
    props.Color = {1.0F, 0.0F, 0.0F, 1.0F};
    Nebula::Transform transform;

    renderer->AddMesh(mesh, props, transform);

    while (!window->ShouldClose())
    {
        renderer->Render();
        window->PollEvents();
        window->SwapBuffers();
    }

    NEB_CORE_LOG_TRACE("Destroying renderer");
    Nebula::DestroyBasicRenderer(renderer);
    NEB_CORE_LOG_TRACE("Destroying window");
    Nebula::DestroyWindow(window);

    #endif

    return 0;
}
