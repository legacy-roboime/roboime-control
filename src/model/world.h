#ifndef __ROBOIME_WORLD_H
#define __ROBOIME_WORLD_H

#include <vector>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace roboime
{
    struct point
    {
        float x;
        float y;

        point();
        point(float x, float y);
    };

    class action;

    struct robot
    {
        int id;
        point position;
        float angle;

        float radius;
        float wheel_radius;
        float max_speed;

        float angles[4];

        robot(int id, float x, float y, float o);
        std::shared_ptr<action> command;
    };


    class action
    {
        public:
            virtual std::vector<char> as_buffer() = 0;
    };


    class move_action : public action
    {
        private:
            robot r;
            float vx;
            float vy;
            float va;

            bool dribble;
            bool is_chip;
            float kick;

            char to_byte(float);
            char to_byte_kick(float);

        public:
            move_action(robot r, float vx, float vy, float va, float kick, bool is_chip, bool dribble);
            virtual std::vector<char> as_buffer();
    };


    struct world
    {
        float width;
        float length;
        float line_width;
        float boundary_width;
        float referee_width;
        float center_radius;
        float defense_radius;
        float defense_stretch;
        float free_kick_distance;
        float penalty_spot_distance;
        float penalty_line_distance;
        float goal_width;
        float goal_depth;
        float goal_wall_width;

        boost::shared_mutex access_lock;

        point ball;
        std::vector<robot> team_yellow;
        std::vector<robot> team_blue;

        world();

        void update(const boost::property_tree::ptree& update);
    };
}
#endif
