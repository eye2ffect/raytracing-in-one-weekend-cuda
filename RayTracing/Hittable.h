#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include "Ray.h"
#include "interval.h"

class Material;

class HitRecord
{
public:
    Point3 point;
    Vec3 normal;
    std::shared_ptr<Material> material;
    double t = 0.0;
    bool isFrontFace = false;

    void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
    {
        isFrontFace = Dot(ray.Direction(), outwardNormal) < 0.0;
        normal = isFrontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool Hit(
        const Ray& ray,
        const Interval& rayT,
        HitRecord& hitRecord
    ) const = 0;
};

#endif