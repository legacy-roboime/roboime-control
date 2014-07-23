#include <usb.h>
#include <boost/thread/once.hpp>
#include <memory>

#include "usb_worker.h"

namespace roboime
{
    static void initialize_usb_once()
    {
        static boost::once_flag flag = BOOST_ONCE_INIT;
        boost::call_once(flag, &usb_init);
    }

    void* usb_worker::get_device(uint16_t id_vendor, uint16_t id_product)
    {
    	usb_bus* bus_list, *bus;
    	usb_find_busses();
    	usb_find_devices();
 
    	bus_list = usb_get_busses();

        for (bus = bus_list; bus != NULL; bus = bus->next)
        {
            struct usb_device *dev;
            for (dev = bus->devices; dev; dev = dev->next) 
            {
                if (dev->descriptor.idVendor == id_vendor && dev->descriptor.idProduct == id_product) 
                {
                    auto* handle = usb_open(dev);
                    int status = usb_claim_interface(handle, 1);
                    if (!status) 
                    {
                        throw std::runtime_error("Error acquiring usb interface!");
                    }
                    return handle;
                }
            }
        }
        throw std::runtime_error("Could not find suitable usb interface with vendor " + 
                                 std::to_string(id_vendor) + 
                                 " and product " + 
                                 std::to_string(id_product) + ".");
    }

    usb_worker::usb_worker(const config& conf) :
        conf(conf),
        work(service),
        worker([this](){ this->service.run(); })
    {
        initialize_usb_once();
        handle = get_device(5824, 1500);
        std::cout << "Acquired transmitter handle " << std::endl;
        // control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout
    }

    usb_worker::~usb_worker()
    {
        usb_reset(static_cast<usb_dev_handle*>(handle));
    }

    void usb_worker::async_process_action(std::vector<std::shared_ptr<action>> command)
    {
        service.post(
            [this, &command] () {
                char full_packet[47];
                memset(full_packet, 255, 47); 
                full_packet[0] = 254;
                full_packet[1] = 0;
                full_packet[2] = 44;

                for (int i = 0, end_i = command.size(); i < end_i; ++i)
                {
                    auto raw_command = command[i].get();
                    std::vector<char> buffer = raw_command->as_buffer();
                    memcpy(&full_packet[0] + 3 + 7 * i, buffer.data(), 7);
                }
                full_packet[46] = 55;

                auto internal_handle = static_cast<usb_dev_handle*>(handle); 
                usb_control_msg(internal_handle, 5696, 3, 0, 0, full_packet, 47, 200);
            }
        );
    }

    void usb_worker::send_raw_data(char* command, size_t count)
    {
        auto internal_handle = static_cast<usb_dev_handle*>(handle); 
        usb_control_msg(internal_handle, 5696, 3, 0, 0, command, count, 200);
    }
}
