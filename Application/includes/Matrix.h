#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <Resource.h>

namespace App::Math {

    class Matrix final 
    {
    public:
        enum class RotateAxis : uint8_t { X_AXIS, Y_AXIS, Z_AXIS };

        union
        {
            struct
            {
                float _m00, _m01, _m02, _m03,
                      _m10, _m11, _m12, _m13,
                      _m20, _m21, _m22, _m23,
                      _m30, _m31, _m32, _m33;
            };
            struct
            {
                Vector  m_v1stVector,
                    m_v2ndVector,
                    m_v3rdVector,
                    m_v4rtVector;
            };
            struct
            {
                Vector  m_vRowVectors[4];
            };
            struct
            {
                float   m_arrMatrix[4][4];
            };
        };

    public:
        Matrix(void) = default;
        ~Matrix(void) = default;

        unsigned int  Determinant(void);

        static void   Identity           (Matrix& out_mMatrix);
        static void   Transpose          (Matrix& out_mMatrix);
        static float  Trace              (Matrix& in_mMatrix);
        static void   FrustrumPlanes     (Matrix& in_mProjMatrix, Matrix& in_mViewMatrix, Vector* out_pPlanes);
        static Matrix Rotate             (Vector in_vAxis, float in_fRadians);
        static Matrix Rotate             (float in_fX, float in_fY, float in_fZ, float in_fRadians);
        static Matrix Rotate             (Quaternion in_qRotationQ);
        static Matrix RotationX          (float in_fRadians);
        static Matrix RotationY          (float in_fRadians);
        static Matrix RotationZ          (float in_fRadians);
        static Matrix Scale              (Vector in_vNewDimentions);
        static Matrix Translate          (float in_fPosX, float in_fPosY, float in_fPosZ);
        static Matrix PerspectiveMatrix  (float in_fFOV, float in_fAspectRatio, float in_fNear, float in_fFar);
        static Matrix OrtthographicMatrix(float in_fLPlane, float in_fRPlane, float in_fTopPlane, float in_fBottomPlane, float in_fNear, float in_fFar);
        static Matrix LookAtMatrix       (Vector in_vPosition, Vector in_vTarget, Vector in_vUpVector);
        friend Matrix operator * (Matrix& in_mFMatrix, Matrix& in_SMatrix);
        friend Vector operator * (Matrix& in_mMatrix, Vector& in_vVector);
        friend Matrix operator * (Matrix& in_mMatrix, float in_fScalar);
    };
}