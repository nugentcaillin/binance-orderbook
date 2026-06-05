#include <vector>
#include <stdexcept>

#include "poll_socket/ring_buffer.hpp"

namespace poll_socket {

template <size_t N>
bool RingBuffer<N>::is_write_wrapped() {
    return write_pos_ < read_pos_;
}

template <size_t N>
RingBuffer<N>::RingBuffer() : buf_{}, read_pos_{}, write_pos_{} {}

template <size_t N>
size_t RingBuffer<N>::get_space() {
    return is_write_wrapped() ? read_pos_ - write_pos_ - 1
                              : N - write_pos_ - (read_pos_ == 0 ? 1 : 0);
}

template <size_t N>
size_t RingBuffer<N>::get_unread() {
    return is_write_wrapped() ? N - read_pos_ : write_pos_ - read_pos_;
}

template <size_t N>
char *RingBuffer<N>::get_write_pos() {
    return buf_.data() + write_pos_;
}

template <size_t N>
char *RingBuffer<N>::get_read_pos() {
    return buf_.data() + read_pos_;
}

template <size_t N>
void RingBuffer<N>::advance_write_pos(size_t amount) {
    if (amount >= N)
        throw std::out_of_range("amount must be less than buf size");
    bool wrappedWrite = is_write_wrapped();
    write_pos_ += amount;
    if (write_pos_ >= N) {
        write_pos_ %= N;
        wrappedWrite = true;
    }
    // if we wrapped to be >= read function and wrapped bool will disagree
    if (wrappedWrite && !is_write_wrapped())
        throw std::out_of_range(
            "Wrapped write attempted to become >= read position");
    return;
}

template <size_t N>
void RingBuffer<N>::advance_read_pos(size_t amount) {
    if (amount >= N)
        throw std::out_of_range("amount must be less than buf size");
    bool wrappedWrite{is_write_wrapped()};
    read_pos_ += amount;
    if (read_pos_ >= N) {
        if (!wrappedWrite)
            throw std::out_of_range("read attempted to pass write");
        wrappedWrite = false;
        read_pos_ %= N;
    }
    // if we wrapped past write function and wrapped bool will disagree
    if (!wrappedWrite && is_write_wrapped())
        throw std::out_of_range("read attempted to pass write");
    return;
}

template <size_t N>
void RingBuffer<N>::copy_bytes(std::vector<char> &dest, size_t offset,
                               size_t amount) {
    return;
}

}  // namespace poll_socket
