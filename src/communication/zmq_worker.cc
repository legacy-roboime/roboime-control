#include <boost/property_tree/json_parser.hpp>

#include "zmq_worker.h"

namespace roboime
{
    zmq_worker::zmq_worker(
            const config& conf,
            std::function<void(boost::property_tree::ptree /* json */)> process_incoming) :
        conf(conf),
        ctx(std::make_shared<zmq::context_t>(1)),
        recv_sock(*ctx, ZMQ_SUB),
        process_incoming(process_incoming),
        requested_termination(false)
    {
        worker = boost::thread(&zmq_worker::receive_loop, this);
    }

    zmq_worker::~zmq_worker()
    {
        requested_termination = true;
        if (recv_sock.connected())
            recv_sock.disconnect(conf.get<std::string>("zmq_subscriber_addr").c_str());
        recv_sock.close();
        ctx.reset();
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
            return;
        }
        while (!requested_termination)
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
                ERROR(e);
            }
        }
    }
}
