#include <cmath>
#include <functional>
#include <algorithm>

#include "world.h"

namespace roboime
{
    point::point() :
        x(0), y(0)
    {}

    point::point(float x, float y) :
        x(x), y(y)
    {}

    robot::robot(int id, float x, float y, float o) :
        position(x, y),
        angle(o),
        id(id),
        radius(0.0750),
        wheel_radius(0.0289),
        max_speed(54),
        angles{60. * M_PI / 180., 135. * M_PI / 180., -135. * M_PI / 180., -60. * M_PI / 180.}
    {}

    char 
    move_action::to_byte(float x)
    {
        if (abs(x) > r.max_speed)
            x = x / abs(x);
        else
            x = x / r.max_speed;

        return (char) (127 * x);
    }

    char 
    move_action::to_byte_kick(float x)
    {
        if (abs(x) > r.max_speed)
            x = 1;
        return (char) (127 * x);
    }

    std::vector<char>
    move_action::as_buffer()
    {
        if (std::isnan(vx) || std::isnan(vy) || std::isnan(va))
        {
            return std::vector<char>{(char) r.id, 0, 0, 0, 0, 0, 0};
        }
        
        std::vector<float> speeds = {
            (vy * cosf(r.angles[0]) - vx * sinf(r.angles[0]) + va * r.radius) / r.wheel_radius,
            (vy * cosf(r.angles[1]) - vx * sinf(r.angles[1]) + va * r.radius) / r.wheel_radius,
            (vy * cosf(r.angles[2]) - vx * sinf(r.angles[2]) + va * r.radius) / r.wheel_radius,
            (vy * cosf(r.angles[3]) - vx * sinf(r.angles[3]) + va * r.radius) / r.wheel_radius
        };
        auto largest = *std::max_element(speeds.begin(), speeds.end(), [](float x, float y) { return std::abs(x) < std::abs(y); });
        if (largest > r.max_speed && largest != 0)
            for (auto it = speeds.begin(), end_it = speeds.end(); it < end_it; ++it)
                *it = *it * r.max_speed / largest;

        return std::vector<char> {
            (char) r.id, 
            to_byte(speeds[0]), 
            to_byte(speeds[1]), 
            to_byte(speeds[2]), 
            to_byte(speeds[3]),
            to_byte(dribble? 255 : 0),
            to_byte(is_chip? to_byte_kick(kick) : to_byte_kick(-kick))
        };
    }

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
        boost::unique_lock<boost::shared_mutex> lock(access_lock);

        try
        {
            defense_radius = packet.get<float>("geometry.defense_radius");
            defense_stretch = packet.get<float>("geometry.defense_stretch");
            goal_width = packet.get<float>("geometry.goal_width");
        }
        catch (std::exception& e) { }
        
        try
        {
            auto balls = packet.get_child("detection.balls");
            for (auto it = balls.begin(), endIt = balls.end(); it != endIt; ++it)
            {
                ball.x = it->second.get<float>("x");
                ball.y = it->second.get<float>("y");
            }

            auto yellows = packet.get_child("detection.robots_yellow");
            team_yellow.clear();
            for (auto it = yellows.begin(), endIt = yellows.end(); it != endIt; ++it)
            {
                team_yellow.push_back(
                    robot( 
                        it->second.get<int>("robot_id"),
                        it->second.get<float>("x"),
                        it->second.get<float>("y"),
                        it->second.get<float>("orientation")
                    )
                );
            }
            auto blues = packet.get_child("detection.robots_blue");
            team_blue.clear();
            for (auto it = blues.begin(), endIt = blues.end(); it != endIt; ++it)
            {
                team_blue.push_back(
                    robot( 
                        it->second.get<int>("robot_id"),
                        it->second.get<float>("x"),
                        it->second.get<float>("y"),
                        it->second.get<float>("orientation")
                    )
                );
            }
        }
        catch (std::exception& e) {} 
    }
}
