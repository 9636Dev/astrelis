#pragma once

#include "NebulaEngine/Core/Layer.hpp"
#include "NebulaEngine/Renderer/Material.hpp"
#include "NebulaEngine/Renderer/Mesh.hpp"
#include "NebulaEngine/Renderer/Transform.hpp"

class SandboxLayer : public Nebula::Layer
{
public:
    SandboxLayer();
    ~SandboxLayer() override;
    SandboxLayer(const SandboxLayer&) = delete;
    SandboxLayer& operator=(const SandboxLayer&) = delete;
    SandboxLayer(SandboxLayer&&) = delete;
    SandboxLayer& operator=(SandboxLayer&&) = delete;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnUIRender() override;
    void OnEvent(Nebula::Event& event) override;

private:
    Nebula::StaticMesh m_Mesh;
    std::vector<Nebula::Transform> m_BatchTransforms;
    std::vector<Nebula::Material> m_BatchMaterials;
    std::vector<Nebula::Transform> m_Transforms;
    std::vector<Nebula::Material> m_Materials;
};
