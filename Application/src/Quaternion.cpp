#include "Quaternion.h"

App::Math::Vector App::Math::Quaternion::SetRotation(const Vector& in_vAxis, const float in_fRadians)
{
    float vRot = sinf(in_fRadians * 0.5f);
    /* QVQ* or QVP-1 */
    return  Quaternion(vRot, vRot, vRot, cosf(in_fRadians * 0.5f)).SetRotation(in_vAxis);
}

App::Math::Vector App::Math::Quaternion::SetRotation(const Vector& in_vAxis)
{
    /* QVQ* or QVP-1 */
    return ((Quaternion(Vector(in_vAxis, 0.0f)) * (*this)) * Inverse()).GetVectorForm();
}

App::Math::Quaternion App::Math::operator *(const Quaternion& in_qA, const Quaternion& in_qB)
{
    return static_cast<Quaternion>(in_qA) *= in_qB;
}

App::Math::Quaternion App::Math::operator +(const Quaternion& in_qA, const Quaternion& in_qB)
{
    return Quaternion(in_qA.m_vAxis + in_qB.m_vAxis);
}

App::Math::Matrix App::Math::Quaternion::Quaternion2Matrix(void)
{
    Matrix out_mRotationMatrix;

    out_mRotationMatrix._m00 = 1.0f - (2.0f * (m_vAxis[complex::j] * m_vAxis[complex::j])) - (2.0f * (m_vAxis[complex::k] * m_vAxis[complex::k]));
    out_mRotationMatrix._m01 = (2.0f * (m_vAxis[complex::i] * m_vAxis[complex::j])) - (2.0f * (m_vAxis[complex::t] * m_vAxis[complex::k]));
    out_mRotationMatrix._m02 = (2.0f * (m_vAxis[complex::i] * m_vAxis[complex::k])) + (2.0f * (m_vAxis[complex::t] * m_vAxis[complex::j]));
    out_mRotationMatrix._m03 = 0.0f;

    out_mRotationMatrix._m10 = (2.0f * (m_vAxis[complex::i] * m_vAxis[complex::j])) + (2.0f * (m_vAxis[complex::t] * m_vAxis[complex::k]));
    out_mRotationMatrix._m11 = 1.0f - (2.0f * (m_vAxis[complex::i] * m_vAxis[complex::i])) - (2.0f * (m_vAxis[complex::k] * m_vAxis[complex::k]));
    out_mRotationMatrix._m12 = (2.0f * (m_vAxis[complex::j] * m_vAxis[complex::k])) + (2.0f * (m_vAxis[complex::t] * m_vAxis[complex::i]));
    out_mRotationMatrix._m13 = 0.0f;

    out_mRotationMatrix._m20 = (2.0f * (m_vAxis[complex::i] * m_vAxis[complex::k])) - (2.0f * (m_vAxis[complex::j] * m_vAxis[complex::j]));
    out_mRotationMatrix._m21 = (2.0f * (m_vAxis[complex::j] * m_vAxis[complex::k])) - (2.0f * (m_vAxis[complex::j] * m_vAxis[complex::i]));
    out_mRotationMatrix._m22 = 1.0f - (2.0f * (m_vAxis[complex::i] * m_vAxis[complex::i])) - (2.0f * (m_vAxis[complex::j] * m_vAxis[complex::j]));
    out_mRotationMatrix._m23 = 0.0f;

    out_mRotationMatrix.m_v4rtVector = Vector();

    return out_mRotationMatrix;
}

App::Math::Quaternion App::Math::Quaternion::Matrix2Quaternion(Matrix& in_mMatrix)
{
    Quaternion out_cQtn;
    float fTrace = in_mMatrix.Trace();

    if (fTrace > 0.0f)
    {
        float fAngle = sqrt(fTrace + 1.0f);
        float fAxis = 0.5f / fAngle;
        out_cQtn[complex::k] = (in_mMatrix[4] - in_mMatrix[1]) * fAxis;
        out_cQtn[complex::j] = (in_mMatrix[2] - in_mMatrix[8]) * fAxis;
        out_cQtn[complex::i] = (in_mMatrix[9] - in_mMatrix[6]) * fAxis;
        out_cQtn[complex::t] = fAngle * 0.5f;
    }
    else
    {
        t_IntU8 uI = in_mMatrix[{1, 1}] > in_mMatrix[{0, 0}] ? 1 : 0;
        uI = in_mMatrix[{2, 2}] > in_mMatrix[{uI, uI}] ? 2 : uI;
        static const std::array<t_IntU8, 3> aIndex{ 1U, 2U, 0U };
        t_IntU8 uJ = aIndex[uI];
        t_IntU8 uK = aIndex[uJ];
        
        float fAngle = sqrt(in_mMatrix[{uI, uJ}] + in_mMatrix[{uJ, uJ}] + in_mMatrix[{uK, uK}]);
        float fAxis  = safediv(0.5f, fAngle, fAngle);
        out_cQtn[complex::k]  = (in_mMatrix[{uK, uJ}] - in_mMatrix[{uJ, uK}]) * fAxis;
        out_cQtn[(complex)uJ] = (in_mMatrix[{uJ, uI}] - in_mMatrix[{uI, uJ}]) * fAxis;
        out_cQtn[(complex)uK] = (in_mMatrix[{uK, uI}] - in_mMatrix[{uI, uK}]) * fAxis;
    }

    return out_cQtn;
}

App::Math::Quaternion App::Math::Quaternion::Slerp(const Quaternion & in_qSource, const Quaternion & in_qDest, const float in_fAmount)
{
    return const_cast<Quaternion&>(in_qSource).Slerp(in_qDest, in_fAmount);
}

App::Math::Quaternion App::Math::Quaternion::Lerp(const Quaternion& in_qSource, const Quaternion& in_qDest, const float in_fAmount)
{
    return const_cast<Quaternion&>(in_qSource).Lerp(in_qDest, in_fAmount);
}

App::Math::Quaternion App::Math::Quaternion::Slerp(const Quaternion& in_qDest, const float in_fAmount)
{
    float fOmega = acos(m_vAxis.SqrMagnitude(in_qDest.GetVectorForm()));
    float fSOmega = sin(fOmega);
    float fWp = sin((1.0f - in_fAmount) * fOmega) / fSOmega;
    float fWq = sin(in_fAmount * fOmega) / fSOmega;

    return Quaternion(Vector(
        Vector(m_vAxis[complex::i],
               m_vAxis[complex::j],
               m_vAxis[complex::k],
               m_vAxis[complex::t]) * fWp + 
        Vector(in_qDest[complex::i],
               in_qDest[complex::j],
               in_qDest[complex::k],
               in_qDest[complex::t]) * fWq 
    ));
}

App::Math::Quaternion App::Math::Quaternion::Lerp(const Quaternion& in_qDest, const float in_fAmount)
{
    return Quaternion(Vector(
        Vector(m_vAxis[complex::i],
               m_vAxis[complex::j],
               m_vAxis[complex::k],
               m_vAxis[complex::t]) * (1.0f - in_fAmount) +
        Vector(in_qDest[complex::i],
               in_qDest[complex::j],
               in_qDest[complex::k],
               in_qDest[complex::t]) * in_fAmount
    ));
}

App::Math::Matrix App::Math::Quaternion::Quaternion2Matrix(Quaternion& in_qRotation)
{
    return in_qRotation.Quaternion2Matrix();
}