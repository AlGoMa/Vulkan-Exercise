#pragma once
#include <Vector.h>
#include <Matrix.h>
#include <Resource.h>

namespace App::Math {

    using complex = Vector::axis;

    class Quaternion final
    {
    public:
        Quaternion (Vector in_vValue)                                          : m_vAxis(in_vValue) {};
        Quaternion (float in_fX, float in_fY, float in_fZ, float in_fS = 1.0f) : m_vAxis(Vector(in_fX, in_fY, in_fZ, in_fS)) {};
        Quaternion (void)                                                      : m_vAxis(Vector(0.0f, 0.0f, 0.0f)) {};
        ~Quaternion(void) = default;

        static Vector     SetRotation       (const Vector& in_vAxis, const float in_fRadians);
        static Matrix     Quaternion2Matrix (Quaternion& in_qRotation);
        static Quaternion Matrix2Quaternion (Matrix& in_mMatrix);
        static Quaternion Slerp             (const Quaternion& in_qSource, const Quaternion& in_qDest, const float in_fAmount);
        static Quaternion Lerp              (const Quaternion& in_qSource, const Quaternion& in_qDest, const float in_fAmount);

        Vector            SetRotation       (const Vector& in_vAxis);
        Matrix            Quaternion2Matrix (void);
        Quaternion        Slerp             (const Quaternion& in_qDest, const float in_fAmount);
        Quaternion        Lerp              (const Quaternion& in_qDest, const float in_fAmount);

        friend Quaternion   operator *(const Quaternion& in_qFirstQuaterion, const Quaternion& in_qSecondQuaternion);
        friend Quaternion   operator +(const Quaternion& in_qFirstQuaterion, const Quaternion& in_qSecondQuaternion);
        template<Indexable type>
        inline float&       operator [](const type in_tIndex) noexcept { return m_vAxis[static_cast<t_IntU8>(in_tIndex)]; }
        template<Indexable type>
        inline const float& operator [](const type in_tIndex) const noexcept { return const_cast<Vector&>(m_vAxis)[static_cast<t_IntU8>(in_tIndex)]; }

        inline Quaternion   Cojugated     (void)       { return Quaternion(-m_vAxis[0U], -m_vAxis[1U], -m_vAxis[2U], m_vAxis[3U]); }
        inline Quaternion   Inverse       (void)       { return Quaternion(Cojugated().m_vAxis / Norm()); }
        const  float        Norm          (void) const { return Vector::SqrMagnitude(m_vAxis); }
        inline const Vector GetVectorForm (void) const { return m_vAxis; }

        Quaternion operator *=(const Quaternion& in_qB) { 
            Vector vCross = Vector::Cross(m_vAxis, in_qB.m_vAxis);

            return Quaternion (vCross + Vector(in_qB[complex::i] * m_vAxis[complex::t] + in_qB[complex::t] * m_vAxis[complex::i],
                                               in_qB[complex::j] * m_vAxis[complex::t] + in_qB[complex::t] * m_vAxis[complex::j],
                                               in_qB[complex::k] * m_vAxis[complex::t] + in_qB[complex::t] * m_vAxis[complex::k],
                                               in_qB[complex::t] * m_vAxis[complex::t] - (Vector::Dot(Vector(m_vAxis, 0.0f), Vector(in_qB.m_vAxis, 0.0f)))));
        }
        inline Quaternion operator +=(Quaternion& in_qB) { return m_vAxis += in_qB.m_vAxis; }

    private:
        Vector m_vAxis;
    };
}