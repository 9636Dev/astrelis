#include "Texture.hpp"

namespace Nebula::OpenGL
{
    Texture::Texture() : m_Texture(0) { GL::GenTextures(1, &m_Texture); }

    Texture::~Texture() { GL::DeleteTextures(1, &m_Texture); }

    void Texture::SetData(const InMemoryImage& image)
    {
        Bind(TextureUnit::Texture0);

        GL::TexImage2D(TextureTarget::Texture2D, 0, InternalFormat::RGBA, static_cast<std::int32_t>(image.GetWidth()),
                       static_cast<std::int32_t>(image.GetHeight()), 0, PixelFormat::RGBA, PixelType::UnsignedByte,
                       image.GetBytes().data());

        SetFiltering(TextureFilter::Linear, TextureFilter::Linear);
        SetWrapping(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge);
        Unbind();
    }

    void Texture::Bind(TextureUnit unit) const
    {
        GL::ActiveTexture(unit);
        GL::BindTexture(TextureTarget::Texture2D, m_Texture);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void Texture::Unbind() const { GL::BindTexture(TextureTarget::Texture2D, 0); }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void Texture::SetFiltering(TextureFilter minFilter, TextureFilter magFilter)
    {
        Bind(TextureUnit::Texture0);
        GL::TexParameteri(TextureTarget::Texture2D, TextureParameter::TextureMinFilter,
                          static_cast<std::int32_t>(minFilter));
        GL::TexParameteri(TextureTarget::Texture2D, TextureParameter::TextureMagFilter,
                          static_cast<std::int32_t>(magFilter));
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void Texture::SetWrapping(TextureWrap wrapS, TextureWrap wrapT)
    {
        Bind(TextureUnit::Texture0);
        GL::TexParameteri(TextureTarget::Texture2D, TextureParameter::TextureWrapS, static_cast<std::int32_t>(wrapS));
        GL::TexParameteri(TextureTarget::Texture2D, TextureParameter::TextureWrapT, static_cast<std::int32_t>(wrapT));
    }
} // namespace Nebula::OpenGL
