#pragma once

#include <simd/simd.h>
#include <vector>

struct Vertex {
    vector_float3 Position;
};

struct Mesh {
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;
};
