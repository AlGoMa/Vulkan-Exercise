#include "GeometryMesh.h"
using namespace App::Utilities::Gfx;

using axis = Vector::axis;

void  GeometryMesh::BuildBasisMesh(GeometryMesh::MeshesType in_eMesh,
                                   Vector in_vPosition,
                                   Vector in_vSize,
                                   Vector in_vSolidColor)
{
    Vector vMiddlePoint = in_vSize;

    switch (in_eMesh)
    {
    case GeometryMesh::MeshesType::Plane:
    {
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] - vMiddlePoint[axis::x], in_vPosition[axis::y] - vMiddlePoint[axis::y], in_vPosition[axis::z]), Vector::Up(), in_vSolidColor, -1.0f, -1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] + vMiddlePoint[axis::x], in_vPosition[axis::y] - vMiddlePoint[axis::y], in_vPosition[axis::z]), Vector::Up(), in_vSolidColor, 1.0f, -1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] + vMiddlePoint[axis::x], in_vPosition[axis::y] + vMiddlePoint[axis::y], in_vPosition[axis::z]), Vector::Up(), in_vSolidColor, 1.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] - vMiddlePoint[axis::x], in_vPosition[axis::y] + vMiddlePoint[axis::y], in_vPosition[axis::z]), Vector::Up(), in_vSolidColor, -1.0f, 1.0f));
        //, 1, 3, 3, 1, 2
        m_aIndexlist.push_back(0);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(2);
        m_aIndexlist.push_back(2);
        m_aIndexlist.push_back(3);
        m_aIndexlist.push_back(0);
    }
    break;
    case GeometryMesh::MeshesType::Piramid:
    {
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] - vMiddlePoint[axis::x], in_vPosition[axis::y], in_vPosition[axis::z] + vMiddlePoint[axis::z]), Vector::Up(), in_vSolidColor, 0.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] + vMiddlePoint[axis::x], in_vPosition[axis::y], in_vPosition[axis::z] + vMiddlePoint[axis::z]), Vector::Up(), in_vSolidColor, 1.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] - vMiddlePoint[axis::x], in_vPosition[axis::y], in_vPosition[axis::z] - vMiddlePoint[axis::z]), Vector::Up(), in_vSolidColor, 0.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x] + vMiddlePoint[axis::x], in_vPosition[axis::y], in_vPosition[axis::z] - vMiddlePoint[axis::z]), Vector::Up(), in_vSolidColor, 1.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition[axis::x], in_vPosition[axis::y] + in_vSize[axis::y], in_vPosition[axis::z]), Vector::Up(), in_vSolidColor, 0.5f, 0.5f));
        //, 1, 3, 3, 1, 2
        /*Normals calculation. Slow process.*/
        Vector V = m_aVertexInfo[1].m_vPosition - m_aVertexInfo[0].m_vPosition;
        Vector U = m_aVertexInfo[2].m_vPosition - m_aVertexInfo[0].m_vPosition;

        m_aVertexInfo[0].m_vNormals = Vector::Cross(V, U);
        Vector::Normalize(m_aVertexInfo[0].m_vNormals);

        V = m_aVertexInfo[0].m_vPosition - m_aVertexInfo[1].m_vPosition;
        U = m_aVertexInfo[2].m_vPosition - m_aVertexInfo[1].m_vPosition;

        m_aVertexInfo[1].m_vNormals = Vector::Cross(V, U);
        Vector::Normalize(m_aVertexInfo[1].m_vNormals);

        V = m_aVertexInfo[0].m_vPosition - m_aVertexInfo[2].m_vPosition;
        U = m_aVertexInfo[1].m_vPosition - m_aVertexInfo[2].m_vPosition;

        m_aVertexInfo[2].m_vNormals = Vector::Cross(V, U);
        Vector::Normalize(m_aVertexInfo[2].m_vNormals);

        V = m_aVertexInfo[0].m_vPosition - m_aVertexInfo[3].m_vPosition;
        U = m_aVertexInfo[2].m_vPosition - m_aVertexInfo[3].m_vPosition;

        m_aVertexInfo[3].m_vNormals = Vector::Cross(V, U);
        Vector::Normalize(m_aVertexInfo[3].m_vNormals);

        m_aIndexlist.push_back(0);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(2);
        m_aIndexlist.push_back(2);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(3);
        m_aIndexlist.push_back(3);
        m_aIndexlist.push_back(2);
        m_aIndexlist.push_back(4);
        m_aIndexlist.push_back(4);
        m_aIndexlist.push_back(2);
        m_aIndexlist.push_back(0);
        m_aIndexlist.push_back(0);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(4);
        m_aIndexlist.push_back(4);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(3);
    }
    break;
    case GeometryMesh::MeshesType::Triangle:
    {
        m_aVertexInfo.push_back(Vertex(Vector(0.0f, -0.5f, 0.0f), Vector::Forward(), in_vSolidColor, 0.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(0.5f, 0.5f, 0.0f),  Vector::Forward(), in_vSolidColor, 1.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(-0.5f, 0.5f, 0.0f), Vector::Forward(), in_vSolidColor, 0.0f, 1.0f));

        m_aIndexlist.push_back(0);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(2);
    }
    break;
    case GeometryMesh::MeshesType::Cube:
        break;
    }
}

