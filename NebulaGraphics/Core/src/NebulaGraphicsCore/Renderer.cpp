#include "Renderer.hpp"

#include "NebulaCore/Assert.hpp"

namespace Nebula
{
    void Renderer::AddRenderPass(RenderPass renderPass, std::size_t insertionIndex)
    {
        if (insertionIndex == std::numeric_limits<std::size_t>::max())
        {
            m_RenderPasses.push_back(std::move(renderPass));
            m_RenderPassObjectCount.push_back(0);
            insertionIndex = m_RenderPasses.size() - 1;
        }
        else
        {
            NEB_ASSERT(insertionIndex <= m_RenderPasses.size(), "Insertion index is out of bounds");
            auto index = static_cast<std::_Bit_const_iterator::difference_type>(insertionIndex);
            m_RenderPasses.insert(m_RenderPasses.begin() + index, std::move(renderPass));
            m_RenderPassObjectCount.insert(m_RenderPassObjectCount.begin() + index, 0);
        }

        InternalAddRenderPass(m_RenderPasses[insertionIndex], insertionIndex);
    }

    RenderPass Renderer::RemoveRenderPass(std::size_t index)
    {
        NEB_ASSERT(index < m_RenderPasses.size(), "Index is out of bounds");
        auto renderPass = std::move(m_RenderPasses[index]);
        m_RenderPasses.erase(m_RenderPasses.begin() + static_cast<std::_Bit_const_iterator::difference_type>(index));
        std::size_t objectCount = m_RenderPassObjectCount[index];
        m_RenderPassObjectCount.erase(m_RenderPassObjectCount.begin() + static_cast<std::_Bit_const_iterator::difference_type>(index));
        // Now we need to remove that many elements from the renderable objects vector
        auto offset = GetStartOffsetForRenderPass(index);
        m_RenderableObjects.erase(m_RenderableObjects.begin() + static_cast<std::_Bit_const_iterator::difference_type>(offset), m_RenderableObjects.begin() + static_cast<std::_Bit_const_iterator::difference_type>(offset + objectCount));

        InternalRemoveRenderPass(index);
        return renderPass;
    }

    void Renderer::AddRenderableObject(RenderableObject renderableObject, std::size_t renderPassIndex)
    {
        if (renderPassIndex >= m_RenderPasses.size())
        {
            renderPassIndex = 0;
        }

        m_RenderableObjects.push_back(std::move(renderableObject));
        m_RenderPassObjectCount[renderPassIndex]++;

        InternalAddRenderableObject(m_RenderableObjects.back(), renderPassIndex);
    }

    std::size_t Renderer::GetIndexOfRenderPass(const std::string& name) const
    {
        for (std::size_t i = 0; i < m_RenderPasses.size(); ++i)
        {
            if (m_RenderPasses[i].Name == name)
            {
                return i;
            }
        }

        return std::numeric_limits<std::size_t>::max();
    }

    std::size_t Renderer::GetStartOffsetForRenderPass(std::size_t renderPassIndex) const
    {
        NEB_ASSERT(renderPassIndex < m_RenderPasses.size(), "Render pass index is out of bounds");
        std::size_t offset = 0;
        for (std::size_t i = 0; i < renderPassIndex; ++i)
        {
            offset += m_RenderPassObjectCount[i];
        }

        return offset;
    }
} // namespace Nebula
