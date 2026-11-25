#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

inline double RandomDouble();
inline double RandomDouble(double minimum, double maximum);

class Vec3
{
public:
    Vec3() : mElements{ 0.0, 0.0, 0.0 } {}
    Vec3(double e0, double e1, double e2) : mElements{ e0, e1, e2 } {}

    double X() const { return mElements[0]; }
    double Y() const { return mElements[1]; }
    double Z() const { return mElements[2]; }

    Vec3 operator-() const { return Vec3(-mElements[0], -mElements[1], -mElements[2]); }
    double operator[](int i) const { return mElements[i]; }
    double& operator[](int i) { return mElements[i]; }

    Vec3& operator+=(const Vec3& v)
    {
        mElements[0] += v.mElements[0];
        mElements[1] += v.mElements[1];
        mElements[2] += v.mElements[2];
        return *this;
    }

    Vec3& operator*=(double t)
    {
        mElements[0] *= t;
        mElements[1] *= t;
        mElements[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t)
    {
        return *this *= 1.0 / t;
    }

    double Length() const
    {
        return std::sqrt(LengthSquared());
    }

    double LengthSquared() const
    {
        return mElements[0] * mElements[0]
            + mElements[1] * mElements[1]
            + mElements[2] * mElements[2];
    }

    static Vec3 Random()
    {
        return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
    }

    static Vec3 Random(double minimum, double maximum)
    {
        return Vec3(
            RandomDouble(minimum, maximum),
            RandomDouble(minimum, maximum),
            RandomDouble(minimum, maximum)
        );
    }

    bool NearZero() const
    {
        // Return true if the vector is close to zero in all dimensions
        auto threshold = 1e-8;

        return (std::fabs(mElements[0]) < threshold)
            && (std::fabs(mElements[1]) < threshold)
            && (std::fabs(mElements[2]) < threshold);
    }

private:
    double mElements[3] = {};
};

using Vector3 = Vec3;
using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.X() << ' ' << v.Y() << ' ' << v.Z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
    return Vec3(u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
    return Vec3(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
    return Vec3(u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z());
}

inline Vec3 operator*(double t, const Vec3& v)
{
    return Vec3(t * v.X(), t * v.Y(), t * v.Z());
}

inline Vec3 operator*(const Vec3& v, double t)
{
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
    return (1.0 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
    return u.X() * v.X()
        + u.Y() * v.Y()
        + u.Z() * v.Z();
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
    return Vec3(u.Y() * v.Z() - u.Z() * v.Y(),
        u.Z() * v.X() - u.X() * v.Z(),
        u.X() * v.Y() - u.Y() * v.X());
}

inline Vec3 UnitVector(const Vec3& v)
{
    return v / v.Length();
}

inline Vector3 RandomUnitVector()
{
    while (true)
    {
        auto p = Vec3::Random(-1.0, 1.0);
        auto lengthSquared = p.LengthSquared();

        if (1e-160 < lengthSquared && lengthSquared <= 1.0)
        {
            return p / std::sqrt(lengthSquared);
        }
    }
}

inline Vector3 RandomOnHemisphere(const Vector3& normal)
{
    Vector3 unitSphereDirection = RandomUnitVector();

    if (Dot(unitSphereDirection, normal) > 0.0)
    {
        return unitSphereDirection;
    }

    return -unitSphereDirection;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n)
{
    return v - 2.0 * Dot(v, n) * n;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etaiOverEtat)
{
    auto cosTheta = std::fmin(Dot(-uv, n), 1.0);
    Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * n;
    return rOutPerp + rOutParallel;
}

#endif