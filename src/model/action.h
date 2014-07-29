#ifndef __ROBOIME_ACTION_H
#define __ROBOIME_ACTION_H

#include <vector>
#include "robot.h"

namespace roboime
{
    class action
    {
        public:
            robot r;
            inline void update_robot(const robot& r_up) { r.position = r_up.position; r.angle = r_up.angle; }
            virtual std::vector<char> as_buffer() = 0;
            action(robot r) : r(r) {}
    };
}
#endif
