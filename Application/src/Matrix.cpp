#include <Matrix.h>

unsigned int App::Math::Matrix::Determinant(void)
{
    return 0;
}

void App::Math::Matrix::Identity(Matrix& out_mMatrix)
{
    out_mMatrix.m_v1stVector = Vector(1.0f, 0.0f, 0.0f, 0.0f);
    out_mMatrix.m_v2ndVector = Vector(0.0f, 1.0f, 0.0f, 0.0f);
    out_mMatrix.m_v3rdVector = Vector(0.0f, 0.0f, 1.0f, 0.0f);
    out_mMatrix.m_v4rtVector = Vector(0.0f, 0.0f, 0.0f, 1.0f);
}

void App::Math::Matrix::Transpose(Matrix& out_mMatrix)
{
    Matrix mTempMatrix = out_mMatrix;

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            mTempMatrix.m_arrMatrix[i][j] = out_mMatrix.m_arrMatrix[j][i];

    out_mMatrix = mTempMatrix;
}

float App::Math::Matrix::Trace(Matrix& in_mMatrix)
{
    float out_fTraceMatrixRes = 0.0f;

    for (size_t i = 0; i < 4; i++)
    {
        out_fTraceMatrixRes += in_mMatrix.m_arrMatrix[i][i];
    }

    return out_fTraceMatrixRes;
}

void App::Math::Matrix::FrustrumPlanes(Matrix& in_mProjMatrix, Matrix& in_mViewMatrix, Vector* out_pPlanes)
{
    Matrix mVP = in_mViewMatrix * in_mProjMatrix;

    Vector out_vPlanes[6];

    //!< Near 
    out_vPlanes[0].X = -mVP._m02;
    out_vPlanes[0].Y = -mVP._m12;
    out_vPlanes[0].Z = -mVP._m22;
    out_vPlanes[0].W = -mVP._m32;

    //!< Far
    out_vPlanes[1].X = mVP._m02 - mVP._m03;
    out_vPlanes[1].Y = mVP._m12 - mVP._m13;
    out_vPlanes[1].Z = mVP._m22 - mVP._m23;
    out_vPlanes[1].W = mVP._m32 - mVP._m33;

    // Calculat e   left plane of frustum.
    out_vPlanes[2].X = -(mVP._m03 + mVP._m00);
    out_vPlanes[2].Y = -(mVP._m13 + mVP._m10);
    out_vPlanes[2].Z = -(mVP._m23 + mVP._m20);
    out_vPlanes[2].W = -(mVP._m33 + mVP._m30);

    // Calculat e   right plane of frustum.
    out_vPlanes[3].X = mVP._m00 - mVP._m03;
    out_vPlanes[3].Y = mVP._m10 - mVP._m13;
    out_vPlanes[3].Z = mVP._m20 - mVP._m23;
    out_vPlanes[3].W = mVP._m30 - mVP._m33;

    // Calculat e   top plane of frustum.
    out_vPlanes[4].X = mVP._m01 - mVP._m03;
    out_vPlanes[4].Y = mVP._m11 - mVP._m13;
    out_vPlanes[4].Z = mVP._m21 - mVP._m23;
    out_vPlanes[4].W = mVP._m31 - mVP._m33;

    // Calculat e   bottom plane of frustum.
    out_vPlanes[5].X = -(mVP._m03 + mVP._m01);
    out_vPlanes[5].Y = -(mVP._m13 + mVP._m11);
    out_vPlanes[5].Z = -(mVP._m23 + mVP._m21);
    out_vPlanes[5].W = -(mVP._m33 + mVP._m31);
}


App::Math::Matrix App::Math::Matrix::PerspectiveMatrix(float in_fFOV, float in_fAspectRatio, float in_fNear, float in_fFar)
{
    Matrix out_mProjectivMatrix;
    Identity(out_mProjectivMatrix);

    out_mProjectivMatrix._m11 = 1.0f / tanf(in_fFOV * 0.5f);
    out_mProjectivMatrix._m00 = 1.0f / tanf(in_fAspectRatio * 0.5f);
    out_mProjectivMatrix._m22 = in_fFar / (in_fFar - in_fNear);
    out_mProjectivMatrix._m32 = -(in_fNear * out_mProjectivMatrix._m22);
    out_mProjectivMatrix._m23 = 1.0f;

    return out_mProjectivMatrix;
}

App::Math::Matrix App::Math::Matrix::OrtthographicMatrix(float in_fLPlane, float in_fRPlane, float in_fTopPlane,
    float in_fBottomPlane, float in_fNear, float in_fFar)
{
    Matrix out_OrthogaphicMatrix;
    Identity(out_OrthogaphicMatrix);

    out_OrthogaphicMatrix._m00 = 2.0f / (in_fRPlane - in_fLPlane);
    out_OrthogaphicMatrix._m11 = 2.0f / (in_fTopPlane - in_fBottomPlane);
    out_OrthogaphicMatrix._m22 = 1.0f / (in_fFar - in_fNear);
    out_OrthogaphicMatrix._m32 = in_fNear / (in_fFar - in_fNear);
    out_OrthogaphicMatrix._m30 = (in_fLPlane + in_fRPlane) / (in_fLPlane - in_fRPlane);
    out_OrthogaphicMatrix._m31 = (in_fBottomPlane + in_fTopPlane) / (in_fBottomPlane - in_fTopPlane);

    return out_OrthogaphicMatrix;
}

