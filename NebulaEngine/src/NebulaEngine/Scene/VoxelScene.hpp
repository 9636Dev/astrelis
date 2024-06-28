#pragma once

#include <cstddef>
#include <glm/glm.hpp>

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
        glm::vec4 Color;

        constexpr glm::ivec3 GetPosition() const
        {
            return {
                (Position >> (BITS_PER_POSITION * 2)) & POSITION_BITMASK,
                (Position >> BITS_PER_POSITION) & POSITION_BITMASK,
                Position & POSITION_BITMASK,
            };
        }

        constexpr static std::uint32_t GetPackedPosition(glm::ivec3 position)
        {
            return (position.x << (BITS_PER_POSITION * 2)) | (position.y << BITS_PER_POSITION) | position.z;
        }

        constexpr static Voxel Create(glm::ivec3 offset, glm::vec4 color) { return {GetPackedPosition(offset), color}; }
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
            glm::ivec3 Position;
            std::vector<Voxel> Voxels;
        };

        std::vector<Chunk> m_Chunks;
    };
} // namespace Nebula
