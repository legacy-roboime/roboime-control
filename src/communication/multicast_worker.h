#ifndef ROBOIME_MULTICAST_WORKER_H
#define ROBOIME_MULTICAST_WORKER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "world.h"
#include "config.h"


namespace roboime
{
    constexpr size_t MULTICAST_BUFFER_SIZE(2048);
    class multicast_worker
    {
        private:
            const config& conf;
            world& w;
            boost::asio::io_service sock_svc;
            boost::asio::ip::udp::socket sock;
            char data[MULTICAST_BUFFER_SIZE];
            boost::asio::ip::udp::endpoint sender_endpoint;
            boost::thread worker;

        public:
            multicast_worker(const config& conf, world& w);
            void process_packet(const boost::system::error_code& err,
                size_t num_bytes);
    };
}

#endif
