#include <gtest/gtest.h>

#include "NebulaShaderConductor/Conductor.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"
#include "NebulaShaderConductor/TargetProfile.hpp"
#include "TestSources.hpp"

using Nebula::ShaderConductor::ShaderInput, Nebula::ShaderConductor::ShaderOutput,
    Nebula::ShaderConductor::ShaderConductor, Nebula::ShaderConductor::TargetProfile;

// Fixture for testing the ShaderConductor class
class SpirvTest : public ::testing::Test
{
protected:
    ShaderInput input; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    ShaderOutput output; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)

    void SetUp() override {}

    void TearDown() override {}

    void CompileShader(const std::string& source, const std::string& entryPoint,
                       Nebula::Shader::ShaderStage stage, int majorVersion, int minorVersion,
                       Nebula::ShaderConductor::OptimizationLevel optimizationLevel)
    {
        input.Profile    = TargetProfile(stage, majorVersion, minorVersion);
        input.Source     = source;
        input.EntryPoint = entryPoint;

        output.Optimization = optimizationLevel;

        ShaderConductor conductor;
        auto result = conductor.CompileToSPIRV(input, output);
        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.errorMessage, "");
        EXPECT_GT(result.spirvCode.size(), 0);
    }
};

TEST_F(SpirvTest, SimpleVertexShader)
{
    const auto& source     = TestSources::hlslSources[0];
    const auto& entryPoint = TestSources::vertexEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Vertex, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}

TEST_F(SpirvTest, SimplePixelShader)
{
    const auto& source     = TestSources::hlslSources[0];
    const auto& entryPoint = TestSources::pixelEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Pixel, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}

TEST_F(SpirvTest, SimpleTexturedVertexShader)
{
    const auto& source     = TestSources::hlslSources[1];
    const auto& entryPoint = TestSources::vertexEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Vertex, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}

TEST_F(SpirvTest, SimpleTexturedPixelShader)
{
    const auto& source     = TestSources::hlslSources[1];
    const auto& entryPoint = TestSources::pixelEntry;

    CompileShader(source, entryPoint, Nebula::Shader::ShaderStage::Pixel, 6, 0,
                  Nebula::ShaderConductor::OptimizationLevel::None);
}
