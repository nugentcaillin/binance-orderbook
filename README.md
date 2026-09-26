# Binance Orderbook
A C++20 real time limit order book using level 2 market data from binance. Provides an order book interface with adapters to integrate with industry standard financial libraries.
# Component Diagram (C4)
```mermaid
C4Context
    title Binance Orderbook
    Container(binance_rest, "Binance REST API")
    Container(binance_ws, "Binance WS API")
    Container(limit_book, "External Limit book adapter implementation")
    Container_Boundary(b1, "Binance Order book") {
        Component(web_handler, "Web Handler", "Sets up initial state using REST API and gets deltas using Websocket")
        Component(update_handler, "Update handler", "Forwards update events onto order book adapters")
    }
    Rel(web_handler, binance_rest, "Gets Initial state")
    Rel(web_handler, binance_ws, "Gets deltas")
    Rel(update_handler, limit_book, "Recieves book updates")
```