#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include "SwapChain.h"
#include <array>
#include "VulkanManager.h"
#include <GLFW/glfw3native.h>

VkResult SwapChain::CreateSurface(const void* in_Window)
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window((GLFWwindow*)in_Window);
    createInfo.hinstance = GetModuleHandle(nullptr);

    return vkCreateWin32SurfaceKHR(VulkanInstance->GetApp(), &createInfo, nullptr, &m_pSurface);
}

VkResult SwapChain::CreateSwapChain(const void* in_Window) {
    SCSupportDetails stSwapChainSupport = QuerySwapChainSupport();
    
    VkSurfaceFormatKHR stSurfaceFormat = ChooseSwapSurfaceFormat(stSwapChainSupport.formats);
    VkPresentModeKHR stPresentMode = ChooseSwapPresentMode(stSwapChainSupport.presentModes);
    VkExtent2D stExtent = ChooseSwapExtent(stSwapChainSupport.capabilities, in_Window);

    uint32_t unImageCount = stSwapChainSupport.capabilities.minImageCount + 1;
    if ((0 < stSwapChainSupport.capabilities.maxImageCount) && (unImageCount > stSwapChainSupport.capabilities.maxImageCount)) {
        unImageCount = stSwapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_pSurface;

    createInfo.minImageCount = unImageCount;
    createInfo.imageFormat = stSurfaceFormat.format;
    createInfo.imageColorSpace = stSurfaceFormat.colorSpace;
    createInfo.imageExtent = stExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VulkanDevice::FamilyType cachedFam = VulkanInstance->GetDevice()->GetFamily();

    if (cachedFam.m_uGPFam != cachedFam.m_uPntFam) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = std::vector<uint32_t>{cachedFam.m_uGPFam.value(), cachedFam.m_uPntFam.value()}.data();
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = stSwapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = stPresentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    m_vSwapChainImageFormat = stSurfaceFormat.format;
    m_vSwapChainExtent = stExtent;

    return static_cast<VkResult>(vkCreateSwapchainKHR(VulkanInstance->GetDevice()->GetDevice(), &createInfo, nullptr, &m_pSwapChain) | CreateImages());
}

SwapChain::SCSupportDetails  SwapChain::QuerySwapChainSupport(void) 
{
    SCSupportDetails  stDetails;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanInstance->GetDevice()->GetPhysicalDevice(), m_pSurface, &stDetails.capabilities);

    uint32_t unFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanInstance->GetDevice()->GetPhysicalDevice(), m_pSurface, &unFormatCount, nullptr);

    if (0 != unFormatCount) {
        stDetails.formats.resize(unFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanInstance->GetDevice()->GetPhysicalDevice(), m_pSurface, &unFormatCount, stDetails.formats.data());
    }

    uint32_t unPresentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanInstance->GetDevice()->GetPhysicalDevice(), m_pSurface, &unPresentModeCount, nullptr);

    if (0 != unPresentModeCount) {
        stDetails.presentModes.resize(unPresentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanInstance->GetDevice()->GetPhysicalDevice(), m_pSurface, &unPresentModeCount, stDetails.presentModes.data());
    }

    return stDetails;
}

VkResult SwapChain::CreateFramebuffers(void)
{
    m_vSwapChainFramebuffers.resize(m_vSwapChainImageViews.size());

    for (size_t i = 0; i < m_vSwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            m_vSwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = VulkanInstance->GetPipeline()->GetRenderpass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_vSwapChainExtent.width;
        framebufferInfo.height = m_vSwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanInstance->GetDevice()->GetDevice(), &framebufferInfo, nullptr, &m_vSwapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }

    return static_cast<VkResult>(!std::all_of(m_vSwapChainFramebuffers.cbegin(), m_vSwapChainFramebuffers.cend(),
        [](const VkFramebuffer& in_buffer)
        {
            return in_buffer != VK_NULL_HANDLE;
        }));
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& in_vAvailableFormats)
{
    return (*std::find_if (in_vAvailableFormats.crbegin(), in_vAvailableFormats.crend(), [](const auto& format) {
        return VK_FORMAT_B8G8R8A8_SRGB == format.format && VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == format.colorSpace;
    }));
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& in_vAvailablePresentModes) {
    return (*std::find_if(in_vAvailablePresentModes.crbegin(), in_vAvailablePresentModes.crend(), [](const auto& present) {
        return VK_PRESENT_MODE_MAILBOX_KHR == present || VK_PRESENT_MODE_FIFO_KHR == present;
    }));
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const void* in_Window) {
    VkExtent2D out_ActualExtent = capabilities.currentExtent;
    
    if (UINT32_MAX == capabilities.currentExtent.width) {
        int width, height;
        glfwGetFramebufferSize((GLFWwindow*)in_Window, &width, &height);

        out_ActualExtent.width = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        out_ActualExtent.height = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }

    return out_ActualExtent;
}

