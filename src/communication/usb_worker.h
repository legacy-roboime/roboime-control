#ifndef __ROBOIME_USB_WORKER_H
#define __ROBOIME_USB_WORKER_H

#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <memory>

#include "world.h"
#include "config.h"

namespace roboime
{
    class packet
    {
        void* buf;
        size_t len;
    };

    class usb_worker
    {
        private:
            const config& conf;

            boost::asio::io_service service;
            boost::asio::io_service::work work;
            boost::thread worker;

            void* handle;
            void* get_device(uint16_t vendor, uint16_t product);

            void receive_loop(void);

        public:
            usb_worker(const config& conf);
            ~usb_worker();

            void async_process_action(std::vector<std::shared_ptr<action>> command);
            void send_raw_data(char* command, size_t count);
    };
}
#endif
