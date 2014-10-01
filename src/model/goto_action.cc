#include <goto_action.h>
#include <iostream>

namespace roboime
{
    goto_action::goto_action(robot r, float x, float y, float o) :
        action(r),
        position(x, y),
        angle(o),
        dist_controller(.1, .01, .5, 50., 1.2),
        angle_controller(1., 0., 0., .5, 360, true)
    {}

    std::vector<char>
    goto_action::as_buffer()
    {
        auto delta = position - r.position;
        dist_controller.input = delta.norm();
        dist_controller.feedback = 0;

        angle_controller.input = angle;
        angle_controller.feedback = r.angle;

        if (delta.norm() > 0)
        {
            move_action a(r,
                dist_controller.output * delta.x / delta.norm(),
                dist_controller.output * delta.y / delta.norm(),
                angle_controller.output
            );
            return a.as_buffer();
        }
        else
        {
            move_action a(r,
                0,
                0,
                angle_controller.output
            );
            return a.as_buffer();
        }
    }
}
