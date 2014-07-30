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
                //std::stringstream ss;
                //boost::property_tree::json_parser::write_json(ss, pt);
                //std::cout << ss.str() << std::endl;
                w.update(pt);
            }
        );
        std::cout << "Initializing tx" << std::endl;

        usb_worker tx(conf);

        std::cout << "RoboIME control module!" << std::endl;

        const int send_sleep = conf.get<int>("send_sleep");

        while (1)
        {
            //boost::shared_lock<boost::shared_mutex> lock(w.access_lock);
            if (conf.get<bool>("control_blue"))
                tx.async_process_action(w.actions_blue);
            if (conf.get<bool>("control_yellow"))
                tx.async_process_action(w.actions_yellow);
            std::this_thread::sleep_for(std::chrono::milliseconds(send_sleep));
        }
        getchar();
    }
    catch (std::exception& e)
    {
        ERROR(e);
    }
}
