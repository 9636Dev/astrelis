#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Result.hpp"

#include <cstdint>

struct GLFWwindow;

namespace Astrelis {

    /// @brief Properties for the graphics context.
    /// @note These properties are not guaranteed to be supported by all graphics contexts, and some may only be 'suggestions' to the context.
    struct ContextProps {
        /// @brief Whether or not VSync should be enabled by default
        bool VSync = true;
    };

    /**
    * @brief Per window graphics context, contains per window rendering state and resources.
    */
    class GraphicsContext {
    public:
        GraphicsContext()                                  = default;
        virtual ~GraphicsContext()                         = default;
        GraphicsContext(const GraphicsContext&)            = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
        GraphicsContext(GraphicsContext&&)                 = delete;
        GraphicsContext& operator=(GraphicsContext&&)      = delete;

        /// @brief Initialize the graphics context.
        /// @return Result<EmptyType, std::string> - EmptyType on success, error message on failure.
        virtual Result<EmptyType, std::string> Init() = 0;
        /// @brief Shutdown the graphics context.
        virtual void Shutdown()            = 0;
        virtual bool IsInitialized() const = 0;
        /// @brief Performs frame actions that are required to be done at the beginning of the frame.
        virtual void BeginFrame() = 0;
        /// @brief Performs frame actions that are required to be done at the end of the frame.
        virtual void EndFrame() = 0;
        /// @brief Whether or not the current frame should be skipped (e.g. minimized window or swapchain recreation).
        virtual bool SkipFrame() = 0;

        /// @brief Whether or not VSync is enabled.
        virtual bool IsVSync() const = 0;
        /// @brief Set the VSync state.
        /// @param enabled - Whether or not VSync should be enabled.
        virtual void SetVSync(bool enabled) = 0;

        /// @brief Get the current frame index.
        /// @return std::uint32_t - The current frame index, or 0 for contexts that do not support frame indices.
        virtual std::uint32_t GetCurrentFrameIndex() const = 0;
        /// @brief Get the current image index.
        /// @return std::uint32_t - The current image index, or 0 for contexts that do not support image indices.
        virtual std::uint32_t GetImageIndex() const = 0;

        /// @brief Create a new graphics context.
        /// @param window - The window to create the context for.
        /// @param props - The properties to use for the context.
        /// @return RefPtr<GraphicsContext> - The created graphics context.
        static RefPtr<GraphicsContext> Create(RawRef<GLFWwindow*> window, ContextProps props = {});
    };
} // namespace Astrelis