[[deprecated]]
bool GeometryMesh::LoadObj(std::string in_pszFile)
{
    struct UV
    {
        float u, v;
    };

    std::vector<Vector>     tmpNormals,
                            tmpVertices;
    std::vector<UV>         tmpUVs;
    std::string             strLine;

    std::ifstream streamMesh(in_pszFile, std::ios::in);

    if (streamMesh)
    {
        while ((streamMesh >> strLine))
        {
            if (!strcmp(strLine.c_str(), "v"))
            {
                Vector vVertice;
                streamMesh >> vVertice[axis::x] >> vVertice[axis::y] >> vVertice[axis::z];
                tmpVertices.push_back(vVertice);
            }
            else if (!strcmp(strLine.c_str(), "vn"))
            {
                // Vertex Normal
                Vector vNormal ( 0.0f, 0.0f, 0.0f, 0.0f);
                streamMesh >> vNormal[axis::x] >> vNormal[axis::y] >> vNormal[axis::z];
                tmpNormals.push_back(vNormal);
            }
            else if (!strcmp(strLine.c_str(), "vt"))
            {
                // Vertex TexCoord
                UV uvs;
                streamMesh >> uvs.u >> uvs.v;
                tmpUVs.push_back(uvs);
            }
            else if (!strcmp(strLine.c_str(), "f"))
            {
                uint32_t  unPosition,
                          unTexCoord,
                          unNormal;
                Vertex	vertex;

                for (unsigned int nFaces = 0; nFaces < 3; nFaces++)
                {
                    ZeroMemory(&vertex, sizeof(Vertex));

                    streamMesh >> unPosition;
                    vertex.m_vPosition = tmpVertices[unPosition - 1];

                    if ('/' == streamMesh.peek())
                    {
                        streamMesh.ignore();

                        if ('/' != streamMesh.peek())
                        {
                            streamMesh >> unTexCoord;

                            /*!< Due OBJ file in UVS an Normals begins in 1 to N, it is necessary decrease in one every value to force the begin in 0.*/
                            vertex.m_fUcoord = tmpUVs[unTexCoord - 1].u;
                            vertex.m_fVcoord = tmpUVs[unTexCoord - 1].v;
                        }

                        if ('/' == streamMesh.peek())
                        {
                            streamMesh.ignore();
                            streamMesh >> unNormal;
                            vertex.m_vNormals = tmpNormals[unNormal - 1];
                        }
                    }
                    uint32_t ulIndex = 0;

                    auto c = std::find_if(m_aVertexInfo.cbegin(), m_aVertexInfo.cend(),
                                            [=, &ulIndex](const Vertex& vx) {
                                                ++ulIndex;
                                                
                                                return !memcmp(&vertex, &vx, sizeof(Vertex));
                                            });

                    if (std::end(m_aVertexInfo) == c)
                    {
                        ulIndex = m_aVertexInfo.size();
                        
                        m_aVertexInfo.push_back(vertex);
                    }
                    else
                    {
                        ulIndex -= 1;
                    }

                    m_aIndexlist.push_back(ulIndex);
                }
            }
        }
    }

    streamMesh.close();

    return true;
}

const App::Math::RayTraicing::Hitable<GeometryMesh>::stHitRecord GeometryMesh::Hit(const RayTraicing::Ray& in_rRay, const float in_fMin, const float in_fMax) const
{
    stHitRecord out_stHitInfo {0};
    Vector vOriginCtr = (in_rRay.Origin() - m_vPosition);
                                      
    float fBB  = Vector::Dot(in_rRay.Direction(), in_rRay.Direction());
    float fBAC = 2.0f * Vector::Dot(in_rRay.Direction(), vOriginCtr);
    float fAC = Vector::Magnitude(vOriginCtr) - (1.5f * 1.5f);
    float fDiscriminant = fBB * fBB - fAC * fBAC;

    if (fDiscriminant > 0.0f)
    {
        float fFront = (-fBAC - sqrt(fDiscriminant)) / fBB;

        if (in_fMin < fFront && in_fMax > fFront)
        {
            out_stHitInfo.m_isHitted = true;
            out_stHitInfo.m_fDelta = fFront;
        }
        else
        {
            float fBack = (-fBAC + sqrt(fDiscriminant)) / fBB;

            if (in_fMin < fBack && in_fMax > fBack)
            {
                out_stHitInfo.m_isHitted = true;
                out_stHitInfo.m_fDelta = fBack;
            }
        }

        out_stHitInfo.m_vPoint = const_cast<RayTraicing::Ray&>(in_rRay).PointAtParam(out_stHitInfo.m_fDelta);
        out_stHitInfo.m_vNormal = (out_stHitInfo.m_vPoint - m_vPosition) / 1.0f;
    }

    return out_stHitInfo;
}

void GeometryMesh::Destroy(void)
{
    m_aIndexlist.clear();
    m_aVertexInfo.clear();
}
