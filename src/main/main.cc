#include <iostream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <world.h>

#include "zmq_worker.h"

using namespace roboime;

int main(void)
{
    try
    {
        config conf;
        world w;
        zmq_worker worker(conf, 
            [&w](boost::property_tree::ptree pt) { 
                std::stringstream st;
                boost::property_tree::json_parser::write_json(st, pt);
                std::cout << st.str() << std::endl;
                w.update(pt);
            }
        );
        std::cout << "RoboIME control module!" << std::endl;
        getchar(); 
    }
    catch (std::exception& e)
    {
        std::cerr << __FUNCTION__ << " [ERROR] " << e.what() << std::endl;
    }
}
