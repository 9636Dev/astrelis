#pragma once

/**
* @file GL.hpp
* @brief This file wraps all the OpenGL functionality, and does not include any other OpenGL headers.
*/


#include "Enum.hpp"
#include <string_view>

namespace Nebula::OpenGL
{

    class GL
    {
    public:
        GL() = delete;

        using VertexArray_t = std::uint32_t;
        using Buffer_t      = std::uint32_t;
        using Shader_t      = std::uint32_t;
        using Program_t     = std::uint32_t;
        using Texture_t     = std::uint32_t;
        using sizei_t       = std::int32_t;
        using sizeiptr_t    = std::int64_t;
        using intptr_t      = std::ptrdiff_t;

        static bool Init();
        static void Shutdown();

        static void ClearColor(float red, float green, float blue, float alpha);
        static void Clear(ClearBufferMask mask);

        static void GetIntegerv(GetTarget target, std::int32_t& data);
    private:
        static void CheckError(const std::string_view& function, const std::string_view& file, std::int32_t line);
        static void DebugCallback(std::uint32_t source,
                                  std::uint32_t type,
                                  std::uint32_t errorId,
                                  std::uint32_t severity,
                                  sizei_t msg_length,
                                  const char* message,
                                  const void* userParam);

        struct FunctionCall
        {
            std::string_view Function;
            std::string_view File;
            std::int32_t Line;
        };

        static bool s_Initialized;
        static bool s_DebugOutput;
#ifdef NEBULA_DEBUG
        static FunctionCall s_CurrentFunction;
#endif
    };
} // namespace Nebula::OpenGL
