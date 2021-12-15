#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <Singleton.h>
#include "SwapChain.h"
#include "Device.h"
#include "Pipeline.h"
#include "Entity.h"

#define Instance VulkanManager::GetInstance()

#ifdef NDEBUG
constexpr bool g_bEnableValidationLayers = false;
#else
constexpr bool g_bEnableValidationLayers = true;
#endif

class VulkanManager :
    public App::SingletonPattern::Singleton<VulkanManager>
{
protected:
    VulkanManager(void) : m_pVulkanApp(nullptr),
                          m_pDebugMessenger(VK_NULL_HANDLE),
                          m_unFrame(0U)
    {}

    ~VulkanManager(void) = default;

public:
    VkResult vkCreateVulkanInstance(const char* in_pszAppName, const void* in_pvWindowInstance);
    VkResult vkDraw(const void* in_pvWindowInstance, const Entity* in_pToDraw, bool in_bResize = false);
    void     vDestroy(void);

    inline const VkInstance   GetApp      (void)                    const { return m_pVulkanApp; }
    inline       SwapChain*   GetSC       (void)                          { return &m_SwapChain; }
    inline       Device*      GetDevice   (void)                          { return &m_Device; }
    inline       Pipeline*    GetPipeline (void)                          { return &m_Pipeline; }
    inline void               StopDevice  (void)                          { (void)vkDeviceWaitIdle(m_Device.GetDevice()); }
    
private:
    VkResult CreateDebugUtilsMsn(const VkAllocationCallbacks* pAllocator);

    VkResult DestroyDebugUtilsMsn(const VkAllocationCallbacks* pAllocator);

    bool     CheckValidationLayerSupport(const std::vector<const char*> in_vLayers);
    
    void     Recreate(const void* in_pWindow);

    VkResult CreateSemaphores(void);

    const std::vector<const char*> GetExtensions(void);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData);

    friend class Singleton<VulkanManager>;

    uint32_t                           m_unFrame;
    VkInstance                         m_pVulkanApp;
    VkDebugUtilsMessengerEXT           m_pDebugMessenger;
    std::vector<VkSemaphore>           m_vImageAvailableSemaphores;
    std::vector<VkSemaphore>           m_vRenderFinishedSemaphores;
    std::vector<VkFence>               m_vInFlightFences;
    std::vector<VkFence>               m_vImagesInFlight;
    SwapChain                          m_SwapChain;
    Device                             m_Device;
    Pipeline                           m_Pipeline;
    const std::vector<const char*>     m_validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*>     m_vDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDebugUtilsMessengerCreateInfoEXT m_sCreateInfo{
                                                VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                                                nullptr,
                                                0,
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                                                debugCallback,
                                                nullptr
    };
};
