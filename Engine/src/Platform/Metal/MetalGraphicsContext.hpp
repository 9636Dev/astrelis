#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

namespace Astrelis
{
    // We have to use PIMPL to not expose Metal headers to the client
    class MetalGraphicsContext final : public GraphicsContext
    {
    public:
        explicit MetalGraphicsContext(RawRef<GLFWwindow*> window);
        ~MetalGraphicsContext() override;
        MetalGraphicsContext(const MetalGraphicsContext&)            = delete;
        MetalGraphicsContext& operator=(const MetalGraphicsContext&) = delete;
        MetalGraphicsContext(MetalGraphicsContext&&)                 = delete;
        MetalGraphicsContext& operator=(MetalGraphicsContext&&)      = delete;

        Result<EmptyType, std::string> Init() final;
        void Shutdown() final;
        bool IsInitialized() const final;
        void BeginFrame() final;
        void EndFrame() final;
        bool SkipFrame() final;

        bool IsVSync() const final;
        void SetVSync(bool enabled) final;

        std::uint32_t GetCurrentFrameIndex() const final;
        std::uint32_t GetImageIndex() const final;


        static RefPtr<MetalGraphicsContext> Create(void* window);
    private:
        class Impl;
        Impl* m_Impl;
    };
} // namespace Astrelis