VkResult SwapChain::CreateImages(void)
{
    uint32_t unImageCount = 0;

    if (VK_SUCCESS == vkGetSwapchainImagesKHR(VulkanInstance->GetDevice()->GetDevice(), m_pSwapChain, &unImageCount, nullptr))
    {
        m_vSwapChainImages.resize(unImageCount);
    }

    if (VK_SUCCESS == vkGetSwapchainImagesKHR(VulkanInstance->GetDevice()->GetDevice(), m_pSwapChain, &unImageCount, m_vSwapChainImages.data()))
    {
        m_vSwapChainImageViews.resize(unImageCount);
    }

    for (size_t i = 0; i < m_vSwapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_vSwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_vSwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(VulkanInstance->GetDevice()->GetDevice(), &createInfo, nullptr, &m_vSwapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }

    return static_cast<VkResult>(!std::all_of(m_vSwapChainImageViews.cbegin(), m_vSwapChainImageViews.cend(),
        [](const VkImageView& in_img)
        {
            return in_img != VK_NULL_HANDLE;
        }));
}

VkResult SwapChain::CreateDescriptorPool(void)
{
    VkResult eResult = VK_SUCCESS;

    std::array<VkDescriptorPoolSize, 3> aPoolSizes{};
    aPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    aPoolSizes[0].descriptorCount = static_cast<uint32_t>(m_vSwapChainImages.size());
    aPoolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    aPoolSizes[1].descriptorCount = static_cast<uint32_t>(m_vSwapChainImages.size());
    aPoolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    aPoolSizes[2].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(aPoolSizes.size());
    poolInfo.pPoolSizes = aPoolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(m_vSwapChainImages.size());

    if ((eResult = vkCreateDescriptorPool(VulkanInstance->GetDevice()->GetDevice(), &poolInfo, nullptr, &m_DescriptorPool)) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    return eResult;
}

VkResult SwapChain::CreateDescriptorSets(void)
{
    VkResult eResult = VK_SUCCESS;

    m_vDescriptorSets.resize(m_vSwapChainImages.size());

    for (size_t i = 0; i < m_vSwapChainImages.size(); i++)
    {
        VkDescriptorSetAllocateInfo stAllocInfo{};
        stAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        stAllocInfo.descriptorPool = m_DescriptorPool;
        stAllocInfo.descriptorSetCount = 1;
        stAllocInfo.pSetLayouts = &m_pConstantBuffer->GetDescriptor();

        if ((eResult = vkAllocateDescriptorSets(VulkanInstance->GetDevice()->GetDevice(), &stAllocInfo, &m_vDescriptorSets[i])) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        VkDescriptorBufferInfo stBufferInfo{};
        stBufferInfo.buffer = m_pConstantBuffer.get()->GetBuffer("Uniform",i).m_Buffer;
        stBufferInfo.offset = 0;
        stBufferInfo.range = sizeof(UniformData);

        VkDescriptorBufferInfo stBufferSceneInfo{};
        stBufferSceneInfo.buffer = m_pConstantBuffer.get()->GetBuffer("Scene", 0).m_Buffer;
        stBufferSceneInfo.offset = 0;
        stBufferSceneInfo.range = sizeof(SceneData); 

        std::array<VkWriteDescriptorSet,2> stDescriptorWrites{};
        stDescriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        stDescriptorWrites[0].dstSet = m_vDescriptorSets[i];
        stDescriptorWrites[0].dstBinding = 0;
        stDescriptorWrites[0].dstArrayElement = 0;
        stDescriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        stDescriptorWrites[0].descriptorCount = 1;
        stDescriptorWrites[0].pBufferInfo = &stBufferInfo;
        stDescriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        stDescriptorWrites[1].dstSet = m_vDescriptorSets[i];
        stDescriptorWrites[1].dstBinding = 1;
        stDescriptorWrites[1].dstArrayElement = 0;
        stDescriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        stDescriptorWrites[1].descriptorCount = 1;
        stDescriptorWrites[1].pBufferInfo = &stBufferSceneInfo;

        vkUpdateDescriptorSets(VulkanInstance->GetDevice()->GetDevice(), stDescriptorWrites.size(), stDescriptorWrites.data(), 0, nullptr);
    }

    return eResult;
}

void SwapChain::DestroySwapChain(void) 
{
    for (auto framebuffer : m_vSwapChainFramebuffers) {
        vkDestroyFramebuffer(VulkanInstance->GetDevice()->GetDevice(), framebuffer, nullptr);
    }

    for (auto imageView : m_vSwapChainImageViews) {
        vkDestroyImageView(VulkanInstance->GetDevice()->GetDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(VulkanInstance->GetDevice()->GetDevice(), m_pSwapChain, nullptr);

    m_pConstantBuffer.get()->DestroyBuffer();

    vkDestroyDescriptorPool(VulkanInstance->GetDevice()->GetDevice(), m_DescriptorPool, nullptr);
}

void SwapChain::DestroySurface(void)
{
    vkDestroySurfaceKHR(VulkanInstance->GetApp(), m_pSurface, nullptr);
}
