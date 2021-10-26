#pragma once
#include <Vector.h>
#include <Resource.h>

namespace App::Math {

    class Quaternion : 
        public App::Resources::IResource<Quaternion>
    {
    public:
        Quaternion(Vector in_vValue) : m_vAxis(in_vValue) {};
        Quaternion(float in_fX, float in_fY, float in_fZ) : m_vAxis(Vector(in_fX, in_fY, in_fZ, 1.0f)) {};
        Quaternion(void) : m_vAxis(Vector(0.0f, 0.0f, 0.0f, 1.0f)) {};
        ~Quaternion(void) = default;

        static Quaternion SetRotation(Vector in_vAxis, float in_fRadians);
        static Quaternion Cojugated(Quaternion in_vQuaternion);

        friend Quaternion operator * (Quaternion& in_qFirstQuaterion, Quaternion& in_qSecondQuaternion);
        friend Quaternion operator + (Quaternion& in_qFirstQuaterion, Quaternion& in_qSecondQuaternion);
        
        inline Vector&    GetQuaternion(void) { return m_vAxis; }
    
    private:
        Vector m_vAxis;
    };
}