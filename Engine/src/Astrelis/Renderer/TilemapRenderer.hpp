#pragma once

#include "BaseRenderer.hpp"

namespace Astrelis {
    class TilemapRenderer final : public BaseRenderer {
    public:
        TilemapRenderer(RefPtr<Window> window, Rect2Di viewport);
        ~TilemapRenderer() override;
        TilemapRenderer(const TilemapRenderer&)            = delete;
        TilemapRenderer& operator=(const TilemapRenderer&) = delete;
        TilemapRenderer(TilemapRenderer&&)                 = delete;
        TilemapRenderer& operator=(TilemapRenderer&&)      = delete;

        void BeginFrame() override;
        void EndFrame() override;

        void ResizeViewport() override;
        void Shutdown() override;
        bool InitComponents() override;
    private:
    };
} // namespace Astrelis
