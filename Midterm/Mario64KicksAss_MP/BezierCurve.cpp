#include "BezierCurve.h"

#include <stdexcept>
#include <iostream>

BezierCurve::BezierCurve(const std::vector<Point>& points)
{
    if (points.size() < 4 || (points.size() - 4) % 3)
    {
        throw std::invalid_argument("Invalid number of points. Should be 4 + 3n");
    }

    controlPoints = points;
    curveCount = ((controlPoints.size() - 4) / 3) + 1;

}

Point BezierCurve::evaluateAt(float t)
{
    if (t < 0) t = 0;
    if (t > 1) t = 1;

    int curve = t * curveCount;
    int i = 3 * curve;

    // Convert t from the global parameter space to the one local to the curve.
    t -= (float)curve / curveCount;
    t *= curveCount;

    return (1 - t) * (1 - t) * (1 - t) * controlPoints[i] +
           3 * (1 - t) * (1 - t) * t * controlPoints[i + 1] +
           3 * (1 - t) * t * t * controlPoints[i + 2] +
           t * t * t * controlPoints[i + 3];
}
