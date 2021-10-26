#include "Device.h"
#include <set>
#include "VulkanManager.h"

VkResult Device::CreateLogicalDevice(const std::vector<const char*> in_vDevExtensions,
                                     const std::vector<const char*> in_vValidationLayers)
{
    m_vCachedDeviceExt = in_vDevExtensions;

    m_stFamily = SelectPhysicalDevice();

    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> vQueueCreateInfos;

    for (const uint32_t fam : std::set<uint32_t>{ m_stFamily.m_uGPFam.value(), m_stFamily.m_uPntFam.value() })
    {
        VkDeviceQueueCreateInfo tmp_stQueueCreateInfo{};
        tmp_stQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        tmp_stQueueCreateInfo.queueFamilyIndex = fam;
        tmp_stQueueCreateInfo.queueCount = 1;
        tmp_stQueueCreateInfo.pQueuePriorities = &queuePriority;
        vQueueCreateInfos.push_back(tmp_stQueueCreateInfo);
    }

    VkPhysicalDeviceFeatures stDeviceFeatures{};

    VkDeviceCreateInfo stCreateInfo{};
    stCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    stCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(vQueueCreateInfos.size());
    stCreateInfo.pQueueCreateInfos = vQueueCreateInfos.data();
    stCreateInfo.pEnabledFeatures = &stDeviceFeatures;
    stCreateInfo.enabledExtensionCount = static_cast<uint32_t>(in_vDevExtensions.size());
    stCreateInfo.ppEnabledExtensionNames = in_vDevExtensions.data();

#ifdef NDEBUG
    createInfo.enabledLayerCount = 0;
#else
    stCreateInfo.enabledLayerCount = static_cast<uint32_t>(in_vValidationLayers.size());
    stCreateInfo.ppEnabledLayerNames = in_vValidationLayers.data();
#endif

    return static_cast<VkResult>(vkCreateDevice(m_pPyshicalDev, &stCreateInfo, nullptr, &m_pDevice) | CreateDeviceQueues());
}

VkResult Device::CreateCommandPool(void)
{
    VkCommandPoolCreateInfo stPoolInfo{};
    stPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    stPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    stPoolInfo.queueFamilyIndex = GetFamily().m_uGPFam.value();

    return vkCreateCommandPool(m_pDevice, &stPoolInfo, nullptr, &m_pCommandPool);
}

VkResult Device::CreateDeviceQueues(void)
{
    try
    {
        vkGetDeviceQueue(m_pDevice, m_stFamily.m_uGPFam.value(), 0, &m_pGraphicsQueue);
        vkGetDeviceQueue(m_pDevice, m_stFamily.m_uPntFam.value(), 0, &m_pPresentQueue);
        vkGetPhysicalDeviceProperties(m_pPyshicalDev, &m_gpuProperties);

        return VK_SUCCESS;
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;

        return VK_ERROR_INITIALIZATION_FAILED;
    }
}

Device::FamilyType Device::SelectPhysicalDevice(void) 
{
    FamilyType out_sFam { };

    uint32_t unDeviceCount = 0;

    if ((VK_SUCCESS != vkEnumeratePhysicalDevices(Instance->GetApp(), &unDeviceCount, nullptr)) ||
        (0 >= unDeviceCount))
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> vDevices(unDeviceCount);

    (void)vkEnumeratePhysicalDevices(Instance->GetApp(), &unDeviceCount, vDevices.data());

    m_pPyshicalDev = (*std::find_if(vDevices.cbegin(), vDevices.cend(),
        [=, &out_sFam](VkPhysicalDevice in_device)
        {
            return QueryFamily(in_device, out_sFam) && CheckDeviceExtensionSupport(in_device);
        }));

    return out_sFam;
}

bool Device::QueryFamily(const VkPhysicalDevice in_Device, FamilyType& out_vFamily)
{
    uint32_t unQueueFamilyCount = 0, 
             unIndex = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(in_Device, &unQueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> vQueueFamilies(unQueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(in_Device, &unQueueFamilyCount, vQueueFamilies.data());

    return std::end(vQueueFamilies) != std::find_if(vQueueFamilies.cbegin(), vQueueFamilies.cend(),
    [=, &unIndex, &out_vFamily](const auto& fam)
        {
            VkBool32 bPresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(in_Device, unIndex, Instance->GetSC()->GetSurface(), &bPresentSupport);
            
            if ((fam.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (fam.queueFlags & VK_QUEUE_COMPUTE_BIT)) out_vFamily.m_uGPFam = unIndex;
            if (bPresentSupport)  out_vFamily.m_uPntFam = unIndex;

            return out_vFamily.IsSupported();

            ++unIndex;
        });
}

bool Device::CheckDeviceExtensionSupport(const VkPhysicalDevice in_Device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(in_Device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> vAvlbExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(in_Device, nullptr, &extensionCount, vAvlbExtensions.data());


    std::set<std::string> seRequiredExtensions(m_vCachedDeviceExt.begin(), m_vCachedDeviceExt.end());

    for (const auto& extension : vAvlbExtensions) {
        seRequiredExtensions.erase(extension.extensionName);
    }

    return seRequiredExtensions.empty();
}

void Device::Destroy(void)
{
    vkDestroyCommandPool(m_pDevice, m_pCommandPool, nullptr);

    vkDestroyDevice(m_pDevice, nullptr);
}