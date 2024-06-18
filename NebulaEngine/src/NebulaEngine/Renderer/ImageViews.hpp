#pragma once

namespace Nebula
{
    class ImageViews
    {
    public:
        ImageViews() = default;
        virtual ~ImageViews() = default;
        ImageViews(const ImageViews&) = delete;
        ImageViews& operator=(const ImageViews&) = delete;
        ImageViews(ImageViews&&) = delete;
        ImageViews& operator=(ImageViews&&) = delete;
    };
} // namespace Nebula
