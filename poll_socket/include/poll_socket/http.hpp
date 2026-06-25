#ifndef POLL_SOCKET_HTTP_H
#define POLL_SOCKET_HTTP_H

/**
 * @brief classes to define http request/responses
 * that work with boost asio
 */
#include <map>

namespace poll_socket {

class HttpRequest {
    private:
        std::string method_;
        std::string uri_;
        std::map<std::string, std::string> headers_;
        std::string body_;
    public:
        void set_header(std::string key, std::string val) {
            headers_[key] = val;
        }
        void set_method(std::string method) {
            method_ = method;
        }
        void set_host(std::string host) {
            set_header("Host", host);
        }
        void set_uri(std::string uri) {
            uri_ = uri;
        }
        void set_body(std::string body) {
            body_ = body;
        }
        std::string to_string() {
            std::string req {};
            req.append(method_);
            req.append(" ");
            req.append(uri_);
            req.append(" ");
            req.append("HTTP/1.1");
            req.append("\r\n");

            for (const auto& [key, val] : headers_) {
                req.append(key);
                req.append(": ");
                req.append(val);
                req.append("\r\n");
            }

            req.append(body_);
            req.append("\r\n");
            return req;
        }
};



} // namespace poll_socket
#endif // POLL_SOCKET_HTTP_H
