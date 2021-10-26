#pragma once
#include <VulkanBuffer.h>
#include <array>
using namespace App::Utilities::Gfx;

class VertexBuffer : 
    public VulkanBuffer
{
public:
    VertexBuffer(void) : VulkanBuffer(Buffer::BufferType::vertex) 
    { 
        m_mBufferContainer.insert(std::make_pair("Vertex", std::vector<Buffer::DescBuffer>{})); 
        m_mBufferContainer["Vertex"].resize(1); 
    };
    ~VertexBuffer(void) = default;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, m_vPosition);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, m_vNormals);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, m_vColor);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, aUVCoords);

        return attributeDescriptions;
    }
    
    virtual bool            CreateBuffer(const void* in_pData, const uint32_t in_unSize) override;
};
