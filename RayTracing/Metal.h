#pragma once

#include "material.h"

class Metal : public Material
{
public:
    Metal(const Color& albedo, double fuzz)
        : mAlbedo(albedo)
        , mFuzz(fuzz < 1 ? fuzz : 1)
    {
    }

    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override
    {
        Vec3 reflected = Reflect(rayIn.Direction(), hitRecord.normal);
        reflected = UnitVector(reflected) + (mFuzz * RandomUnitVector());
        scattered = Ray(hitRecord.point, reflected);
        attenuation = mAlbedo;
        return (Dot(scattered.Direction(), hitRecord.normal) > 0);
    }

private:
    Color mAlbedo;
    double mFuzz;
};