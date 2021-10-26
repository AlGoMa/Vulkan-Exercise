#include "GeometryMesh.h"
using namespace App::Utilities::Gfx;

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
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X - vMiddlePoint.X, in_vPosition.Y - vMiddlePoint.Y, in_vPosition.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, -1.0f, -1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X + vMiddlePoint.X, in_vPosition.Y - vMiddlePoint.Y, in_vPosition.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 1.0f, -1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X + vMiddlePoint.X, in_vPosition.Y + vMiddlePoint.Y, in_vPosition.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 1.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X - vMiddlePoint.X, in_vPosition.Y + vMiddlePoint.Y, in_vPosition.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, -1.0f, 1.0f));
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
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X - vMiddlePoint.X, in_vPosition.Y, in_vPosition.Z + vMiddlePoint.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 0.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X + vMiddlePoint.X, in_vPosition.Y, in_vPosition.Z + vMiddlePoint.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 1.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X - vMiddlePoint.X, in_vPosition.Y, in_vPosition.Z - vMiddlePoint.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 0.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X + vMiddlePoint.X, in_vPosition.Y, in_vPosition.Z - vMiddlePoint.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 1.0f, 1.0f));
        m_aVertexInfo.push_back(Vertex(Vector(in_vPosition.X, in_vPosition.Y + in_vSize.Y, in_vPosition.Z), Vector(0.0f, 1.0f, 0.0f), in_vSolidColor, 0.5f, 0.5f));
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
        m_aVertexInfo.push_back(Vertex(Vector(0.0f, -0.5f, 0.0f), Vector(0.0f, 0.0f, 1.0f), in_vSolidColor, 0.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(0.5f, 0.5f, 0.0f), Vector(0.0f, 0.0f, 1.0f), in_vSolidColor, 1.0f, 0.0f));
        m_aVertexInfo.push_back(Vertex(Vector(-0.5f, 0.5f, 0.0f), Vector(0.0f, 0.0f, 1.0f), in_vSolidColor, 1.0f, 1.0f));

        m_aIndexlist.push_back(0);
        m_aIndexlist.push_back(1);
        m_aIndexlist.push_back(2);
    }
    break;
    case GeometryMesh::MeshesType::Cube:
        break;
    }
}

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

                streamMesh >> vVertice.X >> vVertice.Y >> vVertice.Z;

                //vVertice = Vector((float)d_x, (float)d_y, (float)d_z);

                tmpVertices.push_back(vVertice);
            }
            else if (!strcmp(strLine.c_str(), "vn"))
            {
                // Vertex Normal
                
                Vector vNormal ( 0.0f, 0.0f, 0.0f, 0.0f);

                streamMesh >> vNormal.X >> vNormal.Y >> vNormal.Z;
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


void GeometryMesh::Destroy(void)
{
    m_aIndexlist.clear();
    m_aVertexInfo.clear();
}