App::Math::Matrix	App::Math::Matrix::LookAtMatrix(Vector in_vPosition, Vector in_vTarget, Vector in_vUpVector)
{
    Matrix out_mLookAtMatrix;

    Vector vRight, vForward, vUp;

    vForward = in_vPosition - in_vTarget;
    Vector::Normalize(vForward);

    in_vUpVector = Vector::Up();

    Vector::Normalize(in_vUpVector);

    vRight = Vector::Cross(in_vUpVector, vForward);
    
    vUp = Vector::Cross(vForward, vRight);
    /*!< Right column*/
    out_mLookAtMatrix._m00 = vRight.X;
    out_mLookAtMatrix._m10 = vRight.Y;
    out_mLookAtMatrix._m20 = vRight.Z;

    /*!< Up column*/
    out_mLookAtMatrix._m01 = vUp.X;
    out_mLookAtMatrix._m11 = vUp.Y;
    out_mLookAtMatrix._m21 = vUp.Z;

    /*!< Position column*/
    out_mLookAtMatrix._m02 = vForward.X;
    out_mLookAtMatrix._m12 = vForward.Y;
    out_mLookAtMatrix._m22 = vForward.Z;

    out_mLookAtMatrix.m_v4rtVector = Vector(-Vector::Dot(vRight, in_vPosition),
        -Vector::Dot(vUp, in_vPosition),
        -Vector::Dot(vForward, in_vPosition), 1.0f);

    return out_mLookAtMatrix;
}

