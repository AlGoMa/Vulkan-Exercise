#pragma once
#include <Resource.h>
#include <xmmintrin.h>
#include <intrin.h>
#include <Utilities.h>

namespace App::Math {

    class Vector final
    {
    public:
        Vector (float in_X, float in_Y, float in_Z, float in_W = 1.0f);
        Vector (float in_fSingle) : Vector(in_fSingle, in_fSingle, in_fSingle) {}
        Vector (const Vector& in_vVector, float in_W);
        Vector (void) : Vector(0.0f, 0.0f, 0.0f) {}
        ~Vector(void) = default;

        enum class axis    : t_IntU8 { x = 0U, y, z, w,
                                       r = x, g = y, b = z, a = w,
                                       i = x, j = y, k = z, t = w };

    private:
        union
        {
            struct { float X, Y, Z, W; };

            struct { float R, G, B, A; };

            struct { float I, J, K, T; };

            float a[4];
        };

    public:
        /* Static methods */
        static Vector  Normalize     (const Vector& out_vVector);
        static float   SqrMagnitude  (const Vector& in_vVector);
        static float   Magnitude     (const Vector& in_vVector);
        static float   Dot           (const Vector& in_vfVector, const Vector& in_vSVector);
        static Vector  MulS          (const Vector& out_vVector, float in_fScalar);
        static Vector  Cross         (const Vector& in_vA, const Vector& in_vB);
        static float   DotMagnitude  (const Vector& in_vA, const Vector& in_vB);
        static float   CrossMagnitude(const Vector& in_vA, const Vector& in_vB);

        Vector         Normalize     (void);
        Vector         Cross         (const Vector& in_vB);
        float          Dot           (const Vector& in_vB);
        float          DotMagnitude  (const Vector& in_vB);
        float          CrossMagnitude(const Vector& in_vB);
        float          SqrMagnitude  (void);

        /* Operators oveloading */
        Vector operator *=(const float in_fScalar) { _mm_store_ps(a, _mm_mul_ps(_mm_load_ps(a), _mm_set_ps1(in_fScalar))); return *this; }
        Vector operator +=(const float in_fScalar) { _mm_store_ps(a, _mm_add_ps(_mm_load_ps(a), _mm_set_ps1(in_fScalar))); return *this; }
        Vector operator -=(const float in_fScalar) { _mm_store_ps(a, _mm_sub_ps(_mm_load_ps(a), _mm_set_ps1(in_fScalar))); return *this; }
        Vector operator /=(const float in_fScalar) { _mm_store_ps(a, _mm_div_ps(_mm_load_ps(a), _mm_set_ps1(in_fScalar))); return *this; }
        Vector operator *=(const Vector& in_vB)    { _mm_store_ps(a, _mm_mul_ps(_mm_load_ps(a), _mm_load_ps(in_vB.a))); return *this; }
        Vector operator +=(const Vector& in_vB)    { _mm_store_ps(a, _mm_add_ps(_mm_load_ps(a), _mm_load_ps(in_vB.a))); return *this; }
        Vector operator -=(const Vector& in_vB)    { _mm_store_ps(a, _mm_sub_ps(_mm_load_ps(a), _mm_load_ps(in_vB.a))); return *this; }
        
        template<typename type>
        requires ((std::is_integral<type>::value && std::is_unsigned<type>::value) || std::is_enum<type>::value) && (sizeof(type) <= sizeof(t_IntU32))
        inline float& operator [](const type in_nIndex) noexcept { return a[static_cast<t_IntU8>(in_nIndex)]; }

        inline const float& x(void) const { return X; }
        inline const float& y(void) const { return Y; }
        inline const float& z(void) const { return Z; }
        inline const float& w(void) const { return W; }

        /*
        * Friend Overloads
        */
        friend Vector   operator * (const Vector& in_vA, const Vector& in_vB);
        friend Vector   operator + (const Vector& in_vA, const Vector& in_vB);
        friend Vector   operator - (const Vector& in_vA, const Vector& in_vB);
        friend bool     operator ==(const Vector& in_vA, const Vector& in_vB);
        friend Vector   operator * (const Vector& in_vA, const float   in_fScalar);
        friend Vector   operator / (const Vector& in_vA, const float   in_fScalar);
        friend Vector   operator + (const Vector& in_vA, const float   in_fScalar);
        friend Vector   operator - (const Vector& in_vA, const float   in_fScalar);
        
        /*
        * Predefinitions.
        */
        static inline Vector Zero     (void) { return Vector( 0.0f ); }
        static inline Vector Up       (void) { return Vector( 0.0f,  1.0f,  0.0f, 0.0f); }
        static inline Vector Down     (void) { return Vector( 0.0f, -1.0f,  0.0f, 0.0f); }
        static inline Vector Right    (void) { return Vector( 1.0f,  0.0f,  0.0f, 0.0f); }
        static inline Vector Left     (void) { return Vector(-1.0f,  0.0f,  0.0f, 0.0f); }
        static inline Vector Forward  (void) { return Vector( 0.0f,  0.0f,  1.0f, 0.0f); }
        static inline Vector Backward (void) { return Vector( 0.0f,  0.0f, -1.0f, 0.0f); }
    };
}
