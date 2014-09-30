#include "robot.h"

#include <cmath>

namespace roboime
{
#ifndef _MSC_VER
    constexpr float robot::angles[4];
#else
    float robot::angles[4] = {60. * M_PI / 180., 135. * M_PI / 180., -135. * M_PI / 180., -60. * M_PI / 180.}
#endif

    point::point() :
        x(0), y(0)
    {}

    point::point(float x, float y) :
        x(x), y(y)
    {}

    point 
    operator-(point p1, point p2)
    {
        return point(p1.x - p2.x, p1.y - p2.y);
    }

    float
    point::norm(void)
    {
        return std::sqrt(x * x + y * y);
    }

    robot::robot(int id, float x, float y, float o) :
        position(x, y),
        angle(o),
        id(id),
        radius(7.521),
        wheel_radius(2.890),
        max_speed(54.)
    {}
}
