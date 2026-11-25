#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class Camera
{
public:
    double aspectRatio = 1.0;
    int imageWidth = 100;
    int samplesPerPixel = 10;
    int maxDepth = 10;
    double verticalFieldOfView = 90;
    Point3 lookFrom = Point3(0, 0, 0);
    Point3 lookAt = Point3(0, 0, -1);
    Vec3 viewUp = Vec3(0, 1, 0);

    double defocusAngle = 0;
    double focusDistance = 10;

    void Render(const Hittable& world)
    {
        Initialize();

        std::cout << "P3\n" << imageWidth << ' ' << mImageHeight << "\n255\n";

        for (int scanlineIndex = 0; scanlineIndex < mImageHeight; scanlineIndex++)
        {
            std::clog
                << "\rScanlines remaining: "
                << (mImageHeight - scanlineIndex)
                << ' '
                << std::flush;

            for (int pixelIndex = 0; pixelIndex < imageWidth; pixelIndex++)
            {
                Color pixelColor(0.0, 0.0, 0.0);

                for (int sampleIndex = 0; sampleIndex < samplesPerPixel; sampleIndex++)
                {
                    Ray ray = GetRay(pixelIndex, scanlineIndex);
                    pixelColor += RayColor(ray, maxDepth, world);
                }

                WriteColor(std::cout, mPixelSamplesScale * pixelColor);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    void Initialize()
    {
        mImageHeight = static_cast<int>(imageWidth / aspectRatio);
        mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;
        mPixelSamplesScale = 1.0 / static_cast<double>(samplesPerPixel);

        mCenter = lookFrom;

        // Determine viewport dimensions
        auto theta = DegreesToRadians(verticalFieldOfView);
        auto h = std::tan(theta / 2);
        auto viewportHeight = 2.0 * h * focusDistance;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / mImageHeight);

        // 카메라 좌표 프레임에 대한 u,v,w 단위 기저 벡터 계산
        w = UnitVector(lookFrom - lookAt);
        u = UnitVector(Cross(viewUp, w));
        v = Cross(w, u);

        // 뷰포트의 수평 및 수직 가장자리를 가로지르는 벡터 계산
        Vec3 viewportU = viewportWidth * u;    // 뷰포트 수평 가장자리를 가로지르는 벡터
        Vec3 viewportV = viewportHeight * -v;  // 뷰포트 수직 가장자리를 따라 내려가는 벡터

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        mPixelDeltaU = viewportU / imageWidth;
        mPixelDeltaV = viewportV / mImageHeight;

        // Calculate the location of the upper left pixel
        const Point3 viewportUpperLeft =
            mCenter
            - (focusDistance * w)
            - viewportU / 2.0
            - viewportV / 2.0;

        mPixel00Location = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);

        const double defocusRadius =
            focusDistance * std::tan(DegreesToRadians(defocusAngle * 0.5));

        mDefocusDiskU = u * defocusRadius;
        mDefocusDiskV = v * defocusRadius;
    }

    Vec3 RandomInUnitDisk() const
    {
        while (true)
        {
            Vec3 p(2.0 * RandomDouble() - 1.0, 2.0 * RandomDouble() - 1.0, 0.0);
            if (p.LengthSquared() < 1.0)
            {
                return p;
            }
        }
    }

    Ray GetRay(int pixelIndex, int scanlineIndex) const
    {
        auto offset = SampleSquare();

        auto pixelSample =
            mPixel00Location
            + ((pixelIndex + offset.X()) * mPixelDeltaU)
            + ((scanlineIndex + offset.Y()) * mPixelDeltaV);

        auto rayOrigin = (defocusAngle <= 0.0) ? mCenter : DefocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;

        return Ray(rayOrigin, rayDirection);
    }

    Vec3 SampleSquare() const
    {
        return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
    }

    Point3 DefocusDiskSample() const
    {
        const Vec3 point = RandomInUnitDisk();
        return mCenter + (point.X() * mDefocusDiskU) + (point.Y() * mDefocusDiskV);
    }

    Color RayColor(const Ray& ray, int depth, const Hittable& world) const
    {
        if (depth <= 0)
        {
            return Color(0.0, 0.0, 0.0);
        }

        HitRecord hitRecord;

        if (world.Hit(ray, Interval(0.001, Infinity), hitRecord))
        {
            Ray scattered;
            Color attenuation;

            if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered))
            {
                return attenuation * RayColor(scattered, depth - 1, world);
            }

            return Color(0.0, 0.0, 0.0);
        }

        Vec3 unitDirection = UnitVector(ray.Direction());
        auto a = 0.5 * (unitDirection.Y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0)
            + a * Color(0.5, 0.7, 1.0);
    }

private:
    int mImageHeight = 0;
    Point3 mCenter;
    Point3 mPixel00Location;
    Vec3 mPixelDeltaU;
    Vec3 mPixelDeltaV;
    Vec3 u, v, w;

    double mPixelSamplesScale = 1.0;

    Vec3 mDefocusDiskU;
    Vec3 mDefocusDiskV;
};

#endif