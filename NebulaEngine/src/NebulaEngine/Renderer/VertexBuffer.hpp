#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

namespace Nebula
{
    // TODO(9636D): Maybe this should be in a separate file / ShaderDataType.hpp
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    class ShaderDataTypeHelper
    {
    public:
        static std::uint32_t GetSize(ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 4 * 2;
            case ShaderDataType::Float3:
                return 4 * 3;
            case ShaderDataType::Float4:
                return 4 * 4;
            case ShaderDataType::Mat3:
                return 4 * 3 * 3; // For the alignment, the size is 4 * 3 * 3 instead of 3 * 3 * 3
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 4 * 2;
            case ShaderDataType::Int3:
                return 4 * 3;
            case ShaderDataType::Int4:
                return 4 * 4;
            case ShaderDataType::Bool:
                return 1;
            default:
                NEBULA_CORE_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
            }
        }

        static std::uint32_t GetCount(ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Mat3:
                return 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4;
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
            default:
                NEBULA_CORE_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
            }
        }
    };

    struct VertexBufferElement
    {
        std::string Name;
        ShaderDataType Type;
        std::uint32_t Size;
        std::size_t Offset;
        bool Normalized;

        VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
            Name(name),
            Type(type),
            Size(ShaderDataTypeHelper::GetSize(type)),
            Offset(0),
            Normalized(normalized)
        {
        }
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout(std::initializer_list<VertexBufferElement> elements, std::uint32_t startIndex = 0)
            : m_Elements(elements), m_StartIndex(startIndex)
        {
            CalculateOffsetAndStride();
        }

        std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

        std::uint32_t GetStride() const { return m_Stride; }
        std::uint32_t GetStartIndex() const { return m_StartIndex; }

    private:
        void CalculateOffsetAndStride()
        {
            std::uint32_t offset = 0;
            m_Stride = 0;
            for (VertexBufferElement& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

        std::vector<VertexBufferElement> m_Elements;
        std::uint32_t m_StartIndex;
        std::uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer()                               = default;
        virtual ~VertexBuffer()                      = default;
        VertexBuffer(const VertexBuffer&)            = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&)                 = delete;
        VertexBuffer& operator=(VertexBuffer&&)      = delete;

        virtual void Bind() const   = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, std::uint32_t size) = 0;

        virtual const VertexBufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const VertexBufferLayout& layout) = 0;

        static Ptr<VertexBuffer> Create(std::uint32_t size);
        static Ptr<VertexBuffer> Create(float* vertices, std::uint32_t size);
    };
} // namespace Nebula
