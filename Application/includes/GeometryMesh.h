#pragma once
#include <vector>
#include <Buffer.h>
#include <map>
#include <string>
#include <fstream>

namespace App::Utilities::Gfx
{
    class GeometryMesh :
        public Common::Resources::IResource<GeometryMesh>
    {
    public:
        GeometryMesh(void) = default;
        virtual ~GeometryMesh(void) = default;

    public:
        enum class MeshesType : uint8_t
        {
            Triangle = 0,
            Plane,
            Cube,
            Piramid
        };

        virtual bool    LoadObj       (const std::string in_pszFile);
        virtual void    BuildBasisMesh(GeometryMesh::MeshesType in_eMesh,
                                       Vector in_vPosition = Vector(),
                                       Vector in_vSize = Vector(1.0f, 1.0f, 1.0f),
                                       Vector in_vSolidColor = Vector(1.0f, 1.0f, 1.0f));

        inline auto     GetVertex(void) const { return m_aVertexInfo; }
        inline auto     GetIndex (void) const { return m_aIndexlist; }

        virtual void    Destroy(void);

    protected:
        std::vector<Vertex>   m_aVertexInfo;
        std::vector<uint32_t> m_aIndexlist;
        uint32_t              m_unMeshLOD;
    };
}