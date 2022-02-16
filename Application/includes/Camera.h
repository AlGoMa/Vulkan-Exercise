#include <Resource.h>
#include <Vector.h>

namespace App::Math
{
    class Camera final : public Common::Resources::IResource<Camera>
    {
    public:
        Camera(void);
        Camera(Vector in_vPosition, Vector in_vLookAt, Vector in_vUp, float in_fFOV, float in_fAspectRation, float in_fNear, float in_fFar);
        ~Camera(void) = default;
    

    private:
        Vector m_vUp;
        Vector m_vPosition;
        Vector m_vLookAt;



    };
}