#include "Quaternion.h"

App::Math::Quaternion App::Math::Quaternion::SetRotation(Vector in_vAxis, float in_fRadians)
{
    App::Math::Quaternion out_qRotatedQuaternion;

    out_qRotatedQuaternion.GetQuaternion().W = cosf(in_fRadians * 0.5f);
    out_qRotatedQuaternion.GetQuaternion().X = in_vAxis.X * sinf(in_fRadians * 0.5f);
    out_qRotatedQuaternion.GetQuaternion().Y = in_vAxis.Y * sinf(in_fRadians * 0.5f);
    out_qRotatedQuaternion.GetQuaternion().Z = in_vAxis.Z * sinf(in_fRadians * 0.5f);

    return out_qRotatedQuaternion;
}

App::Math::Quaternion App::Math::Quaternion::Cojugated(Quaternion in_vQuaternion)
{
    Quaternion out_qConjugated = Quaternion(-in_vQuaternion.GetQuaternion().X, -in_vQuaternion.GetQuaternion().Y, -in_vQuaternion.GetQuaternion().Z);
    out_qConjugated.GetQuaternion().W = in_vQuaternion.GetQuaternion().W;

    return out_qConjugated;
}

App::Math::Quaternion App::Math::operator * (App::Math::Quaternion& in_qFirstQuaterion, App::Math::Quaternion& in_qSecondQuaternion)
{
    App::Math::Quaternion out_qQuaternion;

    out_qQuaternion.GetQuaternion().X = (in_qFirstQuaterion.GetQuaternion().W * in_qSecondQuaternion.GetQuaternion().X +
        in_qFirstQuaterion.GetQuaternion().X * in_qSecondQuaternion.GetQuaternion().W +
        in_qFirstQuaterion.GetQuaternion().Y * in_qSecondQuaternion.GetQuaternion().Z -
        in_qFirstQuaterion.GetQuaternion().Z * in_qSecondQuaternion.GetQuaternion().Y);

    out_qQuaternion.GetQuaternion().Y = (in_qFirstQuaterion.GetQuaternion().W * in_qSecondQuaternion.GetQuaternion().Y -
        in_qFirstQuaterion.GetQuaternion().X * in_qSecondQuaternion.GetQuaternion().Z +
        in_qFirstQuaterion.GetQuaternion().Y * in_qSecondQuaternion.GetQuaternion().W +
        in_qFirstQuaterion.GetQuaternion().Z * in_qSecondQuaternion.GetQuaternion().X);

    out_qQuaternion.GetQuaternion().Z = (in_qFirstQuaterion.GetQuaternion().W * in_qSecondQuaternion.GetQuaternion().Z +
        in_qFirstQuaterion.GetQuaternion().X * in_qSecondQuaternion.GetQuaternion().Y -
        in_qFirstQuaterion.GetQuaternion().Y * in_qSecondQuaternion.GetQuaternion().X +
        in_qFirstQuaterion.GetQuaternion().Z * in_qSecondQuaternion.GetQuaternion().W);

    out_qQuaternion.GetQuaternion().W = (in_qFirstQuaterion.GetQuaternion().W * in_qSecondQuaternion.GetQuaternion().W -
        in_qFirstQuaterion.GetQuaternion().X * in_qSecondQuaternion.GetQuaternion().X -
        in_qFirstQuaterion.GetQuaternion().Y * in_qSecondQuaternion.GetQuaternion().Y -
        in_qFirstQuaterion.GetQuaternion().Z * in_qSecondQuaternion.GetQuaternion().Z);

    return out_qQuaternion;
}

App::Math::Quaternion App::Math::operator + (App::Math::Quaternion& in_qFirstQuaterion, App::Math::Quaternion& in_qSecondQuaternion)
{
    App::Math::Quaternion out_qQuaternion;

    out_qQuaternion.GetQuaternion() = in_qFirstQuaterion.GetQuaternion() + in_qSecondQuaternion.GetQuaternion();

    return out_qQuaternion;
}
