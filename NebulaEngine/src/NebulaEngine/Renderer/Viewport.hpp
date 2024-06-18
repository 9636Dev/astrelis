#pragma once

namespace Nebula
{
    struct Viewport
    {
        float X;
        float Y;
        float Width;
        float Height;
        float MinDepth;
        float MaxDepth;

        Viewport(float posX, float posY, float width, float height, float minDepth = 0.0F, float maxDepth = 1.0F)
            : X(posX), Y(posY), Width(width), Height(height), MinDepth(minDepth), MaxDepth(maxDepth) {}
    };

} // namespace Nebula
