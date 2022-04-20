#pragma once
#include <Resource.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <SwapChain.h>
#include <Entity.h>

class Pipeline final : 
    public Common::Resources::IResource<Pipeline>
{
public:
    Pipeline (void) : m_pRenderPass(VK_NULL_HANDLE) {
        m_mPipelines.insert(std::make_pair(PipelineType::compute, PipelineData{ VK_NULL_HANDLE, VK_NULL_HANDLE }));
        m_mPipelines.insert(std::make_pair(PipelineType::graphics, PipelineData{VK_NULL_HANDLE, VK_NULL_HANDLE}));
    };
    virtual ~Pipeline(void) {};

    enum class ShaderType : uint8_t { vertex, fragment, geometry, compute, tessellation_control, tessellation_eval };
    enum class PipelineType : uint8_t { graphics, compute };

    struct PipelineData
    {
        VkPipelineLayout  m_ppLayout;
        VkPipeline        m_pp;
    };

    VkResult CreateGraphicsPipeline      (void);
    VkResult CreateComputePipeline       (void);
    VkResult CreatePipelineLayout        (void);
    VkResult CreateComputePipelineLayout (void);
    VkResult CreateRenderPass            (void);
    VkResult CreateCommandBuffers        (void);
    VkResult UpdateCommandBuffers        (Entity* in_pEntity, const uint32_t in_unFrame);
    VkResult UpdateCSCommandBuffers      (const uint32_t in_unFrame);
    void     CreateShader                (const std::string in_strPathToFile, const char* in_strEntryPoint, ShaderType in_eType);
    void     DestroyPipeLine             (void);
    void     DestroyComputePipeLine      (void);
    void     DestroyBuffers              (void);
    void     Destroy                     (void);
    
    inline const VkRenderPass       GetRenderpass     (void)                  const    { return m_pRenderPass; }
    inline const auto&              GetCommandBuffers (void)                  const    { return m_vCmdBuffers; }
    inline const VkPipelineLayout   GetLayout         (PipelineType in_eType) noexcept { return m_mPipelines[in_eType].m_ppLayout; }
    inline const VkPipeline         GetPipeline       (PipelineType in_eType) noexcept { return m_mPipelines[in_eType].m_pp; }

private:
    VkShaderModule CreateShaderModule(const std::string& filename, VkStructureType in_unType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);

    std::vector<VkPipelineShaderStageCreateInfo> m_vShaderRes;
    std::vector<VkCommandBuffer>                 m_vCmdBuffers;
    VkRenderPass                                 m_pRenderPass;
    std::map<PipelineType, PipelineData>         m_mPipelines;
};