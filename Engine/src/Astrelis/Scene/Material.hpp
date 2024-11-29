#pragma once

#include "Astrelis/Core/Types.hpp"
#include "Astrelis/Core/Utils/Renderer.hpp"

#include <vector>

namespace Astrelis {
    // We only store the compiled shader for the compiled platforms, we will have an API to compile the shaders for the platforms
    struct CompiledShader {
        struct VulkanShader {
            std::vector<char> Data;
            CString           Entrypoint;

            VulkanShader(std::vector<char> data, CString entrypoint)
                : Data(std::move(data)), Entrypoint(entrypoint) {
            }
        };
        ASTRELIS_RUN_RENDERER_VULKAN(VulkanShader Vulkan);

        explicit CompiledShader(
#if ASTRELIS_RENDERER_VULKAN
            VulkanShader vulkan
#endif
            )
            : ASTRELIS_RUN_RENDERER_VULKAN(Vulkan(std::move(vulkan))) {
        }
    };

    class Material {
    public:
    private:
        CompiledShader m_VertexShader;
        CompiledShader m_FragmentShader;
    };
} // namespace Astrelis
