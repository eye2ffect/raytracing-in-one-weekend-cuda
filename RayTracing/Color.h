#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include "Ray.h"
#include "interval.h"
#include <iostream>

using Color = Vec3;

inline double LinearToGamma(double linearComponent)
{
    if (linearComponent > 0.0)
    {
        return std::sqrt(linearComponent);
    }

    return 0.0;
}

inline void WriteColor(std::ostream& out, const Color& pixelColor)
{
    auto red = pixelColor.X();
    auto green = pixelColor.Y();
    auto blue = pixelColor.Z();

    // Apply a linear to gamma transform for gamma = 2.0
    red = LinearToGamma(red);
    green = LinearToGamma(green);
    blue = LinearToGamma(blue);

    // Translate the [0, 1] component values to the byte range [0, 255]
    static const Interval intensity(0.000, 0.999);

    int redByte = static_cast<int>(256.0 * intensity.Clamp(red));
    int greenByte = static_cast<int>(256.0 * intensity.Clamp(green));
    int blueByte = static_cast<int>(256.0 * intensity.Clamp(blue));

    // Write out the pixel color components
    out << redByte << ' ' << greenByte << ' ' << blueByte << '\n';
}

#endif