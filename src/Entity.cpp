#include "Entity.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

bool Entity::AddBuffer(VulkanBuffer* in_pBuffer)
{ 
    bool bResult = false;

    if (nullptr != in_pBuffer)
    {
        if (VulkanBuffer::BufferType::index == in_pBuffer->GetBufferType())
        { 
            bResult = in_pBuffer->CreateBuffer(m_aIndexlist.data(), sizeof(m_aIndexlist[0]) * m_aIndexlist.size() );
        } 
        else if (VulkanBuffer::BufferType::vertex == in_pBuffer->GetBufferType())
        {
            bResult = in_pBuffer->CreateBuffer(m_aVertexInfo.data(), sizeof(m_aVertexInfo[0]) * m_aVertexInfo.size());
        }

        m_mBuffers.insert(std::make_pair(in_pBuffer->GetBufferType(), in_pBuffer));
    }

    return bResult;
}

bool Entity::RemoveBuffer(VulkanBuffer::BufferType in_eBuffer)
{
    bool bResult = false;

    if (bResult = (std::end(m_mBuffers) != m_mBuffers.find(in_eBuffer)))
    {
        m_mBuffers[in_eBuffer]->DestroyBuffer();

        delete m_mBuffers[in_eBuffer];

        m_mBuffers.erase(in_eBuffer);
    }

    return bResult;
}

bool Entity::LoadObj(std::string in_strObject)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, in_strObject.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    struct RawVertex { uint32_t index; App::Utilities::Gfx::Vertex v;  };

    std::vector<RawVertex> uniqueVertices;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            App::Utilities::Gfx::Vertex vertex{ App::Math::Vector(attrib.vertices[3 * index.vertex_index + 0],
                                                                  attrib.vertices[3 * index.vertex_index + 1],
                                                                  attrib.vertices[3 * index.vertex_index + 2]),
                                                -1 != index.normal_index ? App::Math::Vector(attrib.normals[3 * index.normal_index + 0],
                                                                                       attrib.normals[3 * index.normal_index + 1],
                                                                                       attrib.normals[3 * index.normal_index + 2]) : App::Math::Vector(),
                                                App::Math::Vector(1.0f, 1.0f, 1.0f),
                                                attrib.texcoords[2 * index.texcoord_index + 0],
                                                1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };

            auto aVertex = std::find_if(uniqueVertices.cbegin(), uniqueVertices.cend(),
                                  [=](const RawVertex& vx) {
                                      return 0 == memcmp(&vertex, &vx.v, sizeof(App::Utilities::Gfx::Vertex));
                                  });

            uint32_t Index = 0;

            if (std::end(uniqueVertices) == aVertex)
            {
                Index = static_cast<uint32_t>(m_aVertexInfo.size());
                uniqueVertices.push_back({ Index, vertex });
                m_aVertexInfo.push_back(vertex);
            }
            else
            {
                Index = (*aVertex).index;
            }

            m_aIndexlist.push_back(Index);
        }
    }

    return true;
}

void Entity::Destroy(void)
{
    __super::Destroy();

    for (auto bf : m_mBuffers)
    {
        bf.second->DestroyBuffer();

        delete bf.second;
    }

    m_mBuffers.clear();
}