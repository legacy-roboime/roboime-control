#include <multicast_worker.h>

namespace roboime
{
    multicast_worker::multicast_worker(const config& conf, world& w) :
        w(w),
        sock(sock_svc),
        conf(conf)
    {
        using namespace boost::asio;
        ip::udp::endpoint listen_endpoint(
            ip::address::from_string("0.0.0.0"),
            conf.get<size_t>("ssl_vision_port")
        );
        sock.open(listen_endpoint.protocol());
        sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        sock.bind(listen_endpoint);

        sock.set_option(ip::multicast::join_group(
            ip::address::from_string(conf.get<std::string>("ssl_vision_addr"))
        ));

        sock.async_receive_from(
            boost::asio::buffer(data, MULTICAST_BUFFER_SIZE),
            sender_endpoint,
            [this](const boost::system::error_code& err, size_t num_bytes) {
                process_packet(err, num_bytes);
            }
        );

        worker = boost::thread([this](){ sock_svc.run(); });
    }

    void multicast_worker::process_packet(const boost::system::error_code& err,
            size_t num_bytes)
    {
        if (!err)
        {
            std::cout.write(data, num_bytes);
            sock.async_receive_from(
                boost::asio::buffer(data, MULTICAST_BUFFER_SIZE),
                sender_endpoint,
                [this](const boost::system::error_code& err, size_t num_bytes) {
                    process_packet(err, num_bytes);
                }
            );
        }
        else
        {
            auto exc = std::runtime_error(err.message());
            ERROR(exc);
        }
    }
}
