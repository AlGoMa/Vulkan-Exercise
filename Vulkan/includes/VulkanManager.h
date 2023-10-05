#pragma once
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"
#include "Singleton.h"
#include "SwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#define VulkanInstance VulkanManager::GetInstance()

class RenderEntity;

#ifdef NDEBUG
constexpr bool g_bEnableValidationLayers = false;
#else
constexpr bool g_bEnableValidationLayers = true;
#endif

class VulkanManager final :
    public App::SingletonPattern::Singleton<VulkanManager>
{
public:
    VulkanManager(void) : m_unFrame(0U),
                          m_pVulkanApp(nullptr),
                          m_pDebugMessenger(VK_NULL_HANDLE)
    {}
    virtual ~VulkanManager(void) = default;
    
    VkResult vkCreateVulkanInstance(const char* in_pszAppName, const void* in_pvWindowInstance);
    VkResult vkDraw(const void* in_pvWindowInstance, const RenderEntity* in_pToDraw, bool in_bResize = false);
    void     vDestroy(void);

    inline const VkInstance   GetApp      (void) const { return m_pVulkanApp; }
    inline       SwapChain*   GetSC       (void)       { return &m_SwapChain; }
    inline       VulkanDevice*      GetDevice   (void)       { return &m_Device; }
    inline       VukanPipeline*    GetPipeline (void)       { return &m_Pipeline; }
    inline void               StopDevice  (void) const { (void)vkDeviceWaitIdle(m_Device.GetDevice()); }
    
private:
    /**
     * \brief 
     * \param in_pAllocator 
     * \return 
     */
    VkResult CreateDebugUtilsMsn(const VkAllocationCallbacks* in_pAllocator);

    VkResult DestroyDebugUtilsMsn(const VkAllocationCallbacks* in_pAllocator);

    bool     CheckValidationLayerSupport(const std::vector<const char*> in_vLayers);
    
    void     Recreate(const void* in_pWindow);

    VkResult CreateSemaphores(void);

    const std::vector<const char*> GetExtensions(void);

    uint32_t                           m_unFrame;
    VkInstance                         m_pVulkanApp;
    VkDebugUtilsMessengerEXT           m_pDebugMessenger;
    std::vector<VkSemaphore>           m_vImageAvailableSemaphores;
    std::vector<VkSemaphore>           m_vRenderFinishedSemaphores;
    std::vector<VkFence>               m_vInFlightFences;
    std::vector<VkFence>               m_vImagesInFlight;
    SwapChain                          m_SwapChain;
    VulkanDevice                             m_Device;
    VukanPipeline                           m_Pipeline;
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
                                                [](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                   VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                   void* pUserData) {
                                                    std::cerr << "validation layer - msg: " << pCallbackData->pMessage
                                                              << "\nid: " << pCallbackData->pMessageIdName 
                                                              << "\nseverity: " << messageSeverity
                                                              << "\ntype: " << messageType << std::endl;
                                                    return VK_FALSE;
                                                },
                                                nullptr
    };
};
