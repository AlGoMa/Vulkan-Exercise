#pragma once
#include <GeometryMesh.h>
#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>
#include <map>
#include <array>
#include <cstddef>

class Entity : 
    public App::Utilities::Gfx::GeometryMesh
{
public:
    Entity(void) = default;
    ~Entity(void) = default;

    inline VulkanBuffer* GetVertexBuffer (VulkanBuffer::BufferType in_eType) { return m_mBuffers.find(in_eType)->second; }
    bool                 AddBuffer (VulkanBuffer* in_pBuffer);
    bool                 RemoveBuffer (VulkanBuffer::BufferType in_eBuffer);
    virtual bool         LoadObj(std::string in_pszFile) override;
    virtual void         Destroy (void);

private:
    std::map<VulkanBuffer::BufferType, VulkanBuffer*> m_mBuffers;
};
