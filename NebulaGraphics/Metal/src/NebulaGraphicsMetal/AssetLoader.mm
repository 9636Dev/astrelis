#include "AssetLoader.hpp"

namespace Nebula
{
    MetalAssetLoader::MetalAssetLoader(id<MTLDevice> device) : m_Device(device)
    {

    }

    bool MetalAssetLoader::LoadShader(const File& file)
    {
        auto result = Shader::Msl::FromFile(file);
        if (!result.has_value())
        {
            return false;
        }

        auto& msl = result.value();

        @autoreleasepool
        {
            id<MTLFunction> vertexFunction = nil;
            id<MTLFunction> fragmentFunction = nil;

            // We create seperate libraries for each shader and load them into the device
            NSError* error = nil;
            NSString* source = [NSString stringWithUTF8String:msl.VertexSource.c_str()];
            auto library = [m_Device newLibraryWithSource:source options:nil error:&error];

            if (error != nullptr)
            {
                NSLog(@"Error creating library: %@", error.localizedDescription);
                return false;
            }

            vertexFunction = [library newFunctionWithName:@"vertexMain"];

            if (vertexFunction == nil)
            {
                NSLog(@"Error creating vertex function");
                return false;
            }

            source = [NSString stringWithUTF8String:msl.PixelSource.c_str()];
            library = [m_Device newLibraryWithSource:source options:nil error:&error];

            if (error != nullptr)
            {
                NSLog(@"Error creating library: %@", error.localizedDescription);
                return false;
            }

            fragmentFunction = [library newFunctionWithName:@"fragmentMain"];


            m_Shaders[msl.Name] = { msl, vertexFunction, fragmentFunction };
            return true;
        }
    }
}  // namespace Nebula
