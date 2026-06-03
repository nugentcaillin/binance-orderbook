#include <spdlog/spdlog.h>

#include <poll_socket/ring_buffer.hpp>

int main() {
    spdlog::info("Hello, World!");
    poll_socket::RingBuffer<1024> buf{};
    return 0;
}
