#pragma once

#include "Astrelis/Core/Utils/Renderer.hpp"

namespace Astrelis {
    // We only store the compiled shader for the compiled platforms, we will have an API to compile the shaders for the platforms
    struct CompiledShader {
        ASTRELIS_RUN_RENDERER_VULKAN(std::vector<std::uint8_t> Vulkan;)
    };

    class Material {
    public:
    private:
        CompiledShader m_VertexShader;
        CompiledShader m_FragmentShader;
    };
} // namespace Astrelis
