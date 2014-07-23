#include <iostream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <thread>

#include "world.h"
#include "zmq_worker.h"
#include "usb_worker.h"

using namespace roboime;

int main(void)
{
    try
    {
        config conf;
        world w;

        zmq_worker worker(conf, 
            [&w] (boost::property_tree::ptree pt) { 
                std::stringstream st;
                boost::property_tree::json_parser::write_json(st, pt);
                std::cout << st.str() << std::endl;
                w.update(pt);
            }
        );
        std::cout << "Initializing tx" << std::endl;

        usb_worker tx(conf);

        std::cout << "RoboIME control module!" << std::endl;
        std::vector<char> buffer = { 0, 0, 0, 0, 0, 1, 2, 3, 4, 5 };
        while (1)
        {
            tx.send_raw_data(buffer.data(), buffer.size());
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        getchar(); 
    }
    catch (std::exception& e)
    {
        std::cerr << __FUNCTION__ << " [ERROR] " << e.what() << std::endl;
    }
}
