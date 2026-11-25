#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>
#include "rtweekend.h"

class Interval
{
public:
    Interval()
        : mMinimum(+Infinity), mMaximum(-Infinity)
    {
    }

    Interval(double minimum, double maximum)
        : mMinimum(minimum), mMaximum(maximum)
    {
    }

    double Min() const
    {
        return mMinimum;
    }

    double Max() const
    {
        return mMaximum;
    }

    double Size() const
    {
        return mMaximum - mMinimum;
    }

    bool Contains(double value) const
    {
        return mMinimum <= value && value <= mMaximum;
    }

    bool Surrounds(double value) const
    {
        return mMinimum < value && value < mMaximum;
    }

    double Clamp(double value) const
    {
        if (value < mMinimum)
        {
            return mMinimum;
        }

        if (value > mMaximum)
        {
            return mMaximum;
        }

        return value;
    }

    static const Interval Empty;
    static const Interval Universe;

private:
    double mMinimum = 0.0;
    double mMaximum = 0.0;
};

const Interval Interval::Empty(+Infinity, -Infinity);
const Interval Interval::Universe(-Infinity, +Infinity);

#endif