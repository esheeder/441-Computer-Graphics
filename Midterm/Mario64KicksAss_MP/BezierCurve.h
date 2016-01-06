#pragma once

#include "Point.h"

#include <vector>

/**
 * A class representation of a cubic bezier curve
 */
class BezierCurve
{
public:

    /**
     * Empty BezierCurve constructor
     */
    BezierCurve() {}

    /**
     * Construct a BezierCurve from the given control points.
     *
     * @param points A vector of control points. Must be of size
     *     4 + 3n, where n is any integer.
     */
    BezierCurve(const std::vector<Point>& points);

    /**
     * Evaluate the bezier curve at the given t. t will be clamped
     * to the range [0, 1]
     */
    Point evaluateAt(float t);

private:
    std::vector<Point> controlPoints;
    int curveCount;
};
