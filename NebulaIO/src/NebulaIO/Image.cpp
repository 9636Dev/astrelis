#include "Image.hpp"

#include <cstring>
#include <stb_image.h>

namespace Nebula
{
    std::optional<InMemoryImage> InMemoryImage::LoadFromFile(const File& file)
    {
        auto [success, bytes] = file.ReadBytes();
        if (!success)
        {
            return std::nullopt;
        }

        if (bytes.empty())
        {
            return std::nullopt;
        }

        return Load(bytes);
    }

    std::optional<InMemoryImage> InMemoryImage::Load(const std::vector<std::byte>& bytes)
    {
        int width    = 0;
        int height   = 0;
        int channels = 0;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load_from_memory(
            /* NOLINT(cppcoreguidelines-pro-type-reinterpret-cast) */ reinterpret_cast<const stbi_uc*>(bytes.data()),
            static_cast<int>(bytes.size()), &width, &height, &channels, 0);

        if (data == nullptr)
        {
            return std::nullopt;
        }

        // We have to copy the data because stbi_load_from_memory allocates memory on the heap
        std::vector<std::byte> dataCopy(static_cast<std::size_t>(width) * height * channels);
        std::memcpy(dataCopy.data(), data, dataCopy.size());
        stbi_image_free(data);

        return InMemoryImage(dataCopy, static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height),
                             static_cast<std::uint32_t>(channels), 8);
    }
} // namespace Nebula
