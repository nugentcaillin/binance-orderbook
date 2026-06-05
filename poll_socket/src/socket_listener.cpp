#include "poll_socket/socket_listener.hpp"

namespace poll_socket {

SocketListener::SocketListener(boost::asio::io_context& context)
    : buf_ {}
    , ctx_(boost::asio::ssl::context::sslv23)
    , stream_(context, ctx_)
    , resolver_(context)
    {}

} // namespace poll_socket