App::Math::Matrix	App::Math::Matrix::Rotate(Vector in_vAxis, float in_fRadians)
{
    Matrix out_mRotate;

    out_mRotate._m00 = ((in_vAxis.X * in_vAxis.X) * (1 - cosf(in_fRadians))) + cosf(in_fRadians);					//1
    out_mRotate._m01 = ((1 - cosf(in_fRadians)) * (in_vAxis.X * in_vAxis.Y)) - (sinf(in_fRadians) * in_vAxis.Z); //1
    out_mRotate._m02 = ((1 - cosf(in_fRadians)) * (in_vAxis.X * in_vAxis.Z)) + (sin(in_fRadians) * in_vAxis.Y); //1
    out_mRotate._m03 = 0.0f;

    out_mRotate._m10 = ((1 - cosf(in_fRadians)) * (in_vAxis.X * in_vAxis.Y)) + (sinf(in_fRadians) * in_vAxis.Z); //1
    out_mRotate._m11 = ((in_vAxis.Y * in_vAxis.Y) * (1 - cosf(in_fRadians))) + cosf(in_fRadians);					//1
    out_mRotate._m12 = ((1 - cosf(in_fRadians)) * (in_vAxis.Y * in_vAxis.Z)) - (sinf(in_fRadians) * in_vAxis.X); //0
    out_mRotate._m13 = 0.0f;

    out_mRotate._m20 = ((1 - cosf(in_fRadians)) * (in_vAxis.X * in_vAxis.Z)) - (sinf(in_fRadians) * in_vAxis.Y); //0
    out_mRotate._m21 = ((1 - cosf(in_fRadians)) * (in_vAxis.Y * in_vAxis.Z)) + (sinf(in_fRadians) * in_vAxis.X); //0
    out_mRotate._m22 = ((in_vAxis.Z * in_vAxis.Z) * (1 - cosf(in_fRadians))) + cosf(in_fRadians);					//0
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

App::Math::Matrix App::Math::Matrix::Rotate(Quaternion in_qRotationQ)
{
    Matrix out_mRotationMatrix;

    out_mRotationMatrix._m00 = 1.0f - (2.0f * (in_qRotationQ.GetQuaternion().Y * in_qRotationQ.GetQuaternion().Y)) - (2.0f * (in_qRotationQ.GetQuaternion().Z * in_qRotationQ.GetQuaternion().Z));
    out_mRotationMatrix._m01 = (2.0f * (in_qRotationQ.GetQuaternion().X * in_qRotationQ.GetQuaternion().Y)) - (2.0f * (in_qRotationQ.GetQuaternion().W * in_qRotationQ.GetQuaternion().Z));
    out_mRotationMatrix._m02 = (2.0f * (in_qRotationQ.GetQuaternion().X * in_qRotationQ.GetQuaternion().Z)) + (2.0f * (in_qRotationQ.GetQuaternion().W * in_qRotationQ.GetQuaternion().Y));
    out_mRotationMatrix._m03 = 0.0f;

    out_mRotationMatrix._m10 = (2.0f * (in_qRotationQ.GetQuaternion().X * in_qRotationQ.GetQuaternion().Y)) + (2.0f * (in_qRotationQ.GetQuaternion().W * in_qRotationQ.GetQuaternion().Z));
    out_mRotationMatrix._m11 = 1.0f - (2.0f * (in_qRotationQ.GetQuaternion().X * in_qRotationQ.GetQuaternion().X)) - (2.0f * (in_qRotationQ.GetQuaternion().Z * in_qRotationQ.GetQuaternion().Z));
    out_mRotationMatrix._m12 = (2.0f * (in_qRotationQ.GetQuaternion().Y * in_qRotationQ.GetQuaternion().Z)) + (2.0f * (in_qRotationQ.GetQuaternion().W * in_qRotationQ.GetQuaternion().X));
    out_mRotationMatrix._m13 = 0.0f;

    out_mRotationMatrix._m20 = (2.0f * (in_qRotationQ.GetQuaternion().X * in_qRotationQ.GetQuaternion().Z)) - (2.0f * (in_qRotationQ.GetQuaternion().W * in_qRotationQ.GetQuaternion().Y));
    out_mRotationMatrix._m21 = (2.0f * (in_qRotationQ.GetQuaternion().Y * in_qRotationQ.GetQuaternion().Z)) - (2.0f * (in_qRotationQ.GetQuaternion().W * in_qRotationQ.GetQuaternion().X));
    out_mRotationMatrix._m22 = 1.0f - (2.0f * (in_qRotationQ.GetQuaternion().X * in_qRotationQ.GetQuaternion().X)) - (2.0f * (in_qRotationQ.GetQuaternion().Y * in_qRotationQ.GetQuaternion().Y));
    out_mRotationMatrix._m23 = 0.0f;

    out_mRotationMatrix.m_v4rtVector = Vector();

    return out_mRotationMatrix;
}

App::Math::Matrix App::Math::Matrix::RotationX(float in_fRadians)
{
    Matrix out_mRotate;
    Identity(out_mRotate);
    out_mRotate._m11 = cosf(in_fRadians);
    out_mRotate._m21 = -sinf(in_fRadians);
    out_mRotate._m12 = sinf(in_fRadians);
    out_mRotate._m22 = cosf(in_fRadians);

    return out_mRotate;
}

App::Math::Matrix App::Math::Matrix::RotationY(float in_fRadians)
{
    Matrix out_mRotate;
    Identity(out_mRotate);

    out_mRotate._m00 = cosf(in_fRadians);
    out_mRotate._m02 = -sinf(in_fRadians);
    out_mRotate._m20 = sinf(in_fRadians);
    out_mRotate._m22 = cosf(in_fRadians);

    return out_mRotate;
}

App::Math::Matrix App::Math::Matrix::RotationZ(float in_fRadians)
{
    Matrix out_mRotate;
    Identity(out_mRotate);

    out_mRotate._m00 = cosf(in_fRadians);
    out_mRotate._m01 = sinf(in_fRadians);
    out_mRotate._m20 = -sinf(in_fRadians);
    out_mRotate._m21 = cosf(in_fRadians);

    return out_mRotate;
}


App::Math::Matrix App::Math::Matrix::Scale(Vector in_vNewDimentions)
{
    Matrix out_mScale;
    Identity(out_mScale);
    out_mScale._m00 = in_vNewDimentions.X;
    out_mScale._m11 = in_vNewDimentions.Y;
    out_mScale._m22 = in_vNewDimentions.Z;

    return out_mScale;
}

App::Math::Matrix App::Math::Matrix::Translate(float in_fPosX, float in_fPosY, float in_fPosZ)
{
    Matrix out_mTranslate;
    Identity(out_mTranslate);
    out_mTranslate._m30 = in_fPosX;
    out_mTranslate._m31 = in_fPosY;
    out_mTranslate._m32 = in_fPosZ;
    return out_mTranslate;
}

App::Math::Matrix App::Math::operator * (App::Math::Matrix& in_mFMatrix, App::Math::Matrix& in_SMatrix)
{
    App::Math::Matrix out_mMatrix;

    for (size_t nRow = 0; nRow < 4; nRow++)
    {
        for (size_t nColumn = 0; nColumn < 4; nColumn++)
        {
            for (size_t nColIt = 0; nColIt < 4; nColIt++)
            {
                out_mMatrix.m_vRowVectors[nRow].arrfValues[nColumn] +=
                    in_mFMatrix.m_arrMatrix[nRow][nColIt] * in_SMatrix.m_arrMatrix[nColIt][nColumn];
            }
        }
    }

    return out_mMatrix;
}

App::Math::Vector App::Math::operator * (App::Math::Matrix& in_mMatrix, App::Math::Vector& in_vVector)
{
    App::Math::Vector out_vVector;

    in_vVector.W = 1.0f;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            out_vVector.arrfValues[i] += in_vVector.arrfValues[j] * in_mMatrix.m_arrMatrix[j][i];
        }
    }

    return out_vVector;
}

App::Math::Matrix App::Math::operator * (App::Math::Matrix& in_mMatrix, float in_fScalar)
{
    App::Math::Matrix out_mMatrix = in_mMatrix;

    for (size_t i = 0; i < 4; i++)
    {
        App::Math::Vector::MulS(out_mMatrix.m_vRowVectors[i], in_fScalar);
    }

    return out_mMatrix;
}