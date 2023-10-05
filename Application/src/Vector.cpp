#include <Vector.h>
#include <Utilities.h>
#include <bit>
#include <limits>
#include <math.h>

__m128 __mm_select_aor_ps(const __m128 in_vecA, const __m128 in_vecB, const __m128 in_mask)
{
    return _mm_or_ps(_mm_and_ps(in_vecA, in_mask), _mm_andnot_ps(in_vecA, in_mask));
}

__m128 __mm_select_xor_ps(const __m128 in_vecA, const __m128 in_vecB, const __m128 in_mask)
{
    return _mm_xor_ps(in_vecA, _mm_and_ps(_mm_xor_ps(in_vecA, in_vecB), in_mask));
}

App::Math::Vector::Vector(const float in_X, const float in_Y, const float in_Z, const float in_W)
{
    X = in_X;
    Y = in_Y;
    Z = in_Z;
    W = in_W;
}

App::Math::Vector::Vector(const Vector& in_vVector, float in_W)
{
    X = in_vVector.x();
    Y = in_vVector.y();
    Z = in_vVector.z();
    W = in_W;
}

App::Math::Vector App::Math::Vector::Normalize(const Vector& in_vVector)
{
    float fDen = SqrMagnitude(in_vVector);

    float fVecMagnitude = fDen != 0.0f ? 1.0f / fDen : 0.0f;

    return const_cast<Vector&>(in_vVector) *= fVecMagnitude;
}

float App::Math::Vector::SqrMagnitude(const Vector& in_vVector)
{
    return rsqrt(Dot(in_vVector, in_vVector));
}

float App::Math::Vector::Magnitude(const Vector& in_vVector)
{
    return Dot(in_vVector, in_vVector);
}

float App::Math::Vector::Dot(const Vector& in_vA, const Vector& in_vB)
{
    float out_fResult = 0.0f;
    __m128 vRes = _mm_mul_ps(_mm_load_ps(in_vA.xyzw), _mm_load_ps(in_vB.xyzw));
    vRes = _mm_hadd_ps(vRes, vRes);
    vRes = _mm_hadd_ps(vRes, vRes);
    _mm_store_ss(&out_fResult, vRes);
    return out_fResult;
}

App::Math::Vector App::Math::Vector::MulS(const Vector& out_vVector, float in_fScalar)
{
    return static_cast<Vector>(out_vVector) *= in_fScalar;
}

App::Math::Vector App::Math::Vector::Cross(const App::Math::Vector& in_vA, const App::Math::Vector& in_vB)
{
    return Vector((in_vA.Y * in_vB.Z) - (in_vA.Z * in_vB.Y),
                  (in_vA.Z * in_vB.X) - (in_vA.X * in_vB.Z),
                  (in_vA.X * in_vB.Y) - (in_vA.Y * in_vB.X), 0.0f);
}

float App::Math::Vector::DotMagnitude(const Vector& in_vA, const Vector& in_vB)
{
    return cosf(Dot(Normalize(in_vA), Normalize(in_vB)));
}

float App::Math::Vector::CrossMagnitude(const Vector& in_vA, const Vector& in_vB)
{
    Vector out_vRes = Cross(in_vA, in_vB);

    return sinf(Dot(out_vRes, out_vRes));
}

App::Math::Vector  App::Math::Vector::Reflect(const Vector& in_vA, const Vector& in_vB)
{
    return static_cast<Vector>(in_vA).Reflect(in_vB);
}

App::Math::Vector App::Math::operator *(const Vector& in_vA, const Vector& in_vB)
{
    return static_cast<Vector>(in_vA) *= in_vB;
}

App::Math::Vector App::Math::operator +(const Vector& in_vA, const Vector& in_vB)
{
    return static_cast<Vector>(in_vA) += in_vB;
}

App::Math::Vector App::Math::operator /(const Vector& in_vA, const Vector& in_vB)
{
    return static_cast<Vector>(in_vA) /= in_vB;
}

bool App::Math::operator == (const Vector& in_vA, const Vector& in_vB)
{
    return memcmp(in_vA.xyzw, in_vB.xyzw, 4) == 0;
}

App::Math::Vector App::Math::operator - (const Vector& in_vA, const Vector& in_vB)
{
    return static_cast<Vector>(in_vA) -= in_vB;
}

App::Math::Vector App::Math::operator *(const Vector& in_vA, const float in_fScalar)
{
    return static_cast<Vector>(in_vA) *= in_fScalar;
}

App::Math::Vector App::Math::operator +(const Vector& in_vA, const float in_fScalar)
{
    return static_cast<Vector>(in_vA) += in_fScalar;
}

App::Math::Vector App::Math::operator -(const Vector& in_vA, const float in_fScalar)
{
    return static_cast<Vector>(in_vA) -= in_fScalar;
}

App::Math::Vector App::Math::operator / (const Vector& in_vA, const float in_fScalar)
{
    return static_cast<Vector>(in_vA) /= in_fScalar;
}

App::Math::Vector App::Math::operator *(const float in_fScalar, const Vector& in_vA)
{
    return static_cast<Vector>(in_vA) *= in_fScalar;
}

App::Math::Vector App::Math::operator +(const float in_fScalar, const Vector& in_vA)
{
    return static_cast<Vector>(in_vA) += in_fScalar;
}

App::Math::Vector App::Math::operator -(const float in_fScalar, const Vector& in_vA)
{
    return static_cast<Vector>(in_vA) -= in_fScalar;
}

App::Math::Vector App::Math::operator / (const float in_fScalar, const Vector& in_vA)
{
    return static_cast<Vector>(in_vA) /= in_fScalar;
}

/* Non-static functions. */
float App::Math::Vector::Magnitude(void)
{
    return Magnitude(*this);
}

App::Math::Vector App::Math::Vector::Normalize(void)
{
    return Normalize(*this);
}

App::Math::Vector App::Math::Vector::Cross(const Vector& in_vB)
{
    return Cross(*this, in_vB);
}

float App::Math::Vector::Dot(const Vector& in_vB)
{
    return Dot(*this, in_vB);
}

float App::Math::Vector::DotMagnitude(const Vector& in_vB)
{
    return DotMagnitude(*this, in_vB);
}

float App::Math::Vector::CrossMagnitude(const Vector& in_vB)
{
    return CrossMagnitude(*this, in_vB);
}

float App::Math::Vector::SqrMagnitude(void)
{
    return SqrMagnitude(*this);
}

App::Math::Vector App::Math::Vector::Reflect(const Vector& in_vB)
{
    return *this - 2.0f * Dot(*this, in_vB) * in_vB;
}