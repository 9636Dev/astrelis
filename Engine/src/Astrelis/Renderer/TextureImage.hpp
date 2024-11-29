#pragma once

#include "Astrelis/IO/Image.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

namespace Astrelis {
    class TextureImage {
    public:
        TextureImage()                                     = default;
        virtual ~TextureImage()                            = default;
        TextureImage(const TextureImage& other)            = default;
        TextureImage& operator=(const TextureImage& other) = default;
        TextureImage(TextureImage&& other)                 = default;
        TextureImage& operator=(TextureImage&& other)      = default;

        virtual bool LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context)                           = 0;
    };
} // namespace Astrelis
