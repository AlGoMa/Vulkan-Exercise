#include "ConstantBuffer.h"
#include <VulkanManager.h>
#include <array>

bool ConstantBuffer::CreateDescriptor(void)
{
    bool bResult = false;

    std::array<VkDescriptorSetLayoutBinding,2> aUboLayoutBinding{};
    aUboLayoutBinding[0].binding = 0;
    aUboLayoutBinding[0].descriptorCount = 1;
    aUboLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    aUboLayoutBinding[0].pImmutableSamplers = nullptr;
    aUboLayoutBinding[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    aUboLayoutBinding[1].binding = 1;
    aUboLayoutBinding[1].descriptorCount = 1;
    aUboLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    aUboLayoutBinding[1].pImmutableSamplers = nullptr;
    aUboLayoutBinding[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = aUboLayoutBinding.size();
    layoutInfo.pBindings = aUboLayoutBinding.data();

    if ((vkCreateDescriptorSetLayout(VulkanInstance->GetDevice()->GetDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS))
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    return bResult;
}

bool ConstantBuffer::CreateBuffer(const void* in_pData, const uint32_t in_unSize)
{
    bool bResult = false;

    VkDeviceSize bufferSize = sizeof(UniformData);

    m_mBufferContainer["Uniform"].resize(VulkanInstance->GetSC()->GetSCImages().size());
    m_mBufferContainer["Scene"].resize(VulkanInstance->GetSC()->GetSCImages().size());

    for ( auto& Buffers : m_mBufferContainer["Uniform"])
    {
        bResult |= __super::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, Buffers);
    }

    VkDeviceSize limits = VulkanInstance->GetDevice()->GetGPUProperties().limits.minUniformBufferOffsetAlignment;

    bufferSize = ((sizeof(SceneData) + limits - 1) & ~(limits - 1)) * VulkanInstance->GetSC()->GetSCImages().size();

    for (auto& Buffers : m_mBufferContainer["Scene"])
    {
        bResult |= __super::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, Buffers);
    }

    return !bResult;
}

void ConstantBuffer::UpdateBuffer(const uint32_t in_unFrame)
{
    void* data;
    vkMapMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Uniform"][in_unFrame].m_BufferMem, 0, sizeof(UniformData), 0, &data);
    memcpy(data, &m_tUniformData, sizeof(UniformData));
    vkUnmapMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Uniform"][in_unFrame].m_BufferMem);

    VkDeviceSize limits = VulkanInstance->GetDevice()->GetGPUProperties().limits.minUniformBufferOffsetAlignment; 

    VkDeviceSize bufferSize = ((sizeof(SceneData) + limits - 1) & ~(limits - 1)) * in_unFrame;

    char* dataScene;
    vkMapMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Scene"][in_unFrame].m_BufferMem, 0, sizeof(SceneData), 0, (void**)&dataScene);
    memcpy(dataScene, &m_tSceneData, sizeof(SceneData));
    vkUnmapMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Scene"][in_unFrame].m_BufferMem);
}

void ConstantBuffer::Destroy(void)
{
    vkDestroyDescriptorSetLayout(VulkanInstance->GetDevice()->GetDevice(), m_DescriptorSetLayout, nullptr);
}

void ConstantBuffer::DestroyBuffer(void)
{
    for (size_t i = 0; i < VulkanInstance->GetSC()->GetSCImages().size(); i++)
    {
        vkDestroyBuffer(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Uniform"][i].m_Buffer, nullptr);
        vkFreeMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Uniform"][i].m_BufferMem, nullptr);

        vkDestroyBuffer(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Scene"][i].m_Buffer, nullptr);
        vkFreeMemory(VulkanInstance->GetDevice()->GetDevice(), m_mBufferContainer["Scene"][i].m_BufferMem, nullptr);
    }
}
