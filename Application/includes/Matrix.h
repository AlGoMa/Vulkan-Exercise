#pragma once
#include <Vector.h>
#include <Resource.h>
#include <array>

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
        Matrix (const Vector in_vA, const Vector in_vB, const Vector in_vC, const Vector in_vD);
        Matrix (const bool in_bIdentity);
        Matrix (void);
        ~Matrix(void) = default;

        typedef struct { t_IntU8 mX, mY; } point;

        inline float&       operator [](const t_IntU8 in_unXY) noexcept       { return m_arrMatrix[in_unXY / 4][in_unXY % 4]; }
        inline float&       operator [](const point   in_sXY)  noexcept       { return m_arrMatrix[in_sXY.mY][in_sXY.mX]; }
        inline const float& operator [](const t_IntU8 in_unXY) const noexcept { return m_arrMatrix[in_unXY / 4][in_unXY % 4]; }
        inline const float& operator [](const point   in_sXY)  const noexcept { return m_arrMatrix[in_sXY.mY][in_sXY.mX]; }
        
        unsigned      int    Determinant (void);
        inline        Matrix Transpose   (void) { return Transpose(*this); }
        inline        float  Trace       (void) const { return m_v1stVector.x() + m_v2ndVector.y() + m_v3rdVector.z() + m_v4rtVector.w(); }
        
        inline static Matrix Identity    (void) { return Matrix(Vector::Right(), Vector::Up(), Vector::Forward(), Vector(Vector(0.0f), 1.0f)); }
        static Matrix Transpose          (Matrix& out_mMatrix);
        static float  Trace              (Matrix& in_mMatrix);
        static void   FrustrumPlanes     (Matrix& in_mProjMatrix, Matrix& in_mViewMatrix, Vector* out_pPlanes);
        static Matrix Rotate             (Vector in_vAxis, float in_fRadians);
        static Matrix Rotate             (float in_fX, float in_fY, float in_fZ, float in_fRadians);
        static Matrix RotationX          (float in_fRadians);
        static Matrix RotationY          (float in_fRadians);
        static Matrix RotationZ          (float in_fRadians);
        static Matrix Scale              (Vector in_vNewDimentions);
        static Matrix Translate          (float in_fPosX, float in_fPosY, float in_fPosZ);

        /* Camera stuff. */
        static Matrix PerspectiveMatrix  (const float in_fFOV, const float in_fAspectRatio, const float in_fNear, const float in_fFar);
        static Matrix OrtthographicMatrix(float in_fLPlane, float in_fRPlane, float in_fTopPlane, float in_fBottomPlane, float in_fNear, float in_fFar);
        static Matrix LookAtMatrix       (Vector in_vPosition, Vector in_vTarget, Vector in_vUpVector);

        __declspec(noinline) const Vector& Vec1(void) const { return m_v1stVector; }
        __declspec(noinline) const Vector& Vec2(void) const { return m_v2ndVector; }
        __declspec(noinline) const Vector& Vec3(void) const { return m_v3rdVector; }
        __declspec(noinline) const Vector& Vec4(void) const { return m_v4rtVector; }

        friend Matrix operator *(Matrix& in_mA, Matrix& in_mB);
        friend Vector operator *(Matrix& in_mMatrix, Vector& in_vVector);
        friend Matrix operator *(Matrix& in_mMatrix, float in_fScalar);

        Matrix operator *=(Matrix& in_mB)      { return *this * in_mB; }
        Vector operator *=(Vector& in_vVector) { return *this * in_vVector; }
        Matrix operator *=(float in_fScalar)   { return *this * in_fScalar; }

        static const  Matrix mIdentity;
    };
}