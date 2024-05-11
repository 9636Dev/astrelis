#pragma once

#include "Enum.hpp"
#include "GL.hpp"
#include "NebulaIO/Image.hpp"

namespace Nebula::OpenGL
{
    class Texture
    {
    public:
        Texture();
        Texture(const Texture&) = delete;
        Texture(Texture&& other) noexcept : m_Texture(other.m_Texture)
        {
            other.m_Texture = 0;
        }
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&& other) noexcept
        {
            if (this != &other)
            {
                m_Texture       = other.m_Texture;
                other.m_Texture = 0;
            }
            return *this;
        }
        ~Texture();

        void SetData(const InMemoryImage& image);
        void Bind(TextureUnit unit) const;
        void Unbind() const;

        void SetFiltering(TextureFilter minFilter, TextureFilter magFilter);
        void SetWrapping(TextureWrap wrapS, TextureWrap wrapT);

    private:
        Texture_t m_Texture;
    };
} // namespace Nebula::OpenGL
