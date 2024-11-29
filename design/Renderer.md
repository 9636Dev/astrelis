# Overview
The renderer implemented in a hybrid between a draw-list and deferred renderer.

## Draw List
This means that static meshes can be added to the renderer in a draw list (which is grouped using a secondary list), so the static data is contigious in memory.
The draw list is assumed to be sorted by material, and transparent objects are sorted by depth. When submitting, if it is not sorted, you can check it using a utility function.
You can also sort the draw list by material and transparaency (treated as two lists) using a utility function.

## Deferred Renderer
The deferred renderer is a hybrid between client-manageed rendering and renderer-managed (traditional) rendering.
Meshes that are added will be sorted, and instanced and batched as needed, by material, transparency, and mesh. (Same mesh and same material can be instanced, but different materials will be batched separately).
Here, the 'Material' refers to the shader, but the actual material properties could be different (e.g. different textures, colors, etc).
Clients can also submit meshes that are required to be batched or rendered, for example UI, which can be batched and rendered in a single draw call, and particles, which can be rendered in a single draw call using instancing.

## Drawing
The renderer will draw in the following order:
1. Static Meshes (Opaque + Transparent)
2. Dynamic Meshes (Opaque)
3. Dynamic Meshes (Transparent)

Currently skeletal meshes are not supported, but will be added in the future.
Currently lights are not supported, but will be added in the future.
Baked lighting is not supported, but will be added in the future.

# Renderer Types
The renderer is optimized for certain types of use cases, and there are multiple types of renderers instead of a single generic renderer.

## Tilemap Renderer (2D Voxels)
The tilemap renderer is optimized for rendering 2D voxel data, which is designed to be high performance and low memory usage (efficient representation of data, and efficient instancing).
Tiles with the same material shader signature are batched together, and it can be designed to preprocess data to chunk the meshes into larger meshes, which can be instanced.
