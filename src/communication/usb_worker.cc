#include <libusb.h>
#include <memory>

#include "usb_worker.h"

constexpr unsigned int timeout = 200;

namespace roboime
{
    usb_worker::usb_worker(const config& conf) :
        conf(conf)
    {
        libusb_init(&context);
        // 5824/1500
        handle = libusb_open_device_with_vid_pid(context, 0x16c0, 0x05dc);
        std::cout << "Acquired transmitter USB handle." << std::endl;
    }

    usb_worker::~usb_worker()
    {
        libusb_close(handle);
        libusb_exit(context);
    }

    void
    usb_worker::async_process_action(const std::vector<std::shared_ptr<action>>& command)
    {
        unsigned char full_packet[46];
        memset(full_packet, 255, 47);
        full_packet[0] = 254;
        full_packet[1] = 0;
        full_packet[2] = 44;

        for (int i = 0, end_i = command.size(); i < end_i && i < 6; ++i)
        {
            auto raw_command = command[i].get();
            std::vector<char> buffer = raw_command->as_buffer();
            memcpy(&full_packet[0] + 3 + 7 * i, buffer.data(), 7);
        }
        full_packet[45] = 55;

        for (int i = 0; i < sizeof(full_packet) / sizeof(full_packet[0]); ++i)
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) full_packet[i] << " ";
        std::cout << std::endl;
        // 5696
        libusb_control_transfer(handle, 0x40, 3, 0, 0, full_packet, 46, timeout);
    }

    void
    usb_worker::send_raw_data(unsigned char* command, size_t size)
    {
        libusb_control_transfer(handle, 0x40, 3, 0, 0, command, size, timeout);
    }
}
