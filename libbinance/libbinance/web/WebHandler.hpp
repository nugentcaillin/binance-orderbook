#pragma once

#include <string>

namespace binance {

class WebHandler {
public:
    void monitor(std::string symbol);
private:
    void get_initial_state();
};

} // namespace binance