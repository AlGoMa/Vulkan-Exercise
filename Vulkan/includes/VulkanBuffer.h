#pragma once
#include <Buffer.h>
#include <vulkan/vulkan.h>

class VulkanBuffer :
    public App::Utilities::Gfx::Buffer<VkBuffer, VkDeviceMemory, VkDescriptorSetAllocateInfo>
{
public:
    VulkanBuffer(BufferType in_eType) : Buffer<VkBuffer, VkDeviceMemory, VkDescriptorSetAllocateInfo>(in_eType) {};
    ~VulkanBuffer(void) = default;

    virtual bool            CreateBuffer(const void* in_pData, const uint32_t in_unSize) = 0;
    virtual void            DestroyBuffer(void) override;

protected:
    bool     CreateBuffer   (VkDeviceSize in_unSize, VkBufferUsageFlags in_unSsage, VkMemoryPropertyFlags in_uPproperties, VkSharingMode in_eSharingmode, Buffer::DescBuffer& in_stBuffer);
    void     CopyBuffer     (VkBuffer in_unSrcBuffer, VkBuffer in_unDstBuffer, VkDeviceSize in_unSize);
    uint32_t FindMemoryType (uint32_t in_unTypeFilter, VkMemoryPropertyFlags in_unProperties);
};
