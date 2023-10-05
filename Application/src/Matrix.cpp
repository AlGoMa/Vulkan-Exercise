#include <Matrix.h>

using vaxis = App::Math::Vector::axis;

const App::Math::Matrix App::Math::Matrix::mIdentity = App::Math::Matrix::Identity();

App::Math::Matrix::Matrix(const Vector in_vA, 
                          const Vector in_vB, 
                          const Vector in_vC,
                          const Vector in_vD)
{
    m_v1stVector = in_vA;
    m_v2ndVector = in_vB;
    m_v3rdVector = in_vC;
    m_v4rtVector = in_vD;
}

App::Math::Matrix::Matrix(void)
{
    m_v1stVector = Vector::Zero();
    m_v2ndVector = Vector::Zero();
    m_v3rdVector = Vector::Zero();
    m_v4rtVector = Vector::Zero();
}

App::Math::Matrix::Matrix(const bool in_bIdentity)
{
    float fVal = in_bIdentity ? 1.0f : 0.0f;
    m_v1stVector = Vector(fVal, 0.0f, 0.0f);
    m_v2ndVector = Vector(0.0f, fVal, 0.0f);
    m_v3rdVector = Vector(0.0f, 0.0f, fVal);
    m_v4rtVector = Vector(Vector(0.0f), fVal);
}

unsigned int App::Math::Matrix::Determinant(void)
{
    return 0;
}

App::Math::Matrix App::Math::Matrix::Transpose(Matrix& out_mMatrix)
{
    return out_mMatrix = Matrix(Vector(out_mMatrix[0], out_mMatrix[4], out_mMatrix[8],  out_mMatrix[12]),
                  Vector(out_mMatrix[1], out_mMatrix[5], out_mMatrix[9],  out_mMatrix[13]),
                  Vector(out_mMatrix[2], out_mMatrix[6], out_mMatrix[10], out_mMatrix[14]),
                  Vector(out_mMatrix[3], out_mMatrix[7], out_mMatrix[11], out_mMatrix[15]));
}

float App::Math::Matrix::Trace(Matrix& in_mMatrix)
{
    return in_mMatrix.Trace();
}

void App::Math::Matrix::FrustrumPlanes(Matrix& in_mProjMatrix, Matrix& in_mViewMatrix, Vector* out_pPlanes)
{
    Matrix mVP = in_mViewMatrix * in_mProjMatrix;

    Vector out_vPlanes[6];

    //!< Near 
    out_vPlanes[0][0U] = -mVP._m02;
    out_vPlanes[0][1U] = -mVP._m12;
    out_vPlanes[0][2U] = -mVP._m22;
    out_vPlanes[0][3U] = -mVP._m32;

    //!< Far
    out_vPlanes[1][0U] = mVP._m02 - mVP._m03;
    out_vPlanes[1][1U] = mVP._m12 - mVP._m13;
    out_vPlanes[1][2U] = mVP._m22 - mVP._m23;
    out_vPlanes[1][3U] = mVP._m32 - mVP._m33;

    // Calculat e   left plane of frustum.
    out_vPlanes[2][0U] = -(mVP._m03 + mVP._m00);
    out_vPlanes[2][1U] = -(mVP._m13 + mVP._m10);
    out_vPlanes[2][2U] = -(mVP._m23 + mVP._m20);
    out_vPlanes[2][3U] = -(mVP._m33 + mVP._m30);

    // Calculat e   right plane of frustum.
    out_vPlanes[3][0U] = mVP._m00 - mVP._m03;
    out_vPlanes[3][1U] = mVP._m10 - mVP._m13;
    out_vPlanes[3][2U] = mVP._m20 - mVP._m23;
    out_vPlanes[3][3U] = mVP._m30 - mVP._m33;

    // Calculat e   top plane of frustum.
    out_vPlanes[4][0U] = mVP._m01 - mVP._m03;
    out_vPlanes[4][1U] = mVP._m11 - mVP._m13;
    out_vPlanes[4][2U] = mVP._m21 - mVP._m23;
    out_vPlanes[4][3U] = mVP._m31 - mVP._m33;

    // Calculat e   bottom plane of frustum.
    out_vPlanes[5][0U] = -(mVP._m03 + mVP._m01);
    out_vPlanes[5][1U] = -(mVP._m13 + mVP._m11);
    out_vPlanes[5][2U] = -(mVP._m23 + mVP._m21);
    out_vPlanes[5][3U] = -(mVP._m33 + mVP._m31);
}

App::Math::Matrix App::Math::Matrix::PerspectiveMatrix(const float in_fFOV, const float in_fAspectRatio, const float in_fNear, const float in_fFar)
{
    Matrix out_mProjectivMatrix = mIdentity;

    out_mProjectivMatrix._m00 = 1.0f / tanf(in_fAspectRatio * 0.5f);
    out_mProjectivMatrix._m11 = 1.0f / tanf(in_fFOV * 0.5f);
    out_mProjectivMatrix._m22 = (in_fFar + in_fNear) / (in_fFar - in_fNear);
    out_mProjectivMatrix._m32 = (2.0f * (in_fNear * in_fFar)) / (in_fFar - in_fNear);
    out_mProjectivMatrix._m23 = -1.0f;

    return out_mProjectivMatrix;
}

