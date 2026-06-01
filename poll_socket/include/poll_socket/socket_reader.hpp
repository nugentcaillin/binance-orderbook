#ifndef POLL_SOCKET_CONNECTION_H
#define POLL_SOCKET_CONNECTION_H

#include <vector>

namespace poll_socket {

/**
 * @brief circular buffer that functions
 * like a SPSC queue
 *
 * @warning not thread safe
 *
 */
template
<size_t buff_size>
class RingBuffer {
private:
    std::vector<char> buf_;
    size_t read_pos_;
    size_t write_pos_;
public:
    /**
     * @returns number of bytes that can be written without filling buffer or
     * wrapping round ring
     *
     * @note equals the amount that can be written in one recv syscall
     *
     */
    size_t get_space();

    /**
     * @returns number of bytes that can be read without wrapping round ring
     *
     * @note equals the amount that can be read by one send() syscall
     */
    size_t get_unread();


    /**
     * @returns naked pointer to write position
     *
     * @warn intended to be handed off to recv syscall and
     * should not be used under any other circumstances
     */
    char *get_write_pos();

    /**
     * @returns naked pointer to read position
     *
     * @warn intended to be handed off to read syscall
     * and should not be used under any other circumstances
     */
    char *get_read_pos();

    /**
     * @brief advances write position and wraps round buffer
     * @param amount the amount to move write pos forward
     */
    void advance_write_pos(size_t amount);

    /**
     * @brief advances read position and wraps round buffer
     * @param amount the amount to move read pos forward
     */
    void advance_read_pos(size_t amount);

    /**
     * @brief copy bytes from buffer to dest
     *
     * @param dest the container to write bytes to
     * @param offset the location in container to write bytes to
     * @param amount the amount of bytes to read
     *
     * @warning throws exception if amount is greater than unread bytes
     */
    void copy_bytes(std::vector<char>& dest, size_t offset, size_t amount);
};


} // namespace poll_socket

#endif // POLL_SOCKET_CONNECTION_H
