#pragma once
#include <Resource.h>
#include <Vector.h>
#include <map>
#include <fstream>
#include <string>
using namespace App::Math;

namespace App::Utilities::Gfx
{
    static std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    struct Vertex
    {
        Vertex(void) :
            m_vPosition(App::Math::Vector()),
            m_vNormals(App::Math::Vector()),
            m_vColor(App::Math::Vector()),
            aUVCoords()
        {
        };

        Vertex(Vector in_vPos, Vector in_vNorm, Vector in_vColor, float in_fUcoord, float in_fVcoord) :
            m_vPosition(in_vPos),
            m_vNormals(in_vNorm),
            m_vColor(in_vColor)
        {
            memcpy(&aUVCoords[0], std::vector<float>{ in_fUcoord, in_fVcoord }.data(), sizeof(aUVCoords));
        };

        bool operator==(const Vertex& other) const
        {
            return m_vPosition == other.m_vPosition &&
                   m_vNormals == other.m_vNormals && 
                   m_vColor == other.m_vColor &&
                   X == other.X &&
                   Y == other.Y;
        }

        Vector m_vPosition,
               m_vNormals,
               m_vColor;

        union
        {
            struct { float X, Y; };

            struct { float m_fUcoord, m_fVcoord; };

            float aUVCoords[2];
        };
    };

    template <typename TBuffer, typename TMem, typename TDescriptor>
    class Buffer : 
        public App::Resources::IResource<Buffer<TBuffer, TMem, TDescriptor>>
    {
    public:
        enum class BufferType : uint8_t
        {
            vertex,
            constant,
            depht,
            index,
            staging,
            storge
        };

    protected:
        Buffer(BufferType in_eBufferType) : m_eType(in_eBufferType) {};
        Buffer(void) = default;
        virtual ~Buffer(void) = default;

        struct DescBuffer
        {
            TBuffer     m_Buffer;
            TMem        m_BufferMem;
            TDescriptor m_Descriptor;
        };

        virtual bool    CreateBuffer (const void* in_pData, const uint32_t in_unSize) = 0;
        virtual void    DestroyBuffer(void) = 0;
        virtual bool    UpdateBuffer (const void* in_pData, const uint32_t in_unSize) { return true; };
        virtual void    Destroy      (void) { m_mBufferContainer.clear(); };
    
    public:
        inline const BufferType GetBufferType(void) const { return m_eType; }
        inline       DescBuffer GetBuffer(const std::string in_strBuffer, const uint32_t in_unIndex = 0) noexcept { return m_mBufferContainer[in_strBuffer][in_unIndex]; }
        
    protected:
        BufferType m_eType;
        std::map<std::string, std::vector<DescBuffer>> m_mBufferContainer;
    };
}