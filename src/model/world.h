#ifndef __ROBOIME_WORLD_H
#define __ROBOIME_WORLD_H

#include <vector>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "action.h"

namespace roboime
{
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
        std::vector<std::shared_ptr<action>> actions_yellow;
        std::vector<std::shared_ptr<action>> actions_blue;

        std::vector<robot> team_yellow;
        std::vector<robot> team_blue;

        world();

        void update(const boost::property_tree::ptree& update);
    };
}
#endif
