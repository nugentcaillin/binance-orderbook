#include <vector>

#include "poll_socket/ring_buffer.hpp"

namespace poll_socket {

template <size_t N>
RingBuffer<N>::RingBuffer() : buf_{}, read_pos_{}, write_pos_{} {}

template <size_t N>
size_t RingBuffer<N>::get_space() {
    return 0;
}

template <size_t N>
size_t RingBuffer<N>::get_unread() {
    return 0;
}

template <size_t N>
char *RingBuffer<N>::get_write_pos() {
    return nullptr;
}

template <size_t N>
char *RingBuffer<N>::get_read_pos() {
    return nullptr;
}

template <size_t N>
void RingBuffer<N>::advance_write_pos(size_t amount) {
    return;
}

template <size_t N>
void RingBuffer<N>::advance_read_pos(size_t amount) {
    return;
}

template <size_t N>
void RingBuffer<N>::copy_bytes(std::vector<char> &dest, size_t offset,
                               size_t amount) {
    return;
}

}  // namespace poll_socket