App::Math::Matrix App::Math::Matrix::OrthographicMatrix(float in_fLPlane, float in_fRPlane, float in_fTopPlane,
    float in_fBottomPlane, float in_fNear, float in_fFar)
{
    Matrix out_OrthogaphicMatrix = mIdentity;

    out_OrthogaphicMatrix._m00 = 2.0f / (in_fRPlane - in_fLPlane);
    out_OrthogaphicMatrix._m11 = 2.0f / (in_fTopPlane - in_fBottomPlane);
    out_OrthogaphicMatrix._m22 = 1.0f / (in_fFar - in_fNear);
    out_OrthogaphicMatrix._m32 = in_fNear / (in_fFar - in_fNear);
    out_OrthogaphicMatrix._m30 = (in_fLPlane + in_fRPlane) / (in_fLPlane - in_fRPlane);
    out_OrthogaphicMatrix._m31 = (in_fBottomPlane + in_fTopPlane) / (in_fBottomPlane - in_fTopPlane);

    return out_OrthogaphicMatrix;
}

App::Math::Matrix App::Math::Matrix::LookAtMatrix(const Vector in_vPosition, Vector in_vTarget, Vector in_vUpVector)
{
    Matrix out_mLookAtMatrix;

    Vector vRight, vForward, vUp;

    vForward = (in_vPosition - in_vTarget).Normalize();

    in_vUpVector = Vector::Up();

    vRight = Vector::Cross(in_vUpVector, vForward);

    vUp = Vector::Cross(vForward, vRight);
    /*!< Right column*/
    out_mLookAtMatrix._m00 = vRight[0U];
    out_mLookAtMatrix._m10 = vRight[1U];
    out_mLookAtMatrix._m20 = vRight[2U];

    /*!< Up column*/
    out_mLookAtMatrix._m01 = vUp[0U];
    out_mLookAtMatrix._m11 = vUp[1U];
    out_mLookAtMatrix._m21 = vUp[2U];

    /*!< Position column*/
    out_mLookAtMatrix._m02 = vForward[0U];
    out_mLookAtMatrix._m12 = vForward[1U];
    out_mLookAtMatrix._m22 = vForward[2U];

    out_mLookAtMatrix.m_v4rtVector = Vector(-Vector::Dot(vRight, in_vPosition),
                                            -Vector::Dot(vUp, in_vPosition),
                                            -Vector::Dot(vForward, in_vPosition), 1.0f);

    return out_mLookAtMatrix;
}

App::Math::Matrix App::Math::Matrix::Rotate(Vector in_vAxis, float in_fRadians)
{
    Matrix out_mRotate;

    out_mRotate._m00 = ((in_vAxis[vaxis::x] * in_vAxis[vaxis::x]) * (1 - cosf(in_fRadians))) + cosf(in_fRadians);//1
    out_mRotate._m01 = ((1 - cosf(in_fRadians)) * (in_vAxis[vaxis::x] * in_vAxis[vaxis::y])) - (sinf(in_fRadians) * in_vAxis[vaxis::z]); //1
    out_mRotate._m02 = ((1 - cosf(in_fRadians)) * (in_vAxis[vaxis::x] * in_vAxis[vaxis::z])) + (sin(in_fRadians) * in_vAxis[vaxis::y]); //1
    out_mRotate._m03 = 0.0f;

    out_mRotate._m10 = ((1 - cosf(in_fRadians)) * (in_vAxis[vaxis::x] * in_vAxis[vaxis::y])) + (sinf(in_fRadians) * in_vAxis[vaxis::z]); //1
    out_mRotate._m11 = ((in_vAxis[vaxis::y] * in_vAxis[vaxis::y]) * (1 - cosf(in_fRadians))) + cosf(in_fRadians);//1
    out_mRotate._m12 = ((1 - cosf(in_fRadians)) * (in_vAxis[vaxis::y] * in_vAxis[vaxis::z])) - (sinf(in_fRadians) * in_vAxis[vaxis::x]); //0
    out_mRotate._m13 = 0.0f;

    out_mRotate._m20 = ((1 - cosf(in_fRadians)) * (in_vAxis[vaxis::x] * in_vAxis[vaxis::z])) - (sinf(in_fRadians) * in_vAxis[vaxis::y]); //0
    out_mRotate._m21 = ((1 - cosf(in_fRadians)) * (in_vAxis[vaxis::y] * in_vAxis[vaxis::z])) + (sinf(in_fRadians) * in_vAxis[vaxis::x]); //0
    out_mRotate._m22 = ((in_vAxis[vaxis::z] * in_vAxis[vaxis::z]) * (1 - cosf(in_fRadians))) + cosf(in_fRadians);//0
    out_mRotate._m23 = 0.0f;

    out_mRotate.m_v4rtVector = Vector();

    return out_mRotate;
}


