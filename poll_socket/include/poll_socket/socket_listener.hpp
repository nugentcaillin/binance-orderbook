#ifndef SOCKET_POLL_SOCKET_LISTENER_H
#define SOCKET_POLL_SOCKET_LISTENER_H

#include "ring_buffer.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


static constexpr size_t RECV_BUFF_SIZE { 65536 };



namespace poll_socket {

/**
 * @brief take tls records and write data into stream
 */
class SocketListener {
    private:
    RingBuffer<RECV_BUFF_SIZE> buf_;
    boost::asio::ssl::context ctx_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream_;
    boost::asio::ip::tcp::resolver resolver_;

    public:
    SocketListener(boost::asio::io_context& context);

};

} // namespace poll_socket

#endif // SOCKET_POLL_SOCKET_LISTENER_H
