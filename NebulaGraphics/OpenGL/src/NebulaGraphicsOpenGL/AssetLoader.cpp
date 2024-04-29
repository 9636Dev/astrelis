#include "AssetLoader.hpp"
#include "NebulaShaderBuilder/Glsl.hpp"

namespace Nebula
{
    bool GlAssetLoader::LoadShader(const File& file)
    {
        auto res = Shader::Glsl::FromFile(file);
        if (!res.has_value())
        {
            return false;
        }

        m_Shaders[res->Name] = std::move(res.value());
        return true;
    }
} // namespace Nebula
