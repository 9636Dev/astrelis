#pragma once

#include "NebulaEngine/IO/Image.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

namespace Nebula
{
    class TextureImage
    {
    public:
        TextureImage() = default;
        virtual ~TextureImage() = default;
        TextureImage(const TextureImage& other) = delete;
        TextureImage& operator=(const TextureImage& other) = delete;
        TextureImage(TextureImage&& other) = delete;
        TextureImage& operator=(TextureImage&& other) = delete;

        virtual bool LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context) = 0;
    };
}  // namespace Nebula
