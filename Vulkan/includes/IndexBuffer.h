#pragma once
#include <VulkanBuffer.h>

class IndexBuffer : 
    public VulkanBuffer
{
public:
    IndexBuffer(void) : VulkanBuffer(Buffer::BufferType::index) 
    { 
        m_mBufferContainer.insert(std::make_pair("Index", std::vector<Buffer::DescBuffer>{}));
        m_mBufferContainer["Index"].resize(1);
    };
    ~IndexBuffer(void) = default;

    virtual bool            CreateBuffer(const void* in_pData, const uint32_t in_unSize) override; 
};
