#include "Image.hpp"

#include <stb_image.h>
#include <stb_image_write.h>

#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Math.hpp"

namespace Astrelis
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
            ASTRELIS_LOG_ERROR("Failed to load image: {0}", file.GetPath().string());
        }
    }

    bool InMemoryImage::Save(const std::string& filename) const
    {
        return stbi_write_png(filename.c_str(), m_Width, m_Height, m_Channels, m_Data.data(), m_Width * m_Channels) !=
               0;
    }

    double Variance(const std::vector<double>& values, double mean)
    {
        double variance = 0.0;
        for (double value : values)
        {
            variance += (value - mean) * (value - mean);
        }
        return variance / values.size();
    }

    double
        Covariance(const std::vector<double>& values1, const std::vector<double>& values2, double mean1, double mean2)
    {
        double covariance = 0.0;
        for (size_t i = 0; i < values1.size(); ++i)
        {
            covariance += (values1[i] - mean1) * (values2[i] - mean2);
        }
        return covariance / values1.size();
    }

    float InMemoryImage::Similarity(const InMemoryImage& other, CompareMethod method) const
    {
        (void)method;
        if (m_Width != other.m_Width || m_Height != other.m_Height || m_Channels != other.m_Channels)
        {
            return 0.0F;
        }

        double c1 = 0.01 * 255.0 * 0.01 * 255.0;
        double c2 = 0.03 * 255.0 * 0.03 * 255.0;

        std::vector<double> lum1;
        std::vector<double> lum2;
        std::vector<double> contrast1;
        std::vector<double> contrast2;

        for (std::int32_t i = 0; i < m_Width * m_Height; ++i)
        {
            double r1 = static_cast<double>(m_Data[4 * i + 0]);
            double g1 = static_cast<double>(m_Data[4 * i + 1]);
            double b1 = static_cast<double>(m_Data[4 * i + 2]);
            double r2 = static_cast<double>(other.m_Data[4 * i + 0]);
            double g2 = static_cast<double>(other.m_Data[4 * i + 1]);
            double b2 = static_cast<double>(other.m_Data[4 * i + 2]);

            lum1.push_back(0.299 * r1 + 0.587 * g1 + 0.114 * b1);
            lum2.push_back(0.299 * r2 + 0.587 * g2 + 0.114 * b2);
            contrast1.push_back(r1 * r1 + g1 * g1 + b1 * b1);
            contrast2.push_back(r2 * r2 + g2 * g2 + b2 * b2);
        }

        double mean1      = Math::Mean(lum1);
        double mean2      = Math::Mean(lum2);
        double variance1  = Variance(lum1, mean1);
        double variance2  = Variance(lum2, mean2);
        double covariance = Covariance(lum1, lum2, mean1, mean2);

        double luminance = (2.0 * mean1 * mean2 + c1) / (mean1 * mean1 + mean2 * mean2 + c1);
        double contrast  = (2.0 * std::sqrt(variance1) * std::sqrt(variance2) + c2) / (variance1 + variance2 + c2);
        double structure = (covariance + c2 / 2.0) / (std::sqrt(variance1) * std::sqrt(variance2) + c2 / 2.0);

        return static_cast<float>(luminance * contrast * structure);
    }
} // namespace Astrelis
