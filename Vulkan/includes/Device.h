#pragma once
#include <Resource.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

class Device final:
    public Common::Resources::IResource<Device>
{
public:
    Device(void) : m_pDevice(VK_NULL_HANDLE), 
                   m_pPyshicalDev(VK_NULL_HANDLE), 
                   m_gpuProperties({0}), 
                   m_pCommandPool(0), 
                   m_pPresentQueue(VK_NULL_HANDLE),
                   m_pGraphicsQueue(VK_NULL_HANDLE) {};
    virtual ~Device(void) {};

    struct FamilyType {
        std::optional<uint32_t> m_uGPFam;
        std::optional<uint32_t> m_uPntFam;

        bool IsSupported() {
            return m_uGPFam.has_value() && m_uPntFam.has_value();
        }
    };

    inline const VkPhysicalDevice           GetPhysicalDevice (void) const { return m_pPyshicalDev; }
    inline const VkDevice                   GetDevice         (void) const { return m_pDevice; }
    inline const VkQueue                    GetGfxQueue       (void) const { return m_pPresentQueue; }
    inline const VkQueue                    GetPresentQueue   (void) const { return m_pGraphicsQueue; }
    inline const Device::FamilyType         GetFamily         (void) const { return m_stFamily; }
    inline const VkCommandPool              GetCommandPool    (void) const { return m_pCommandPool; }
    inline const VkPhysicalDeviceProperties GetGPUProperties  (void) const { return m_gpuProperties; }

    VkResult CreateLogicalDevice(const std::vector<const char*> in_vDevExtensions,
                                 const std::vector<const char*> in_vValidationLayers);

    VkResult CreateCommandPool  (void);

    void Destroy(void);

private:
    FamilyType SelectPhysicalDevice       (void);
    bool       QueryFamily                (const VkPhysicalDevice in_Device,  __out FamilyType& out_vFamily);
    bool       CheckDeviceExtensionSupport(const VkPhysicalDevice in_Device);
    VkResult   CreateDeviceQueues         (void);

    FamilyType                 m_stFamily;
    VkDevice                   m_pDevice;
    VkPhysicalDevice           m_pPyshicalDev;
    VkCommandPool              m_pCommandPool;
    VkQueue                    m_pPresentQueue;
    VkQueue                    m_pGraphicsQueue;
    VkPhysicalDeviceProperties m_gpuProperties;
    std::vector<const char*>   m_vCachedDeviceExt;
};

