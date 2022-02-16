#pragma once
#include "VulkanBuffer.h"
#include <Matrix.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace App::Math;

struct UniformData
{
    alignas(16) glm::mat4 m_View;
    alignas(16) glm::mat4 m_Model;
    alignas(16) glm::mat4 m_Projection;
    alignas(16) glm::float32 m_Time;
};

struct SceneData
{
    alignas(16) glm::vec4 sky; 
    alignas(16) glm::vec4 ambientColor;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec4 astroMidColor;
    alignas(16) glm::vec4 astroOutColor;
    alignas(16) glm::vec4 waveColor;
    alignas(16) glm::vec4 MousePosition;
};

class ConstantBuffer :
    public VulkanBuffer
{
public:
    ConstantBuffer(void) : VulkanBuffer(VulkanBuffer::BufferType::constant) {
        m_mBufferContainer.insert(std::make_pair("Uniform", std::vector<Buffer::DescBuffer>{}));
        m_mBufferContainer.insert(std::make_pair("Scene", std::vector<Buffer::DescBuffer>{}));
    };
    ~ConstantBuffer(void) = default;
    
    virtual bool                       CreateBuffer    (const void* in_pData, const uint32_t in_unSize) override;
    virtual void                       DestroyBuffer   (void) override;
    virtual void                       Destroy         (void);
    void                               UpdateBuffer    (const uint32_t in_unFrame);
    bool                               CreateDescriptor(void);

    inline const VkDescriptorSetLayout& GetDescriptor (void) const { return m_DescriptorSetLayout; }
    inline       UniformData&           GetUniformData(void)       { return m_tUniformData; }
    inline       SceneData&             GetSceneData  (void)       { return m_tSceneData; }

private:
    UniformData                 m_tUniformData;
    SceneData                   m_tSceneData;
    VkDescriptorSetLayout       m_DescriptorSetLayout;
};

