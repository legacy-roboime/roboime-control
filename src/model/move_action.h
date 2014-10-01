#ifndef __ROBOIME_MOVE_ACTION_H
#define __ROBOIME_MOVE_ACTION_H

#include <vector>
#include "action.h"
#include "robot.h"

namespace roboime
{
    class move_action : public action
    {
        private:
            float vx;
            float vy;
            float va;

            bool dribble;
            bool is_chip;
            float kick;

            char to_byte(float);
            char to_byte_kick(float);

        public:
            move_action(robot r,
                    float vx,
                    float vy,
                    float va,
                    float kick=0,
                    bool is_chip=false,
                    bool dribble=false);

            virtual std::vector<char> as_buffer();
    };
}
#endif
