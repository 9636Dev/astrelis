#pragma once

#include "NebulaIO/Image.hpp"
namespace Nebula
{
    struct Texture
    {
        std::string Name;
        InMemoryImage Image;
    };
} // namespace Nebula
