#include <libbinance/web/WebHandler.hpp>

#include <string>

namespace binance {

WebHandler::WebHandler() {
    
}

void WebHandler::monitor(std::string symbol) {
    get_initial_state();
}

void WebHandler::get_initial_state() {

}

} // namespace binance