#include "StorageBuffer.h"
#include <VulkanManager.h>
#include <array>

bool StorageBuffer::CreateDescriptor(void)
{
    bool bResult = false;

    std::array<VkDescriptorSetLayoutBinding,2> aUboLayoutBinding{};
    aUboLayoutBinding[0].binding = 0;
    aUboLayoutBinding[0].descriptorCount = 1;
    aUboLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    aUboLayoutBinding[0].pImmutableSamplers = nullptr;
    aUboLayoutBinding[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    aUboLayoutBinding[1].binding = 1; 
    aUboLayoutBinding[1].descriptorCount = 1;
    aUboLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    aUboLayoutBinding[1].pImmutableSamplers = nullptr;
    aUboLayoutBinding[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = aUboLayoutBinding.size();
    layoutInfo.pBindings = aUboLayoutBinding.data();

    if (vkCreateDescriptorSetLayout(VulkanInstance->GetDevice()->GetDevice(), &layoutInfo, nullptr, &m_ComputerSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create compute descriptor set layout!");
    }

    return bResult;
}

bool StorageBuffer::CreateBuffer(const void* in_pData, const uint32_t in_unSize)
{
    bool bResult = false;

    VkDeviceSize bufferSize = sizeof(glm::vec4) * 15;

    bResult = __super::CreateBuffer(sizeof(SceneData), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, m_mBufferContainer["Storage"][0]);

    bufferSize = sizeof(glm::vec4) * 5;

    bResult |= __super::CreateBuffer(sizeof(SceneData), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, m_mBufferContainer["Storage"][1]);

    return !bResult;
}

void StorageBuffer::UpdateBuffer(const uint32_t in_unFrame)
{
    //vkMapMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Storage"][in_unFrame].m_BufferMem, 0, VK_WHOLE_SIZE, 0, positions);

    //vkUnmapMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Storage"][in_unFrame].m_BufferMem);
}

void StorageBuffer::Destroy(void)
{
    vkDestroyDescriptorSetLayout(VulkanInstance->GetDevice()->GetDevice(), m_ComputerSetLayout, nullptr);
    
    for (auto& buffer : m_mBufferContainer["Storage"])
    {
        vkDestroyBuffer(VulkanInstance->GetDevice()->GetDevice(), buffer.m_Buffer, nullptr);
        vkFreeMemory(VulkanInstance->GetDevice()->GetDevice(), buffer.m_BufferMem, nullptr);
    }
}
