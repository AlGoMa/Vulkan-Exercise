#pragma once
#include <vector>
#include <Buffer.h>
#include <map>
#include <string>
#include <fstream>
#include <Ray.h>

namespace App::Utilities::Gfx
{
    class GeometryMesh :
        public RayTraicing::Hittable<GeometryMesh>
    {
    public:
        GeometryMesh(const Vector in_vPosition = Vector::Zero()) : m_vPosition(in_vPosition), m_unMeshLOD(0) {};
        virtual ~GeometryMesh(void) {};

    public:
        enum class MeshesType : uint8_t
        {
            Triangle = 0,
            Plane,
            Cube,
            Piramid
        };

        virtual bool           LoadObj        (const std::string in_strFile);
        virtual void           BuildBasisMesh (GeometryMesh::MeshesType in_eMesh,
                                               Vector in_vPosition = Vector(),
                                               Vector in_vSize = Vector(1.f, 1.f, 1.f),
                                               Vector in_vSolidColor = Vector(1.f, 1.f, 1.f));

        virtual const   stHitRecord Hit       (const RayTraicing::Ray& in_rRay, const float in_fMin, const float in_fMax) const;

        inline auto     GetVertex(void) const { return m_aVertexInfo; }
        inline auto     GetIndex (void) const { return m_aIndexlist; }

        virtual void    Destroy(void);

    protected:
        Vector                m_vPosition;
        std::vector<Vertex>   m_aVertexInfo;
        std::vector<t_IntU32> m_aIndexlist;
        uint32_t              m_unMeshLOD;
    };
}