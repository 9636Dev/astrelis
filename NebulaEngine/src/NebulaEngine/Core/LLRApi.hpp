#pragma once

// LLRApi = Low Level Rendering API (Wrapper of Rendering APIs)
// For now, we need to design to wrap around Metal and OpenGL, for cross-platform support, but not optimal for performance

namespace Nebula
{
    class LLRApi
    {
    public:
        LLRApi() = default;
        virtual ~LLRApi() = default;
        LLRApi(const LLRApi &) = delete;
        LLRApi &operator=(const LLRApi &) = delete;
        LLRApi(LLRApi &&) = delete;
        LLRApi &operator=(LLRApi &&) = delete;

        /**
        * @brief Submits a frame to the rendering API
        * @details This function should be called at the end of the frame to submit the frame to the rendering API, on OpenGL, this would be nothing, but on Metal, this would encode the command buffer and present the drawable
        */
        virtual void SubmitFrame() = 0;

        virtual void Clear() = 0;
    };
} // namespace Nebula
