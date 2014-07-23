#ifndef __ROBOIME_ZMQ_WORKER_H
#define __ROBOIME_ZMQ_WORKER_H

#include <zmq.hpp>
#include <boost/thread.hpp>

#include "config.h"

namespace roboime
{
    class zmq_worker
    {
        private:
            const config& conf;
            std::function<void(boost::property_tree::ptree /* json_string */)> process_incoming;
            boost::thread worker;
            zmq::context_t ctx;
            zmq::socket_t recv_sock;

            void receive_loop(void);

        public:
            zmq_worker(
                const config& conf,
                std::function<void(boost::property_tree::ptree /* json_string */)> process_incoming);
            ~zmq_worker();
    };
}
#endif
