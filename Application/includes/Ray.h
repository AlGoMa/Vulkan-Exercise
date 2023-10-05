#pragma once
#include <Vector.h>


namespace App::Math::RayTraicing
{
    class Ray : 
        public Common::Resources::IResource<Ray>
    {
    public:
        Ray(Vector in_vOrigin, Vector in_vDirection) : 
            m_vPosition(in_vOrigin), m_vDirection(in_vDirection) {}
        virtual ~Ray(void) {}

        inline const Vector&        Origin       (void) const        { return m_vPosition; }
        inline const Vector&        Direction    (void) const        { return m_vDirection; }
        __declspec(noinline) Vector PointAtParam (const float in_fT) { return m_vPosition + (m_vDirection * in_fT); }

    private:
        Vector m_vPosition;
        Vector m_vDirection;
    };

    template <typename TObject>
    requires (std::is_class<TObject>::value)
    class Hittable :
        public Common::Resources::IResource<TObject>
    {
    public:
        Hittable(void) {}
        virtual ~Hittable(void) {};

        typedef struct HITRECORD
        {
            Vector  m_vPoint;
            Vector  m_vNormal;
            float   m_fDelta;
            t_IntU8 m_isHitted;
        } stHitRecord;

    protected:
        virtual const stHitRecord Hit(const Ray& in_rRay, const float in_fMin, const float in_fMax) const = 0;
    };

}