#ifndef __ROBOIME_KICKTO_ACTION_H
#define __ROBOIME_KICKTO_ACTION_H

#include <vector>
#include "move_action.h"
#include "pid_controller.h"

namespace roboime
{
    class kickto_action : public action
    {
        private:
            point target_position;
            bool force_kick;
            pid_controller dist_controller;
            pid_controller angle_controller;
        public:
            kickto_action(robot r, float tx, float ty, bool force_kick);
            virtual std::vector<char> as_buffer();
    };
}
#endif
