#pragma once
#include <GeometryMesh.h>
#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>
#include <map>
#include <array>
#include <cstddef>

class RenderEntity : 
    public App::Utilities::Gfx::GeometryMesh
{
public:
    RenderEntity(void) = default;
    ~RenderEntity(void) = default;

    inline VulkanBuffer* GetBuffer (VulkanBuffer::BufferType in_eType) { return m_mBuffers.find(in_eType)->second; }
    bool                 AddBuffer (VulkanBuffer* in_pBuffer);
    bool                 RemoveBuffer (VulkanBuffer::BufferType in_eBuffer);
    virtual bool         LoadObj(const std::string in_pszFile) override;
    virtual void         Destroy (void);

private:
    std::map<VulkanBuffer::BufferType, VulkanBuffer*> m_mBuffers;
};
