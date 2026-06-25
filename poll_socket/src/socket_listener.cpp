#include "poll_socket/socket_listener.hpp"
#include "poll_socket/http.hpp"

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <spdlog/spdlog.h>


namespace poll_socket {

SocketListener::SocketListener(boost::asio::io_context& context)
    : recv_buf_ {},
      send_buf_ {},
      ctx_(boost::asio::ssl::context::sslv23),
      stream_(context, ctx_),
      resolver_(context),
      hostname_ {},
      exit_requested_ { false } {}

void SocketListener::listen(std::string hostname) {
    hostname_ = hostname;
    resolver_.async_resolve(hostname, "https",
            [this](const boost::system::error_code& error,
            resolver_result results) {
                handle_resolve(error, results);
            });
}


void SocketListener::handle_resolve(const boost::system::error_code& error, resolver_result results) {
    spdlog::info("received " + std::to_string(results.size()) + " records for " + hostname_);
    if (error) {
        spdlog::error("error resolving " + hostname_ + ": " + error.what());
        request_exit();
        return;
    }
    connect(results, results.begin(), 1);


    return;
}





// TODO: verify that expected behaviour happens when all records
// fail to connect
void SocketListener::connect(resolver_result results, resolver_result::iterator curr, size_t attempt_idx) {
    if (curr == results.end()) {
        spdlog::error("no more records to attempt connection for " + hostname_);
        request_exit();
        return;

    }
    spdlog::info("Attempting to connect to record " + std::to_string(attempt_idx) + " of " + std::to_string(results.size()) + " for " + hostname_);
    stream_.lowest_layer().async_connect(*curr,
        [this, results, curr, attempt_idx](const boost::system::error_code& error) {
            handle_connect(error, results, curr, attempt_idx);
        });
}


void SocketListener::handle_connect(const boost::system::error_code& error, resolver_result results, resolver_result::iterator curr, size_t attempt_idx) {
    if (error) {
        spdlog::warn("Failed to connect to record " + std::to_string(attempt_idx) + " of " + std::to_string(results.size()) + " for " + hostname_);
        connect(results, ++curr, ++attempt_idx);
        return;
    }
    spdlog::info("Successfully connected to " + hostname_);
    handshake();
}

void SocketListener::handshake() {
    stream_.async_handshake(ssl_stream::handshake_type::client,
        [this](const boost::system::error_code& error) {
            handle_handshake(error);
        });
}


void SocketListener::handle_handshake(const boost::system::error_code& error) {
    if (error) {
        spdlog::error("Failed SSL handshake for " + hostname_ + ": " + error.what());
        request_exit();
        return;
    }
    spdlog::info("Performed SSL handshake with " + hostname_);
    initiate_opening_handshake();
}

void SocketListener::request_exit() {
    exit_requested_ = true;
}

bool SocketListener::done() {
    return exit_requested_;
}

void SocketListener::initiate_opening_handshake() {
    HttpRequest req;
    req.set_method("GET");
    req.set_header("Connection", "close");
    req.set_host(hostname_);
    req.set_uri("/foobar");

    std::string req_str = req.to_string();
    spdlog::info("sending handshake of length " + std::to_string(req_str.size()));

    size_t copied = send_buf_.fill_from(req_str);

    if (copied != req_str.size()) {
        spdlog::error("Not enough space to write opening handshake to buffer");
        request_exit();
        return;
    }


    spdlog::info("Sent websocket opening handshake to " + hostname_);
    request_exit();
}


}  // namespace poll_socket
