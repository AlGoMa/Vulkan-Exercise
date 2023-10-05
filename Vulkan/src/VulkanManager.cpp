#include "VulkanManager.h"
#include <set>

bool VulkanManager::CheckValidationLayerSupport(const std::vector<const char*> in_vLayers) {
    bool bResult = false;

    t_IntU32 u32LayerCount;
    (void)vkEnumerateInstanceLayerProperties(&u32LayerCount, nullptr);

    std::vector<VkLayerProperties> vAvailableLayers(u32LayerCount);
    (void)vkEnumerateInstanceLayerProperties(&u32LayerCount, vAvailableLayers.data());

    std::for_each(in_vLayers.cbegin(), in_vLayers.cend(),
        [vAvailableLayers, &bResult](std::string vLayer)
        {
            bResult |= !std::none_of(vAvailableLayers.cbegin(), vAvailableLayers.cend(),
                [vLayer](VkLayerProperties vLayerProp)
                {
                    return vLayer.compare(vLayerProp.layerName) == 0;
                });
        });

    return bResult;
}

const std::vector<const char*> VulkanManager::GetExtensions(void)
{
    t_IntU32 glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> vExt(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (g_bEnableValidationLayers) {
        vExt.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return vExt;
}

VkResult VulkanManager::vkCreateVulkanInstance(const char* in_pszAppName, const void* in_pvWindowInstance) {

    VkApplicationInfo sAppInfo{
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        nullptr,
        in_pszAppName,
        VK_MAKE_VERSION(1, 0, 0),
        "NA",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_0
    };

    auto vExt = GetExtensions();

    VkInstanceCreateInfo stCreateInfo = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        nullptr, 0,
        &sAppInfo,
        0, nullptr,
        vExt.size(),
        vExt.data()
    };

    t_IntU32 extensionCount = 0;
    (void)vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    (void)vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    t_IntU8 unMatches = 0;

    std::for_each(vExt.cbegin(), vExt.cend(),
        [&unMatches, extensions](std::string extension)
        {
            unMatches += static_cast<t_IntU8>(std::end(extensions) != std::find_if(extensions.begin(), extensions.end(),
                [extension](VkExtensionProperties i)
                {
                    return extension.compare(i.extensionName) == 0;
                }));
        });

    if (g_bEnableValidationLayers) {
        stCreateInfo.enabledLayerCount = static_cast<t_IntU32>(m_validationLayers.size());
        stCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
        stCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&m_sCreateInfo;
    }
    else {
        stCreateInfo.enabledLayerCount = 0;
    }
    
    return ((nullptr == m_pVulkanApp) &&
            (unMatches == vExt.size()) &&
            (
                (!g_bEnableValidationLayers) ||
                (g_bEnableValidationLayers && CheckValidationLayerSupport(m_validationLayers))
            )) ?
                (!vkCreateInstance(&stCreateInfo, nullptr, &m_pVulkanApp) &&
                 !m_SwapChain.CreateSurface(in_pvWindowInstance) &&
                 !CreateDebugUtilsMsn(nullptr) &&
                 !m_Device.CreateLogicalDevice(m_vDeviceExtensions, m_validationLayers) &&
                 !m_SwapChain.CreateSwapChain(in_pvWindowInstance) &&
                 !m_Pipeline.CreateRenderPass() &&
                 !m_SwapChain.GetCB()->CreateDescriptor() &&
                 !m_SwapChain.GetSB()->CreateDescriptor() &&
                 !m_SwapChain.CreateFramebuffers() &&
                 !m_Pipeline.CreatePipelineLayout() &&
                 !m_Pipeline.CreateComputePipelineLayout() &&
                 !m_Device.CreateCommandPool() &&
                 !m_Pipeline.CreateCommandBuffers() &&
                 !m_SwapChain.GetCB()->CreateBuffer(nullptr, sizeof(UniformData)) &&
                 !m_SwapChain.GetSB()->CreateBuffer() &&
                 !m_SwapChain.CreateDescriptorPool() &&
                 !m_SwapChain.CreateDescriptorSets() &&
                 !CreateSemaphores()) == true ? VK_SUCCESS : VK_ERROR_INITIALIZATION_FAILED :
                VK_ERROR_INITIALIZATION_FAILED;
}

VkResult VulkanManager::CreateDebugUtilsMsn(const VkAllocationCallbacks* in_pAllocator) {
    VkResult eResult = VK_SUCCESS;

    if (g_bEnableValidationLayers)
    { 
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_pVulkanApp, "vkCreateDebugUtilsMessengerEXT");

        eResult = (nullptr != func) ?
            func(m_pVulkanApp, &m_sCreateInfo, in_pAllocator, &m_pDebugMessenger)
            :
            VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    return eResult;
}

VkResult VulkanManager::DestroyDebugUtilsMsn(const VkAllocationCallbacks* in_pAllocator) {
    VkResult eResult = VK_SUCCESS;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_pVulkanApp, "vkDestroyDebugUtilsMessengerEXT");

    if (nullptr != func)
    {
        func(m_pVulkanApp, m_pDebugMessenger, in_pAllocator);
    }
    else {
        eResult  = VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    return eResult;
}

VkResult VulkanManager::CreateSemaphores(void)
{
    m_vImageAvailableSemaphores.resize(2);
    m_vRenderFinishedSemaphores.resize(2);
    m_vInFlightFences.resize(2);
    m_vImagesInFlight.resize(m_SwapChain.GetSCImages().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    std::vector<bool> vResult;

    for (int i : {0, 1}) {
        vResult.push_back ( vkCreateSemaphore(m_Device.GetDevice(), &semaphoreInfo, nullptr, &m_vImageAvailableSemaphores[i]) | 
                            vkCreateSemaphore(m_Device.GetDevice(), &semaphoreInfo, nullptr, &m_vRenderFinishedSemaphores[i]) |
                            vkCreateFence(m_Device.GetDevice(), &fenceInfo, nullptr, &m_vInFlightFences[i]));
    }

    return static_cast<VkResult>( !std::all_of(vResult.cbegin(), vResult.cend (), [](const auto res) { return res == VK_SUCCESS; }));
}

VkResult VulkanManager::vkDraw(const void* in_pvWindowInstance, const RenderEntity* in_pToDraw, bool in_bResize)
{
    vkWaitForFences(m_Device.GetDevice(), 1, &m_vInFlightFences[m_unFrame], VK_TRUE, UINT64_MAX);

    t_IntU32 unImageIndex = 0U;
    VkResult eResult = vkAcquireNextImageKHR(m_Device.GetDevice(), m_SwapChain.GetSwapChain(), UINT64_MAX, m_vImageAvailableSemaphores[m_unFrame], VK_NULL_HANDLE, &unImageIndex);

    if ((VK_SUBOPTIMAL_KHR == eResult) || (VK_SUCCESS == eResult)) {

        if (m_vImagesInFlight[unImageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(m_Device.GetDevice(), 1, &m_vImagesInFlight[unImageIndex], VK_TRUE, UINT64_MAX);
        }
        
        m_SwapChain.GetCB()->UpdateBuffer(unImageIndex);
        
        m_Pipeline.UpdateCommandBuffers(const_cast<RenderEntity*>(in_pToDraw), unImageIndex);

        m_vImagesInFlight[unImageIndex] = m_vInFlightFences[m_unFrame];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_vImageAvailableSemaphores[m_unFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_Pipeline.GetCommandBuffers()[unImageIndex];

        VkSemaphore signalSemaphores[] = { m_vRenderFinishedSemaphores[m_unFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(m_Device.GetDevice(), 1, &m_vInFlightFences[m_unFrame]);

        if (vkQueueSubmit(m_Device.GetGfxQueue(), 1, &submitInfo, m_vInFlightFences[m_unFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR aSwapChains[] = { m_SwapChain.GetSwapChain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = aSwapChains;

        presentInfo.pImageIndices = &unImageIndex;

        eResult = vkQueuePresentKHR(m_Device.GetPresentQueue(), &presentInfo);

        if (eResult == VK_ERROR_OUT_OF_DATE_KHR || eResult == VK_SUBOPTIMAL_KHR || in_bResize) {
            Recreate(in_pvWindowInstance);
        }
        else if (eResult != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
        
        m_unFrame = (m_unFrame + 1) % 2;
    }
    else if (VK_ERROR_OUT_OF_DATE_KHR == eResult) {
        Recreate(in_pvWindowInstance);
    }

    return eResult;
}

void VulkanManager::Recreate(const void* in_pWindow)
{
    t_IntS32 nWidth = 0, nHeight = 0;

    while ( (0 == nWidth) || (0 == nHeight)) {
        glfwGetFramebufferSize((GLFWwindow*)in_pWindow, &nWidth, &nHeight);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_Device.GetDevice());

    m_SwapChain.DestroySwapChain();
    m_Pipeline.DestroyPipeLine();
    m_Pipeline.DestroyComputePipeLine();

    m_SwapChain.CreateSwapChain(in_pWindow);
    m_Pipeline.CreateRenderPass();
    m_Pipeline.CreatePipelineLayout();
    m_Pipeline.CreateGraphicsPipeline();
    m_Pipeline.CreateComputePipelineLayout();
    m_Pipeline.CreateComputePipeline();
    m_SwapChain.CreateFramebuffers();
    m_SwapChain.GetCB()->CreateBuffer(nullptr, sizeof(UniformData));
    m_SwapChain.CreateDescriptorPool();
    m_SwapChain.CreateDescriptorSets();

    m_vImagesInFlight.resize(m_SwapChain.GetSCImages().size(), VK_NULL_HANDLE);
}

void VulkanManager::vDestroy(void)
{
    m_SwapChain.DestroySwapChain();

    for (t_IntU32 i : {0, 1}) {
        vkDestroySemaphore(m_Device.GetDevice(), m_vRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_Device.GetDevice(), m_vImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_Device.GetDevice(), m_vInFlightFences[i], nullptr);
    }

    m_Pipeline.DestroyBuffers();
    m_Pipeline.Destroy();
    m_SwapChain.GetCB()->Destroy();
    m_SwapChain.GetSB()->Destroy();
    m_Pipeline.DestroyPipeLine();
    m_Pipeline.DestroyComputePipeLine();
    m_Device.Destroy();
    m_SwapChain.DestroySurface();

    if (g_bEnableValidationLayers)
    {
        DestroyDebugUtilsMsn(nullptr);
    }

    vkDestroyInstance(m_pVulkanApp, nullptr);
}
