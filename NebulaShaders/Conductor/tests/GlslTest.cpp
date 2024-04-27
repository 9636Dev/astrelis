#include <gtest/gtest.h>

#include "NebulaShaderConductor/Conductor.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"
#include "NebulaShaderConductor/TargetProfile.hpp"
#include "TestSources.hpp"

using Nebula::ShaderConductor::ShaderInput, Nebula::ShaderConductor::GLSLOutput,
    Nebula::ShaderConductor::ShaderConductor, Nebula::ShaderConductor::TargetProfile;

// Fixture for testing the ShaderConductor class
class GlslTest : public ::testing::Test
{
protected:
    ShaderInput input; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    GLSLOutput output; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)

    void SetUp() override {}

    void TearDown() override {}

    void CompileShader(const std::string& source, const std::string& entryPoint,
                       Nebula::Shader::ShaderStage stage, int majorVersion, int minorVersion,
                       Nebula::ShaderConductor::OptimizationLevel optimizationLevel,
                       std::uint32_t targetVersion = 0)
    {
        input.Profile    = TargetProfile(stage, majorVersion, minorVersion);
        input.Source     = source;
        input.EntryPoint = entryPoint;

        output.Optimization = optimizationLevel;
        output.Version = targetVersion;
        output.Enable420PackExtension = false;

        ShaderConductor conductor;
        auto result = conductor.CompileToSPIRV(input, output);
        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.errorMessage, "");
        EXPECT_GT(result.spirvCode.size(), 0);

        auto glslResult = ShaderConductor::CompileToGLSL(result.spirvCode, output);
        EXPECT_TRUE(glslResult.first.second.empty());
        EXPECT_GT(glslResult.first.first.size(), 0);
    }
};

TEST_F(GlslTest, SimpleVertexShader)
{
    const auto& source     = TestSources::hlslSources[0];
    const auto& entryPoint = TestSources::vertexEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Vertex, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}

TEST_F(GlslTest, SimplePixelShader)
{
    const auto& source     = TestSources::hlslSources[0];
    const auto& entryPoint = TestSources::pixelEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Pixel, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}

TEST_F(GlslTest, SimpleTexturedVertexShader)
{
    const auto& source     = TestSources::hlslSources[1];
    const auto& entryPoint = TestSources::vertexEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Vertex, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}

TEST_F(GlslTest, SimpleTexturedPixelShader)
{
    const auto& source     = TestSources::hlslSources[1];
    const auto& entryPoint = TestSources::pixelEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Pixel, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}
