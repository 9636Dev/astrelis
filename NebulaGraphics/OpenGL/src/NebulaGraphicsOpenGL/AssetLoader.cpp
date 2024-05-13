#include "AssetLoader.hpp"
#include "NebulaShaderBuilder/Glsl.hpp"

#include "DefaultShaders.hpp"

namespace Nebula
{
    bool GlAssetLoader::LoadDefaultAssets()
    {
        std::string defaultVertexShader = OpenGLDefaults::DefaultVertexShader();
        std::string defaultFragmentShader = OpenGLDefaults::DefaultFragmentShader();

        Shader::Glsl defaultShader;
        defaultShader.GlslVersion = 410;
        defaultShader.VertexSource = defaultVertexShader;
        defaultShader.PixelSource = defaultFragmentShader;
        defaultShader.Name = "DefaultShader";

        m_Shaders[defaultShader.Name] = std::move(defaultShader);

        return true;
    }

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

    bool GlAssetLoader::LoadTexture(const Texture& texture)
    {
        OpenGL::Texture tex;
        tex.SetData(texture.Image);
        m_Textures[texture.Name] = std::move(tex);
        return true;
    }
} // namespace Nebula
