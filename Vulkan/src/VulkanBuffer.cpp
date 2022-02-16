#include <VulkanBuffer.h>
#include <VulkanManager.h>
#include <array>

uint32_t VulkanBuffer::FindMemoryType(uint32_t in_unTypeFilter, VkMemoryPropertyFlags in_unProperties)
{
    VkPhysicalDeviceMemoryProperties stMemProperties;
    vkGetPhysicalDeviceMemoryProperties(VulkanInstance->GetDevice()->GetPhysicalDevice(), &stMemProperties);

    for (uint32_t _ = 0; _ < stMemProperties.memoryTypeCount; _++)
    {
        if ((in_unTypeFilter & (1 << _)) && (stMemProperties.memoryTypes[_].propertyFlags & in_unProperties) == in_unProperties)
        {
            return _;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

void VulkanBuffer::CopyBuffer(VkBuffer in_unSrcBuffer, VkBuffer in_unDstBuffer, VkDeviceSize in_unSize)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = VulkanInstance->GetDevice()->GetCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer pstCommandBuffer;
    vkAllocateCommandBuffers(VulkanInstance->GetDevice()->GetDevice(), &allocInfo, &pstCommandBuffer);

    VkCommandBufferBeginInfo stBeginInfo{};
    stBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    stBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(pstCommandBuffer, &stBeginInfo);

    VkBufferCopy stCopyRegion{};
    stCopyRegion.size = in_unSize;
    vkCmdCopyBuffer(pstCommandBuffer, in_unSrcBuffer, in_unDstBuffer, 1, &stCopyRegion);

    vkEndCommandBuffer(pstCommandBuffer);

    VkSubmitInfo stSubmitInfo{};
    stSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    stSubmitInfo.commandBufferCount = 1;
    stSubmitInfo.pCommandBuffers = &pstCommandBuffer;

    vkQueueSubmit(VulkanInstance->GetDevice()->GetGfxQueue(), 1, &stSubmitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(VulkanInstance->GetDevice()->GetGfxQueue());

    vkFreeCommandBuffers(VulkanInstance->GetDevice()->GetDevice(), VulkanInstance->GetDevice()->GetCommandPool(), 1, &pstCommandBuffer);
}

bool VulkanBuffer::CreateBuffer(VkDeviceSize          in_unSize,
                                VkBufferUsageFlags    in_unUsage,
                                VkMemoryPropertyFlags in_unProperties,
                                VkSharingMode         in_eSharingmode,
                                Buffer::DescBuffer&   in_stBuffer)
{
    bool bReturn = false;

    VkBufferCreateInfo stBufferInfo{};
    stBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stBufferInfo.size = in_unSize;
    stBufferInfo.usage = in_unUsage;
    stBufferInfo.sharingMode = in_eSharingmode;

    if (VK_SUCCESS != vkCreateBuffer(VulkanInstance->GetDevice()->GetDevice(), &stBufferInfo, nullptr, &in_stBuffer.m_Buffer))
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements stMemReqs;
    vkGetBufferMemoryRequirements(VulkanInstance->GetDevice()->GetDevice(), in_stBuffer.m_Buffer, &stMemReqs);

    VkMemoryAllocateInfo stAllocInfo{};
    stAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    stAllocInfo.allocationSize = stMemReqs.size;
    stAllocInfo.memoryTypeIndex = FindMemoryType(stMemReqs.memoryTypeBits, in_unProperties);

    if (bReturn = (VK_SUCCESS != vkAllocateMemory(VulkanInstance->GetDevice()->GetDevice(), &stAllocInfo, nullptr, &in_stBuffer.m_BufferMem)))
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    return !bReturn && (!vkBindBufferMemory(VulkanInstance->GetDevice()->GetDevice(), in_stBuffer.m_Buffer, in_stBuffer.m_BufferMem, 0));
}

void VulkanBuffer::DestroyBuffer(void)
{
    for (auto& Buf : m_mBufferContainer)
    {
        for (auto& desc : Buf.second)
        {
            vkDestroyBuffer(VulkanInstance->GetDevice()->GetDevice(), desc.m_Buffer, nullptr);

            vkFreeMemory(VulkanInstance->GetDevice()->GetDevice(), desc.m_BufferMem, nullptr);
        }
    }
}