#pragma once

#include <cstddef>
#include <utility>

#include "NebulaEngine/Core/Math.hpp"

namespace Nebula
{
    constexpr std::uint32_t VOXEL_CHUNK_SIZE  = 64;
    constexpr std::uint32_t BITS_PER_POSITION = 6;
    constexpr std::uint32_t POSITION_BITMASK  = 0b111111;

    struct Voxel
    {
        // 6 * 3 = 24 < 32
        std::uint32_t Position;
        static_assert(sizeof(Position) * 8 >= static_cast<std::uint64_t>(BITS_PER_POSITION * 3));
        Vec4f Color;

        Vec3u GetPosition() const
        {
            return {
                (Position >> (BITS_PER_POSITION * 2)) & POSITION_BITMASK,
                (Position >> BITS_PER_POSITION) & POSITION_BITMASK,
                Position & POSITION_BITMASK,
            };
        }

        static std::uint32_t GetPackedPosition(Vec3u position)
        {
            return (position.x << (BITS_PER_POSITION * 2)) | (position.y << BITS_PER_POSITION) | position.z;
        }

        static Voxel Create(Vec3u offset, Vec4f color)
        {
            return {GetPackedPosition(std::move(offset)), std::move(color)};
        }
    };

    class VoxelScene
    {
    public:
        friend class VoxelRenderer;
        VoxelScene()                                   = default;
        ~VoxelScene()                                  = default;
        VoxelScene(const VoxelScene& other)            = default;
        VoxelScene& operator=(const VoxelScene& other) = default;
        VoxelScene(VoxelScene&& other)                 = default;
        VoxelScene& operator=(VoxelScene&& other)      = default;

        struct Chunk
        {
            Vec3i Position;
            std::vector<Voxel> Voxels;
        };

        std::vector<Chunk> m_Chunks;
    };
} // namespace Nebula
