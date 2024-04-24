#include "TargetProfile.hpp"

namespace Nebula::ShaderConductor
{
    std::wstring TargetProfile::ToString() const
    {
        std::wstring result;
        switch (Stage)
        {
        case ShaderStage::Vertex:
            result += L"vs";
            break;
        case ShaderStage::Pixel:
            result += L"ps";
            break;
        default:
            break;
        }
        result += L"_";
        result += std::to_wstring(Major);
        result += L"_";
        result += std::to_wstring(Minor);
        return result;
    }
} // namespace Nebula::ShaderConductor
