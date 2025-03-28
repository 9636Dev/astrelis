#include "Image.hpp"

#include "Astrelis/Core/Base.hpp"

#include <stb_image.h>
#include <stb_image_write.h>

namespace Astrelis {
    InMemoryImage::InMemoryImage(const File& file) : m_Width(0), m_Height(0), m_Channels(0) {
        stbi_uc* data =
            stbi_load(file.GetPath().string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
        if (data != nullptr) {
            m_Data.resize(static_cast<std::size_t>(m_Width) * m_Height * 4);
            if (m_Channels == 3) {
                for (int row = 0; row < m_Height; row++) {
                    for (int col = 0; col < m_Width; col++) {
                        m_Data[4 * (row * m_Width + col) + 0] =
                            static_cast<std::byte>(data[3 * (row * m_Width + col) + 0]);
                        m_Data[4 * (row * m_Width + col) + 1] =
                            static_cast<std::byte>(data[3 * (row * m_Width + col) + 1]);
                        m_Data[4 * (row * m_Width + col) + 2] =
                            static_cast<std::byte>(data[3 * (row * m_Width + col) + 2]);
                        m_Data[4 * (row * m_Width + col) + 3] = static_cast<std::byte>(255);
                    }
                    m_Channels = 4;
                }
            }
            else {
                std::memcpy(m_Data.data(), data, m_Data.size());
            }
            stbi_image_free(data);
        }
        else {
            ASTRELIS_LOG_ERROR("Failed to load image: {0}", file.GetPath().string());
        }
    }

    bool InMemoryImage::Save(const std::string& filename) const {
        return stbi_write_png(filename.c_str(), m_Width, m_Height, m_Channels, m_Data.data(),
                   m_Width * m_Channels)
            != 0;
    }
} // namespace Astrelis
