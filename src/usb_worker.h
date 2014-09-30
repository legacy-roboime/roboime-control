#ifndef __ROBOIME_USB_WORKER_H
#define __ROBOIME_USB_WORKER_H

#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <memory>

#include "world.h"
#include "config.h"

struct libusb_context;
struct libusb_device_handle;

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

            libusb_context* context;
            libusb_device_handle* handle;

            void receive_loop(void);

        public:
            usb_worker(const config& conf);
            ~usb_worker();

            void async_process_action(const std::vector<std::shared_ptr<action>>& command);
            void send_raw_data(unsigned char* command, size_t count);
    };
}
#endif

// vim: et sw=4 ts=4 sts=4
