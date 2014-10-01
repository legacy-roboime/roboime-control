#include "kickto_action.h"

namespace roboime
{
    kickto_action::kickto_action(robot r, float tx, float ty, bool force_kick) :
        roboime::action(r),
        target_position(tx, ty),
        force_kick(force_kick),
        dist_controller(.1, .5, .005, 50., 0.8),
        angle_controller(.1, 0.5, .005, .5, 360, true)
    {}
}
