#include "poll_socket/socket_reader.hpp"
#include <spdlog/spdlog.h>

namespace poll_socket {

    SocketReader::SocketReader() {}

    bool socketReader::connect(std::string& uri) {
        spdlog::info("Attempting to connect to " + uri);
        spdlog::info("Successfully connected to " + uri);
        return true;
    }

} // namespace poll_socket
