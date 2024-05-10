#include "Renderer.hpp"

#include "Core.hpp"
#include "Mesh.hpp"
#include "NebulaGraphicsCore/Renderer.hpp"

namespace Nebula
{
    MetalRenderer::MetalRenderer(std::shared_ptr<MetalWindow> window) :
        m_Window(std::move(window)),
        m_AssetLoader(m_Window->m_MetalDevice)
    {
        m_CommandQueue = [m_Window->m_MetalDevice newCommandQueue];
    }

    MetalRenderer::~MetalRenderer() = default;

    void MetalRenderer::Render()
    {
        auto commandBuffer           = [m_CommandQueue commandBuffer];
        commandBuffer.label          = @"Main Command Buffer";
        id<CAMetalDrawable> drawable = [m_Window->m_CAMetalLayer nextDrawable];
        // We create a buffer for the uniform data
        id<MTLBuffer> uniformBuffer = [m_Window->m_MetalDevice newBufferWithLength:sizeof(float) * 16
                                                                           options:MTLResourceStorageModeShared];

        for (std::size_t i = 0; i < m_RenderPasses.size(); i++)
        {
            auto& renderPass                                     = m_MetalRenderPasses[i];
            auto renderPassDescriptor                            = [MTLRenderPassDescriptor renderPassDescriptor];
            renderPassDescriptor.colorAttachments[0].texture     = drawable.texture;
            renderPassDescriptor.colorAttachments[0].loadAction  = MTLLoadActionClear;
            renderPassDescriptor.colorAttachments[0].clearColor  = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
            renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

            auto renderEncoder  = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
            renderEncoder.label = @"Main Render Encoder";

            [renderEncoder setRenderPipelineState:renderPass];

            for (std::size_t j = 0; j < m_RenderPassObjectCount[i]; j++)
            {
                Matrix4f modelMatrix = m_RenderableObjects[j].m_Transform.GetModelMatrix();
                // Copy data to the buffer
                memcpy([uniformBuffer contents], modelMatrix.data(), sizeof(float) * 16);
                // Bind the buffer to the vertex shader

                auto& renderableObject = m_MetalMeshes[j];
                [renderEncoder setVertexBuffer:renderableObject.VertexBuffer offset:0 atIndex:0];
                [renderEncoder setVertexBuffer:uniformBuffer offset:0 atIndex:1];
                [renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                          indexCount:renderableObject.IndexBuffer.length / sizeof(std::uint32_t)
                                           indexType:MTLIndexTypeUInt32
                                         indexBuffer:renderableObject.IndexBuffer
                                   indexBufferOffset:0];
            }

            [renderEncoder endEncoding];
            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];
        }
    }

    void MetalRenderer::InternalAddRenderPass(RenderPass renderPass, std::size_t insertionIndex)
    {
        NSError* error = nil;

        MTLVertexDescriptor* vertexDescriptor      = [[MTLVertexDescriptor alloc] init];
        vertexDescriptor.attributes[0].format      = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].offset      = 0;
        vertexDescriptor.attributes[0].bufferIndex = 0;

        vertexDescriptor.layouts[0].stride       = 3 * sizeof(float);
        vertexDescriptor.layouts[0].stepRate     = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;

        auto shader                                        = m_AssetLoader.m_Shaders[renderPass.ShaderProgram];
        MTLRenderPipelineDescriptor* pipelineDescriptor    = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineDescriptor.vertexFunction                  = shader.Vertex;
        pipelineDescriptor.fragmentFunction                = shader.Pixel;
        pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

        pipelineDescriptor.vertexDescriptor = vertexDescriptor;

        id<MTLRenderPipelineState> pipelineState =
            [m_Window->m_MetalDevice newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];

        if (error != nil)
        {
            NSLog(@"Error creating render pipeline state: %@", error.localizedDescription);
            return;
        }

        m_MetalRenderPasses.insert(m_MetalRenderPasses.begin() + static_cast<std::int64_t>(insertionIndex),
                                   pipelineState);
    }

    void MetalRenderer::InternalRemoveRenderPass(std::size_t index) {}

    void MetalRenderer::InternalAddRenderableObject(RenderableObject renderableObject, std::size_t renderableIndex)
    {
        auto vertexBuffer =
            [m_Window->m_MetalDevice newBufferWithBytes:renderableObject.m_Mesh->GetVertexData().data()
                                                 length:renderableObject.m_Mesh->GetVertexData().size() * sizeof(float)
                                                options:MTLResourceStorageModeShared];

        auto indexBuffer = [m_Window->m_MetalDevice
            newBufferWithBytes:renderableObject.m_Mesh->GetIndexData().data()
                        length:renderableObject.m_Mesh->GetIndexData().size() * sizeof(std::uint32_t)
                       options:MTLResourceStorageModeShared];

        m_MetalMeshes.insert(m_MetalMeshes.begin() + static_cast<std::int64_t>(renderableIndex),
                             {vertexBuffer, indexBuffer});
    }

    void MetalRenderer::InternalRemoveRenderableObject(std::size_t index) {}

    void MetalRenderer::OnResize(std::uint32_t width, std::uint32_t height) {}

    void MetalRenderer::SetClearColor(float red, float green, float blue, float alpha) {}
} // namespace Nebula

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"

extern "C" NEBULA_GRAPHICS_METAL_API Nebula::RendererCreationResult
    nebulaGraphicsMetalCreateMetalRenderer([[maybe_unused]] const Nebula::MetalContext& context,
                                           const std::shared_ptr<Nebula::Window>& window)
{
    std::shared_ptr<Nebula::MetalWindow> windowPtr = std::dynamic_pointer_cast<Nebula::MetalWindow>(window);
    if (windowPtr == nullptr)
    {
        return {nullptr, Nebula::RendererCreationResult::ErrorType::InvalidArguments};
    }

    auto renderer = std::make_shared<Nebula::MetalRenderer>(windowPtr);
    return {renderer, Nebula::RendererCreationResult::ErrorType::None};
}

#pragma clang diagnostic pop
