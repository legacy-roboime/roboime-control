#ifndef __ROBOIME_ROBOT_H
#define __ROBOIME_ROBOT_H

#include <cmath>

namespace roboime
{
    struct point
    {
        float x;
        float y;

        point();
        point(float x, float y);

        float norm();
    };
    point operator-(point p1, point p2);

    struct robot
    {
        int id;
        bool on_field;

        point position;
        float angle;

        float radius;
        float wheel_radius;
        float max_speed;

        static constexpr float angles[4]{60. * M_PI / 180., 135. * M_PI / 180., -135. * M_PI / 180., -60. * M_PI / 180.};

        robot(int id, float x, float y, float o);
    };
}
#endif
