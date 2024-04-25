#include <gtest/gtest.h>

#include "NebulaGraphicsCore/Window.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern "C" Nebula::WindowCreationResult
        nebulaGraphicsOpenGLCreateGLFWWindow(Nebula::WindowProps<Nebula::OpenGLContext>& props);
#pragma clang diagnostic pop

TEST(WindowTest, CreateWindow) {
    Nebula::WindowProps<Nebula::OpenGLContext> props("Test Window", 800, 600, {4, 1});
    auto result = nebulaGraphicsOpenGLCreateGLFWWindow(props);

    EXPECT_TRUE(result.Error == Nebula::WindowCreationResult::ErrorType::None);
    EXPECT_TRUE(result.Window != nullptr);
}
