#pragma once
#include <math.h>
#include <Resource.h>

namespace App::Math {

    class Vector final
    {
    public:
        Vector(float in_X, float in_Y, float in_Z, float in_W = 1.0f);
        Vector(void) = default;
        ~Vector(void) = default;

        union
        {
            struct { float X, Y, Z, W; };

            struct { float R, G, B, A; };

            float arrfValues[4];
        };

        static void    Normalize   (Vector& out_vVector);
        static float   SqrMagnitude(Vector& in_vVector);
        static float   Magnitude   (Vector& in_vVector);
        static float   Dot         (const Vector& in_vfVector, const Vector& in_vSVector);
        static void    MulS        (Vector& out_vVector, float in_fScalar);
        static Vector  Cross       (Vector& in_vFirstVector, Vector& in_vSecondVector);
        static float   Angle       (Vector in_vFVector, Vector in_vSVector);

        /*
        * Overloads
        */
        friend Vector   operator *  (const Vector& in_vFVector, const Vector& in_vSVector);
        friend Vector   operator +  (const Vector& in_vFVector, const Vector& in_vSVector);
        friend Vector   operator *  (const Vector& in_vFVector, const float in_fScalar);
        friend Vector   operator -  (const Vector& in_vFVector, const Vector in_vSVector);
        friend bool     operator == (const Vector& in_vFVector, const Vector in_vSVector);
        /*
        * Predefinitions.
        */
        static Vector Up       (void) { return Vector(0.0f, 1.0f, 0.0f, 1.0f); };
        static Vector Down     (void) { return Vector(0.0f, -1.0f, 0.0f, 1.0f); };
        static Vector Right    (void) { return Vector(1.0f, 0.0f, 0.0f, 1.0f); };
        static Vector Left     (void) { return Vector(-1.0f, 0.0f, 0.0f, 1.0f); };
        static Vector Forward  (void) { return Vector(0.0f, 0.0f, 1.0f, 1.0f); };
        static Vector Backward (void) { return Vector(0.0f, 0.0f, -1.0f, 1.0f); };
    };
}
