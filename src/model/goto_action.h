#ifndef __ROBOIME_GOTO_ACTION_H
#define __ROBOIME_GOTO_ACTION_H

#include <vector>
#include "move_action.h"
#include "pid_controller.h"

namespace roboime
{
    class goto_action : public action
    {
        private:
            point position;
            float angle;
            pid_controller dist_controller;
            pid_controller angle_controller;
        public: 
            goto_action(robot r, float x, float y, float o);
            virtual std::vector<char> as_buffer();
    };
}
#endif
