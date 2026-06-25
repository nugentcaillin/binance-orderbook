#ifndef SOCKET_POLL_SOCKET_LISTENER_H
#define SOCKET_POLL_SOCKET_LISTENER_H

#include "ring_buffer.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


static constexpr size_t RECV_BUFF_SIZE { 65536 };
static constexpr size_t SEND_BUFF_SIZE { 1024 };


using resolver_result = boost::asio::ip::tcp::resolver::results_type;
using ssl_stream = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;


namespace poll_socket {

/**
 * @brief take tls records and write data into stream
 */
class SocketListener {
    private:
    RingBuffer<RECV_BUFF_SIZE> recv_buf_;
    RingBuffer<SEND_BUFF_SIZE> send_buf_;
    boost::asio::ssl::context ctx_;
    ssl_stream stream_;
    boost::asio::ip::tcp::resolver resolver_;
    std::string hostname_;
    bool exit_requested_;

    void handle_resolve(const boost::system::error_code& error, resolver_result results);
    void connect(resolver_result results, resolver_result::iterator curr, size_t attempt_idx);
    void handle_connect(const boost::system::error_code& error, resolver_result results, resolver_result::iterator curr, size_t attempt_idx);
    void handshake();
    void handle_handshake(const boost::system::error_code& error);
    void initiate_opening_handshake();
    void handle_opening_handshake_initiation(const boost::system::error_code& error);

    void request_exit();
    public:
    SocketListener(boost::asio::io_context& context);

    void listen(std::string hostname);
    bool done();

};

} // namespace poll_socket

#endif // SOCKET_POLL_SOCKET_LISTENER_H
