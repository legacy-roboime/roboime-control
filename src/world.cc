#include <boost/property_tree/json_parser.hpp>
#include <cmath>
#include <functional>
#include <algorithm>

#include "world.h"
#include "move_action.h"
#include "goto_action.h"
#include "config.h"

namespace roboime
{
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
        catch (std::exception& e) {}
        
        try
        {
            auto balls = packet.get_child("detection.balls");
            for (auto it = balls.begin(), endIt = balls.end(); it != endIt; ++it)
            {
                ball.x = it->second.get<float>("x");
                ball.y = it->second.get<float>("y");
            }
        }
        catch(std::exception& e) {}

        try
        {
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

        try
        {
            auto yellows = packet.get_child("control.actions_yellow");
            //boost::property_tree::json_parser::write_json(std::cout, yellows);
            if (yellows.begin() != yellows.end())
            {
                actions_yellow.clear();
                for (auto it = yellows.begin(), endIt = yellows.end(); it != endIt; ++it)
                {
                    auto r_id = it->second.get<int>("uid");
                    auto r_it = std::find_if(team_yellow.begin(), team_yellow.end(), [&r_id](const robot& r) { return r.id == r_id; });

                    if (r_it != team_yellow.end())
                    {
                        auto r = *r_it;
                        if (it->second.get<std::string>("type") == "move")
                        {
                            actions_yellow.push_back(
                                std::make_shared<move_action>(
                                    r,
                                    it->second.get<float>("speeds.vx"),
                                    it->second.get<float>("speeds.vy"),
                                    it->second.get<float>("speeds.va"),
                                    std::max(it->second.get<float>("kick"), it->second.get<float>("chip")),
                                    it->second.get<float>("chip") > 0,
                                    it->second.get<float>("dribble") > 0
                                )
                            );
                        }
                        if (it->second.get<std::string>("type") == "goto")
                        {
                            actions_yellow.push_back(
                                std::make_shared<goto_action>(
                                    r,
                                    it->second.get<float>("target.x"),
                                    it->second.get<float>("target.y"),
                                    it->second.get<float>("target.angle")
                                )
                            );
                        }
                    }
                }
            }
        }
        catch (std::exception& e) 
        {
            //ERROR(e);
            for (auto action : actions_yellow)
            {
                auto r_id = action.get()->r.id;
                auto r_it = std::find_if(team_yellow.begin(), team_yellow.end(), [&r_id](const robot& r) { return r.id == r_id; });
                if (r_it != team_yellow.end())
                {
                    auto r = *r_it;
                    action->update_robot(r);
                }
            }
            //std::cout << actions_yellow.size() << std::endl;
        } 

         try
         {
             auto blues = packet.get_child("control.actions_blue");
             if (blues.begin() != blues.end())
             {
                 actions_blue.clear();            
                 for (auto it = blues.begin(), endIt = blues.end(); it != endIt; ++it)
                 {
                     auto r_id = it->second.get<int>("uid");
                     auto r_it = std::find_if(team_blue.begin(), team_blue.end(), [&r_id](const robot& r) { return r.id == r_id; });
                     if (r_it != team_blue.end()) 
                     {
                         auto r = *r_it;
                         if (it->second.get<std::string>("type") == "move")
                         {
                             actions_blue.push_back(
                                 std::make_shared<move_action>(
                                     r,
                                     it->second.get<float>("speeds.vx"),
                                     it->second.get<float>("speeds.vy"),
                                     it->second.get<float>("speeds.va"),
                                     std::max(it->second.get<float>("kick"), it->second.get<float>("chip")),
                                     it->second.get<float>("chip") > 0,
                                     it->second.get<float>("dribble") > 0
                                 )
                             );
                         }
                         if (it->second.get<std::string>("type") == "goto")
                         {
                             actions_blue.push_back(
                                 std::make_shared<goto_action>(
                                     r,
                                     it->second.get<float>("target.x"),
                                     it->second.get<float>("target.y"),
                                     it->second.get<float>("target.angle")
                                 )
                             );
                         }
                     }
                 }
             }
         }
         catch (std::exception& e) 
         {
            for (auto action : actions_blue)
            {
                auto r_id = action.get()->r.id;
                auto r_it = std::find_if(team_blue.begin(), team_blue.end(), [&r_id](const robot& r) { return r.id == r_id; });
                if (r_it != team_blue.end())
                {
                    auto r = *r_it;
                    action->update_robot(r);
                }
            }
            //std::cout << actions_blue.size() << std::endl;
             
         }
    }
}

