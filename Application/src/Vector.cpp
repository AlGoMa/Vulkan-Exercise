#include <Vector.h>

App::Math::Vector::Vector(float in_X, float in_Y, float in_Z, float in_W)
{
    X = in_X;
    Y = in_Y;
    Z = in_Z;
    W = in_W;
}

void App::Math::Vector::Normalize(Vector& out_vVector)
{
    float fDen = SqrMagnitude(out_vVector);

    float fVecMagnitude = fDen != 0.0f ? 1.0f / fDen : 0.0f;

    MulS(out_vVector, fVecMagnitude);
}

float App::Math::Vector::SqrMagnitude(Vector& in_vVector)
{
    return sqrt(Dot(in_vVector, in_vVector));
}

float App::Math::Vector::Magnitude(App::Math::Vector& in_vVector)
{
    return Dot(in_vVector, in_vVector);
}

float App::Math::Vector::Dot(const Vector& in_vFVector, const Vector& in_vSVector)
{
    return (in_vFVector.X * in_vSVector.X) + (in_vFVector.Y * in_vSVector.Y) + (in_vFVector.Z * in_vSVector.Z) + (in_vFVector.W * in_vSVector.W);
}

void App::Math::Vector::MulS(Vector& out_vVector, float in_fScalar)
{
    out_vVector.X *= in_fScalar;
    out_vVector.Y *= in_fScalar;
    out_vVector.Z *= in_fScalar;
    out_vVector.W *= in_fScalar;
}

App::Math::Vector App::Math::Vector::Cross(App::Math::Vector& in_vFirstVector, App::Math::Vector& in_vSecondVector)
{
    App::Math::Vector out_vVector;
    out_vVector.W = 0.0f;

    out_vVector.X = (in_vFirstVector.Y * in_vSecondVector.Z) - (in_vFirstVector.Z * in_vSecondVector.Y);
    out_vVector.Y = (in_vFirstVector.Z * in_vSecondVector.X) - (in_vFirstVector.X * in_vSecondVector.Z);
    out_vVector.Z = (in_vFirstVector.X * in_vSecondVector.Y) - (in_vFirstVector.Y * in_vSecondVector.X);

    return out_vVector;
}

float App::Math::Vector::Angle(Vector in_vFVector, Vector in_vSVector)
{
    Normalize(in_vFVector);
    Normalize(in_vSVector);

    return cosf(Dot(in_vFVector, in_vSVector));
}

App::Math::Vector App::Math::operator *(const App::Math::Vector& in_vFVector, const App::Math::Vector& in_vSVector)
{
    App::Math::Vector out_vVector;

    out_vVector.X = in_vFVector.X * in_vSVector.X;
    out_vVector.Y = in_vFVector.Y * in_vSVector.Y;
    out_vVector.Z = in_vFVector.Z * in_vSVector.Z;
    out_vVector.W = in_vFVector.W * in_vSVector.W;

    return out_vVector;
}

App::Math::Vector App::Math::operator +(const App::Math::Vector& in_vFVector, const App::Math::Vector& in_vSVector)
{
    App::Math::Vector out_vVector;

    out_vVector.X = in_vFVector.X + in_vSVector.X;
    out_vVector.Y = in_vFVector.Y + in_vSVector.Y;
    out_vVector.Z = in_vFVector.Z + in_vSVector.Z;
    out_vVector.W = in_vFVector.W + in_vSVector.W;

    return out_vVector;
}

App::Math::Vector App::Math::operator *(const App::Math::Vector& in_vVector, const float in_fScalar)
{
    App::Math::Vector out_vVector = in_vVector;

    App::Math::Vector::MulS(out_vVector, in_fScalar);

    return out_vVector;
}

App::Math::Vector App::Math::operator - (const App::Math::Vector& in_vFVector, const App::Math::Vector in_vSVector)
{
    return App::Math::Vector(in_vFVector.X - in_vSVector.X,
        in_vFVector.Y - in_vSVector.Y,
        in_vFVector.Z - in_vSVector.Z,
        in_vFVector.W - in_vSVector.W);
}

bool App::Math::operator == (const App::Math::Vector& in_vFVector, const App::Math::Vector in_vSVector)
{
    return in_vFVector.X == in_vSVector.X &&
           in_vFVector.Y == in_vSVector.Y &&
           in_vFVector.Z == in_vSVector.Z &&
           in_vFVector.W == in_vSVector.W;
}

