#pragma once
#include "VulkanBuffer.h"
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
using namespace App::Math;

class StorageBuffer :
    public VulkanBuffer
{
public:
    StorageBuffer(void) : VulkanBuffer(VulkanBuffer::BufferType::storge), m_ComputerSetLayout(VK_NULL_HANDLE) {
        m_mBufferContainer.insert(std::make_pair("Storage", std::vector<Buffer::DescBuffer>{}));
        m_mBufferContainer["Storage"].resize(2);
    };
    ~StorageBuffer(void) = default;
    
    virtual bool                        CreateBuffer    (const void* in_pData = nullptr, const uint32_t in_unSize = 0) override;
    virtual void                        Destroy         (void);
    void                                UpdateBuffer    (const uint32_t in_unFrame);
    bool                                CreateDescriptor(void);

    inline const VkDescriptorSetLayout& GetDescriptor   (void) const { return m_ComputerSetLayout; }

private:
    VkDescriptorSetLayout       m_ComputerSetLayout;
};

