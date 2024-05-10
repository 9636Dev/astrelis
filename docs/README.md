# Nebula Engine

## Overview
Nebula Engine is a 2D or 3D game engine written in C++.

## Architecture

### Entity-Component-System

The engine is component-based and uses an Entity-Component-System (ECS) architecture.
Entities are by default GameObjects, which are containers for components.
Rendered entities must have a RenderComponent attached, containing a Mesh, Material, and Transform.
Only entities with RenderComponent or UIComponent are submitted to the renderer for rendering, and vice versa for physics.

### Rendering

The engine dynamically loads rendering backends at runtime by loading from dynamic link libraries (DLLs).
The engine currently supports OpenGL, DirectX12, and Metal rendering backends.
The renderer is consists of Render Passes, which are responsible for rendering a specific type of object.
Render Passes can only have one type of Shader per pass, the engine will automatically add entities to the correct pass based on the shader type.
UI elements also use a separate render pass, which is rendered after all other passes.

The default rendering pipeline is as follows:
- Solid Geometry Pass
- Transparent Geometry Pass
- UI Pass
- Post-Processing Pass

Users can create custom render passes and shaders by creating a new class that inherits from the RenderPass, and can be inserted anywhere in the pipeline.

### Resources

#### Meshes

The engine uses a custom mesh format. There are 4 types of meshes:
- `StaticMesh2D` - A 2D mesh that does not change.
- `DynamicMesh2D` - A 2D mesh that can change.
- `StaticMesh3D` - A 3D mesh that does not change.
- `DynamicMesh3D` - A 3D mesh that can change.

The file format specifications are found here: (INSERT LINK ONCE FILE FORMAT IS FINALIZED)

#### Textures

The engine uses the STB image library to load images. The engine supports the following image formats:
- PNG
- JPEG
- BMP

The texture type and location are stored in a texture object, which is then stored in a texture manager.
The textures can be stored in a texture atlas, which is a single texture that contains multiple textures.
This can be controlled by the user in the texture manager.

#### Materials

The engine uses a custom material format. The material contains the shader, textures, and uniform data.
This is yet to be finalized.

### Input

The engine wraps GLFW for input handling. The engine supports the following input types:
- Keyboard
- MouseButton
- MouseMovement
- MouseScroll (both horizontal and vertical for trackpads)

### Editor

The engine has a custom editor that is still in development.

### Physics

The engine uses a custom physics engine that is still in development.

### Audio

The design is still in development.

### Scripting

Still in development.

## Features
- 2D and 3D rendering (3D rendering is still in development)
- 2D and 3D physics (still in development)
- Audio (still in development)
- Input (still in development)
- Networking (still in development)
- Scripting (still in development)
- UI (still in development)
- Editor (still in development)
- Cross-platform Runtimes (Windows, Linux, MacOS)
- Various rendering backends (OpenGL, DirectX12, Metal)

## License

The license is yet to be determined.
