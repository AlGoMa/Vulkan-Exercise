#include <VertexBuffer.h>
#include <VulkanManager.h>

bool VertexBuffer::CreateBuffer(const void* in_pData, const uint32_t in_unSize)
{
    bool bResult = false;

    VkDeviceSize bufferSize = in_unSize;
    VkBufferUsageFlags unFlags = 0;

    Buffer::DescBuffer stagingBuffer;
    bResult = __super::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, stagingBuffer);

    void* data;
    vkMapMemory(VulkanInstance->GetDevice()->GetDevice(), stagingBuffer.m_BufferMem, 0, bufferSize, 0, &data);

    memcpy(data, in_pData, (size_t)bufferSize);

    vkUnmapMemory(VulkanInstance->GetDevice()->GetDevice(), stagingBuffer.m_BufferMem);

    bResult &= __super::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE, m_mBufferContainer["Vertex"].front());

    CopyBuffer(stagingBuffer.m_Buffer, m_mBufferContainer["Vertex"].front().m_Buffer, bufferSize);

    vkDestroyBuffer(VulkanInstance->GetDevice()->GetDevice(), stagingBuffer.m_Buffer, nullptr);
    vkFreeMemory(VulkanInstance->GetDevice()->GetDevice(), stagingBuffer.m_BufferMem, nullptr);

    return !bResult;
}
