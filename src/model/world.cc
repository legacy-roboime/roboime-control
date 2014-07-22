#include "world.h"

namespace roboime
{
    point::point() :
        x(0), y(0)
    {}

    point::point(float x, float y) :
        x(x), y(y)
    {}

    world::world() :
        width(4.),
        length(6.),
        line_width(.01),
        boundary_width(.25),
        referee_width(.425),
        center_radius(.5),
        defense_radius(.5),
        defense_stretch(.35),
        free_kick_distance(.7),
        penalty_spot_distance(.45),
        penalty_line_distance(.35),
        goal_width(.7),
        goal_depth(.18),
        goal_wall_width(.002)
    {
    }

    void
    world::update(const boost::property_tree::ptree& packet)
    {
        boost::shared_lock<boost::shared_mutex> lock(access_lock);

        auto balls = packet.get_child("detection.balls");
        for (auto it = balls.begin(), endIt = balls.end(); it != endIt; ++it)
        {
            ball.x = it->second.get<float>("x");
            ball.y = it->second.get<float>("y");
            std::cout << ball.x << " " << ball.y << std::endl;
        }

        // TODO
    }


}
