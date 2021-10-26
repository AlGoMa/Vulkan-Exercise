#pragma once
#include <Resource.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include "Device.h"
#include <vector>
#include "ConstantBuffer.h"
#include "StorageBuffer.h"

class SwapChain :
    public App::Resources::IResource<SwapChain>
{
public:
    SwapChain(void) { m_pConstantBuffer = std::make_unique<ConstantBuffer>();  m_pStorageBuffer = std::make_unique<StorageBuffer>();
    };
    ~SwapChain(void) = default;

    struct SCSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    VkResult CreateSurface(const void* in_Window);

    VkResult CreateSwapChain(const void* in_Window);
    
    VkResult CreateFramebuffers(void);

    VkResult CreateDescriptorPool(void);

    VkResult CreateDescriptorSets(void);
 
    void DestroySwapChain(void);
    void DestroySurface  (void);

    inline const VkSurfaceKHR     GetSurface     (void) const { return m_pSurface; }
    inline const VkSwapchainKHR   GetSwapChain   (void) const { return m_pSwapChain; }
    inline const VkExtent2D       GetExtents     (void) const { return m_vSwapChainExtent; }
    inline const VkFormat         GetFormats     (void) const { return m_vSwapChainImageFormat; }
    inline const auto&            GetFramebuffer (void) const { return m_vSwapChainFramebuffers; }
    inline const auto&            GetViews       (void) const { return m_vSwapChainImageViews; }
    inline const auto&            GetSCImages    (void) const { return m_vSwapChainImages; }
    inline const auto&            GetDescriptors (void) const { return m_vDescriptorSets; }
    inline       ConstantBuffer*  GetCB          (void) const { return m_pConstantBuffer.get(); }
    inline       StorageBuffer*   GetSB          (void) const { return m_pStorageBuffer.get(); }

private:
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const void* in_Window);
    
    SCSupportDetails QuerySwapChainSupport(void);

    VkResult CreateImages (void);
    
    std::unique_ptr<ConstantBuffer>    m_pConstantBuffer;
    std::unique_ptr<StorageBuffer>     m_pStorageBuffer;
    VkSurfaceKHR                       m_pSurface;
    VkSwapchainKHR                     m_pSwapChain;
    std::vector<VkImage>               m_vSwapChainImages;
    VkFormat                           m_vSwapChainImageFormat;
    VkExtent2D                         m_vSwapChainExtent;
    VkDescriptorPool                   m_DescriptorPool;
    std::vector<VkDescriptorSet>       m_vDescriptorSets;
    std::vector<VkImageView>           m_vSwapChainImageViews;
    std::vector<VkFramebuffer>         m_vSwapChainFramebuffers;
    const std::vector<const char*>     m_vDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

