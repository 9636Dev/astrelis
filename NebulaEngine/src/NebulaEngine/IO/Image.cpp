#include "Image.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include <stb_image.h>
#include <stb_image_write.h>

namespace Nebula
{
    InMemoryImage::InMemoryImage(const File& file)
    {
        stbi_uc* data = stbi_load(file.GetPath().string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
        if (data != nullptr)
        {
            m_Data.resize(static_cast<std::size_t>(m_Width) * m_Height * 4);
            if (m_Channels == 3)
            {
                for (int y = 0; y < m_Height; y++)
                {
                    for (int x = 0; x < m_Width; x++)
                    {
                        m_Data[4 * (y * m_Width + x) + 0] = static_cast<std::byte>(data[3 * (y * m_Width + x) + 0]);
                        m_Data[4 * (y * m_Width + x) + 1] = static_cast<std::byte>(data[3 * (y * m_Width + x) + 1]);
                        m_Data[4 * (y * m_Width + x) + 2] = static_cast<std::byte>(data[3 * (y * m_Width + x) + 2]);
                        m_Data[4 * (y * m_Width + x) + 3] = static_cast<std::byte>(255);
                    }
                    m_Channels = 4;
                }
            }
            else
            {
                std::memcpy(m_Data.data(), data, m_Data.size());
            }
            stbi_image_free(data);
        }
        else
        {
            NEBULA_LOG_ERROR("Failed to load image: {0}", file.GetPath().string());
        }
    }

    bool InMemoryImage::Save(const std::string& filename) const
    {
        return stbi_write_png(filename.c_str(), m_Width, m_Height, m_Channels, m_Data.data(), m_Width * m_Channels) !=
               0;
    }
} // namespace Nebula
