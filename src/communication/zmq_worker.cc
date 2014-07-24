#include <boost/property_tree/json_parser.hpp>

#include "zmq_worker.h"

namespace roboime
{
    zmq_worker::zmq_worker(
            const config& conf,
            std::function<void(boost::property_tree::ptree /* json_string */)> process_incoming) :
        conf(conf),
        ctx(1),
        recv_sock(ctx, ZMQ_SUB),
        process_incoming(process_incoming)
    {
        worker = boost::thread(&zmq_worker::receive_loop, this);
    }

    zmq_worker::~zmq_worker()
    {
        if (recv_sock.connected())
            recv_sock.close();
        worker.join();
    }

    void
    zmq_worker::receive_loop()
    {
        try
        {
            recv_sock.setsockopt(ZMQ_IDENTITY, "roboime-control", 15);
            recv_sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);
            recv_sock.connect(conf.get<std::string>("zmq_subscriber_addr").c_str());
            std::cout << "Connected on socket " << conf.get<std::string>("zmq_subscriber_addr") << std::endl;
        }
        catch (std::exception& e)
        {
            ERROR(e);
        }
        for (;;)
        {
            try
            {
                zmq::message_t message;

                recv_sock.recv(&message);

                try
                {
                    std::string raw_json(static_cast<char*>(message.data()), message.size());
                    if (raw_json.size() <= 0) throw std::runtime_error("Unable to parse json message");
                    std::stringstream st;
                    st << raw_json;
                    boost::property_tree::ptree pt;
                    boost::property_tree::json_parser::read_json(st, pt);
                    process_incoming(pt);
                }
                catch (std::exception& e) 
                { 
                    ERROR(e);    
                }
            }
            catch (std::exception& e)
            {
                // I have no frickin idea how to recover from ZMQ errors. 
                // Close the socket, rebind and hope it goes away (while logging).
                recv_sock.close();
                recv_sock.connect(conf.get<std::string>("zmq_subscriber_addr").c_str());
                std::cout << __FUNCTION__ << " [ERROR] " << e.what() << std::endl;
            }
        }
    }
}
