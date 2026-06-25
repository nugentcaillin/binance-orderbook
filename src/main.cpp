#include <spdlog/spdlog.h>

#include <boost/asio.hpp>
#include <poll_socket/ring_buffer.hpp>
#include <poll_socket/socket_listener.hpp>

int main() {
    spdlog::info("Hello, World!");
    boost::asio::io_context context;
    poll_socket::SocketListener listener(context);
    spdlog::info("Created listener without crashing!");
    listener.listen("caillin.requestcatcher.com");
    while (!listener.done()) {
        context.run();
    }
    spdlog::info("Shutting down");
    return 0;
}
