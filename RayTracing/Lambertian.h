#pragma once

#include "material.h"

class Lambertian : public Material
{
public:
    explicit Lambertian(const Color& albedo)
        : mAlbedo(albedo)
    {
    }

    bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override
    {
        auto scatterDirection = hitRecord.normal + RandomUnitVector();

        // Catch degenerate scatter direction
        if (scatterDirection.NearZero())
        {
            scatterDirection = hitRecord.normal;
        }

        scattered = Ray(hitRecord.point, scatterDirection);
        attenuation = mAlbedo;

        return true;
    }

private:
    Color mAlbedo;
};
