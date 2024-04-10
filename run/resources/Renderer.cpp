#include "Pulsarionpch.h"
#include "GLRenderer.h"

#include "Enum.h"
#include "GL.h"

#include "Pulsarion/UI/Window.h"

#include "Pulsarion/Core/GraphicalObject.h"
#include "Pulsarion/Core/Backend/MeshBackend.h"
#include "Pulsarion/Core/ShaderManager.h"
#include "Pulsarion/Core/TextureManager.h"
#include "Pulsarion/Core/Camera.h"

#include "GLMeshBackend.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Pulsarion::OpenGL
{
    GLRenderer::GLRenderer() : m_2DProjection(glm::identity<glm::mat4>()), m_RenderInfo(), m_2DRenderables(), m_Config()
    {

    }

    GLRenderer::~GLRenderer()
    {

    }

    void GLRenderer::Clear()
    {
        GL::Clear(ClearTarget::ColorAndDepthBufferBit);
    }

    void GLRenderer::SetClearColor(glm::vec4 color)
    {
        GL::ClearColor(color.r, color.g, color.b, color.a);
    }

    void GLRenderer::RenderUIWindow(const UI::Window& window)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        window.Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GLRenderer::SetBlend(bool blend)
    {
        if (blend)
        {
            GL::Enable(EnableTarget::Blend);
            GL::BlendFunc(BlendFactor::SourceAlpha, BlendFactor::OneMinusSourceAlpha);
        }
        else
        {
            GL::Disable(EnableTarget::Blend);
        }
    }

    void GLRenderer::SetWireframeMode(bool wireframe)
    {
        if (wireframe)
        {
            GL::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
        }
        else
        {
            GL::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
        }
    }

    void GLRenderer::Add2DRenderable(std::shared_ptr<GraphicalObject2D> object)
    {
        auto it = m_2DRenderables.find(object->GetMesh());
        if (it == m_2DRenderables.end())
        {
            Material2DData materialData;
            materialData.UniformBuffer = nullptr;
            materialData.Renderables.push_back(object);
            m_2DRenderables[object->GetMesh()] = Mesh2DData(object->m_Mesh);
            m_2DRenderables[object->GetMesh()].Materials[object->GetMaterial()] = materialData;
            return;
        }

        auto materialIt = it->second.Materials.find(object->GetMaterial());
        if (materialIt == it->second.Materials.end())
        {
            Material2DData materialData;
            materialData.UniformBuffer = nullptr;
            materialData.Renderables.push_back(object);
            it->second.Materials[object->GetMaterial()] = materialData;
            return;
        }

        materialIt->second.Renderables.push_back(object);
    }

    bool GLRenderer::Remove2DRenderable(std::shared_ptr<GraphicalObject2D> object)
    {
        auto it = m_2DRenderables.find(object->GetMesh());
        if (it == m_2DRenderables.end())
        {
            return false;
        }

        auto materialIt = it->second.Materials.find(object->GetMaterial());
        if (materialIt == it->second.Materials.end())
        {
            return false;
        }

        auto& renderables = materialIt->second.Renderables;
        auto renderableIt = std::find(renderables.begin(), renderables.end(), object);
        if (renderableIt == renderables.end())
        {
            return false;
        }

        renderables.erase(renderableIt);
        return true;
    }

    void GLRenderer::Render(const Camera& camera)
    {
        m_RenderInfo.Clear();
        auto startTime = std::chrono::high_resolution_clock::now();

        for (auto& [mesh, data] : m_2DRenderables)
        {
            if (data.Materials.empty())
                continue;

            if (data.OptimizedShaderSignature == Shading::ShaderSignature()) // Didn't optimize yet, or dirty
            {
                data.OptimizedShaderSignature = Transform2D::GetShaderSignature();
                // If more than a certain material threshold, optimize for material
                if (data.Materials.size() >= m_Config.MaterialOptimizationThreshold.Value)
                {
                    data.OptimizedShaderSignature = data.OptimizedShaderSignature | Material::GetShaderSignature();
                    data.IsMaterialOptimized = true;
                }

                // Transform uniforms into input for instanced rendering
                // TODO: Unoptimize if more than GL_MAX_VERTEX_ATTRIBS
                data.OptimizedShaderSignature.VertexInputs |= data.OptimizedShaderSignature.VertexUniforms;
                data.OptimizedShaderSignature.VertexUniforms = 0;
                data.OptimizedShaderSignature.FragmentInputs |= data.OptimizedShaderSignature.FragmentUniforms;
                data.OptimizedShaderSignature.FragmentUniforms = 0;
                if (!data.IsMaterialOptimized)
                {
                    data.OptimizedShaderSignature = data.OptimizedShaderSignature | Material::GetShaderSignature();
                }
                data.OptimizedShaderSignature = data.OptimizedShaderSignature | mesh->GetShaderSignature() | GraphicalObject2D::GetShaderSignature();

                // Generate optimized shader
                data.Shader = ShaderManager::GetShader(data.OptimizedShaderSignature);
                if (data.Shader == nullptr)
                {
                    PLS_LOG_WARN("Shader failed to generate for signature!");
                    continue;
                }

                // Create the instanced vertex buffer
                GLMeshBackend2D* meshBackend = dynamic_cast<GLMeshBackend2D*>(GetMeshBackend2D(data.Mesh));
                if (meshBackend == nullptr)
                {
                    PLS_LOG_WARN("Mesh backend is not a GLMeshBackend2D!");
                    continue;
                }

                if (meshBackend->m_InstanceBuffer == nullptr)
                {
                    meshBackend->m_InstanceBuffer = std::make_unique<VertexBuffer>();
                }

                // Create the layout and input the data
                // TODO: Use the data to determine if the data has to be updated, and only update the subdata of that object if it is lower than a certain threshold
                VertexBufferLayout layout;
                // Follow the input order
                // TODO: Refactor this into a function
                std::uint32_t index = 0;
                for (const auto& input : data.Shader->GetInputOrder().VertexInputs)
                {
                    switch (input)
                    {
                    case Shading::ShaderInputUniform::Texture2D:
                        layout.SetAttribute<std::uint32_t>(index, 1, false, true);
                        index++;
                        break;
                    case Shading::ShaderInputUniform::ModelMatrix:
                        layout.SetAttribute<float>(index, 4, false, true);
                        layout.SetAttribute<float>(index + 1, 4, false, true);
                        layout.SetAttribute<float>(index + 2, 4, false, true);
                        layout.SetAttribute<float>(index + 3, 4, false, true);
                        index += 4;
                        break;
                    case Shading::ShaderInputUniform::DiffuseColor:
                        layout.SetAttribute<float>(index, 4, false, true);
                        index++;
                        break;
                    case Shading::ShaderInputUniform::Position2D: // Make dynamic
                    case Shading::ShaderInputUniform::TexCoord2D:
                        index++;
                        break;
                    default:
                        PLS_LOG_WARN("Unknown vertex input!");
                        break;
                    }
                }

                // TODO: Exclude mesh specific inputs, such as vertex position, texture coordinates, since they are don't change per instance

                // Input the data for each, as interleaved
                std::vector<float> instanceData;
                const auto bytesPerInstance = layout.GetStride();
                for (const auto& input : data.Shader->GetInputOrder().VertexInputs)
                {
                    for (const auto& material : data.Materials)
                    {
                        for (const auto& renderable : material.second.Renderables)
                        {
                            switch (input)
                            {
                            case Shading::ShaderInputUniform::Texture2D:
                                instanceData.push_back(static_cast<float>(data.TextureUnitMap[renderable->GetMaterial()->GetTextureId()]));
                                break;
                            case Shading::ShaderInputUniform::ModelMatrix: {
                                auto ptr = glm::value_ptr(renderable->GetTransform().GetAsMatrix());
                                instanceData.insert(instanceData.end(), ptr, ptr + 16);
                                break;
                            }
                            case Shading::ShaderInputUniform::Position2D: // Make dynamic
                            case Shading::ShaderInputUniform::TexCoord2D:
                                break;
                            default:
                                PLS_LOG_WARN("Unknown vertex input!");
                                break;
                            }
                        }
                    }
                }

                meshBackend->m_InstanceBuffer->SetData(instanceData.data(), static_cast<std::uint32_t>(instanceData.size() * sizeof(float)));
                meshBackend->m_VertexArray.AddBuffer(*meshBackend->m_InstanceBuffer, layout);
            }

            if (data.Shader == nullptr)
            {
                PLS_LOG_WARN("Shader is null for renderable, but the shader signature is not empty!");
                continue;
            }

            data.Shader->Bind();
            const auto& shaderSignature = data.Shader->GetSignature();

            if (shaderSignature.VertexUniformContains(Shading::ShaderInputUniform::ProjectionMatrix))
                data.Shader->SetUniform("u_ProjectionMatrix", m_2DProjection);
            if (shaderSignature.VertexUniformContains(Shading::ShaderInputUniform::ViewMatrix))
                data.Shader->SetUniform("u_ViewMatrix", camera.Get2DViewMatrix());
            if (shaderSignature.VertexUniformContains(Shading::ShaderInputUniform::ModelMatrix))
                data.Shader->SetUniform("u_ModelMatrix", glm::identity<glm::mat4>());

            mesh->GetBackend().Bind();
            std::size_t rendered = 0;
            for (auto& [material, materialData] : data.Materials)
            {
                if (materialData.Renderables.empty())
                    continue;

                // TODO: Use uniform buffer for view and projection matrices (and material if not optimized)

                if (data.TextureUnitMap.find(material->GetTextureId()) == data.TextureUnitMap.end())
                {
                    // TODO: Use texture unit manager, which will also handle the case where there are more textures than texture units
                    data.TextureUnitMap[material->GetTextureId()] = static_cast<std::uint32_t>(data.TextureUnitMap.size());
                }

                if (data.IsMaterialOptimized)
                {
                    // TODO: Implement
                }
                else
                {
                    if (shaderSignature.FragmentUniformContains(Shading::ShaderInputUniform::DiffuseColor))
                        data.Shader->SetUniform("u_DiffuseColor", material->GetDiffuseColor());
                    if (shaderSignature.FragmentUniformContains(Shading::ShaderInputUniform::Texture2D))
                        data.Shader->SetUniform("u_Texture2D", data.TextureUnitMap[material->GetTextureId()]);
                }
                TextureManager::Bind2DTexture(material->GetTextureId(), data.TextureUnitMap[material->GetTextureId()]);

                GL::DrawElementsInstancedBaseIntance(DrawMode::Triangles, mesh->GetIndices().size(), Type::UnsignedInt, nullptr, static_cast<std::uint32_t>(materialData.Renderables.size()), rendered);
                rendered += materialData.Renderables.size();

                m_RenderInfo.Instances += materialData.Renderables.size();
                m_RenderInfo.DrawCalls++;
            }

            m_RenderInfo.Vertices += mesh->GetVertexData().GetVertexCount();
            m_RenderInfo.Indices += mesh->GetIndices().size();
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        m_RenderInfo.TimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    }

    const RenderInfo& GLRenderer::GetRenderInfo() const
    {
        return m_RenderInfo;
    }

    void GLRenderer::Set2DProjection(const glm::mat4 &projection)
    {
        m_2DProjection = projection;
    }

    const glm::mat4 &GLRenderer::Get2DProjection() const
    {
        return m_2DProjection;
    }
}

namespace Pulsarion
{
    std::unique_ptr<Renderer> CreateRenderer()
    {
        return std::make_unique<OpenGL::GLRenderer>();
    }
}