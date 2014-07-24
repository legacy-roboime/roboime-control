#ifndef __ROBOIME_ACTION_H
#define __ROBOIME_ACTION_H

#include <vector>
#include "robot.h"
#include "pid_controller.h"


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