App::Math::Matrix App::Math::Matrix::Rotate(float in_fX, float in_fY, float in_fZ, float in_fRadians)
{
    Matrix out_mRotate;

    out_mRotate._m00 = (in_fX * in_fX) * (1 - cosf(in_fRadians)) + cosf(in_fRadians);
    out_mRotate._m01 = ((1 - cosf(in_fRadians)) * in_fX * in_fY) - (sinf(in_fRadians) * in_fZ);
    out_mRotate._m02 = ((1 - cosf(in_fRadians)) * in_fX * in_fZ) + (sinf(in_fRadians) * in_fY);
    out_mRotate._m03 = 0.0f;

    out_mRotate._m10 = ((1 - cosf(in_fRadians)) * in_fX * in_fY) + (sinf(in_fRadians) * in_fZ);
    out_mRotate._m11 = (in_fY * in_fY) * (1 - cosf(in_fRadians)) + cosf(in_fRadians);
    out_mRotate._m12 = ((1 - cosf(in_fRadians)) * in_fY * in_fZ) - (sinf(in_fRadians) * in_fX);
    out_mRotate._m13 = 0.0f;

    out_mRotate._m20 = ((1 - cosf(in_fRadians)) * in_fX * in_fZ) - (sinf(in_fRadians) * in_fY);
    out_mRotate._m21 = ((1 - cosf(in_fRadians)) * in_fY * in_fZ) + (sinf(in_fRadians) * in_fX);
    out_mRotate._m22 = (in_fZ * in_fZ) * (1 - cosf(in_fRadians)) + cosf(in_fRadians);
    out_mRotate._m23 = 0.0f;

    out_mRotate.m_v4rtVector = Vector();

    return out_mRotate;
}

App::Math::Matrix App::Math::Matrix::RotationX(float in_fRadians)
{
    Matrix out_mRotate = mIdentity;
    out_mRotate._m11 = cosf(in_fRadians);
    out_mRotate._m21 = -sinf(in_fRadians);
    out_mRotate._m12 = sinf(in_fRadians);
    out_mRotate._m22 = cosf(in_fRadians);

    return out_mRotate;
}

App::Math::Matrix App::Math::Matrix::RotationY(float in_fRadians)
{
    Matrix out_mRotate = mIdentity;

    out_mRotate._m00 = cosf(in_fRadians);
    out_mRotate._m02 = -sinf(in_fRadians);
    out_mRotate._m20 = sinf(in_fRadians);
    out_mRotate._m22 = cosf(in_fRadians);

    return out_mRotate;
}

App::Math::Matrix App::Math::Matrix::RotationZ(float in_fRadians)
{
    Matrix out_mRotate = mIdentity;

    out_mRotate._m00 = cosf(in_fRadians);
    out_mRotate._m01 = sinf(in_fRadians);
    out_mRotate._m10 = -sinf(in_fRadians);
    out_mRotate._m11 = cosf(in_fRadians);

    return out_mRotate;
}


App::Math::Matrix App::Math::Matrix::Scale(Vector in_vNewDimentions)
{
    Matrix out_mScale = mIdentity;
    out_mScale._m00 = in_vNewDimentions[0U];
    out_mScale._m11 = in_vNewDimentions[1U];
    out_mScale._m22 = in_vNewDimentions[2U];

    return out_mScale;
}

App::Math::Matrix App::Math::Matrix::Translate(float in_fPosX, float in_fPosY, float in_fPosZ)
{
    Matrix out_mTranslate = Identity();
    out_mTranslate._m30 = in_fPosX;
    out_mTranslate._m31 = in_fPosY;
    out_mTranslate._m32 = in_fPosZ;
    return out_mTranslate;
}

App::Math::Matrix App::Math::operator *(App::Math::Matrix& in_mA, App::Math::Matrix& in_mB)
{
    App::Math::Matrix out_mMatrix;

    for (t_IntU8 nRow = 0; nRow < 4; nRow++)
    {
        for (t_IntU8 nColumn = 0; nColumn < 4; nColumn++)
        {
            for (t_IntU8 nColIt = 0; nColIt < 4; nColIt++)
            {
                out_mMatrix.m_vRowVectors[nRow][nColumn] +=
                    in_mA.m_arrMatrix[nRow][nColIt] * in_mB.m_arrMatrix[nColIt][nColumn];
            }
        }
    }

    return out_mMatrix;
}

App::Math::Vector App::Math::operator * (App::Math::Matrix& in_mMatrix, App::Math::Vector& in_vVector)
{
    App::Math::Vector out_vVector;

    in_vVector[vaxis::w] = 1.0f;

    for (t_IntU8 i = 0; i < 4; i++)
    {
        for (t_IntU8 j = 0; j < 4; j++)
        {
            out_vVector[i] += in_vVector[j] * in_mMatrix.m_arrMatrix[j][i];
        }
    }

    return out_vVector;
}

App::Math::Matrix App::Math::operator *(App::Math::Matrix& in_mMatrix, float in_fScalar)
{
    App::Math::Matrix out_mMatrix = in_mMatrix;

    for (t_IntU8 i = 0; i < 4; i++)
    {
        out_mMatrix.m_vRowVectors[i] *= in_fScalar;
    }

    return out_mMatrix;
}