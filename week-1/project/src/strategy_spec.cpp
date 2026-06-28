#include <vector>
#include "strategy.hpp"

struct SymbolState {
    double mid_sum {0};
    double mid_squared_sum {0};  
    uint32_t count {0};
    uint32_t head {0};     
    int32_t position {0};
    double mids[64];    
};

class strategy_spec : public csot::Strategy {
public: 
    __attribute__((always_inline)) std::vector<csot::Order> on_tick(const csot::Tick& t) override{

        auto& st = states[t.symbol[3] - '0'];
        const double ask_px = t.ask_px;
        const double bid_px = t.bid_px;

        const double mid = (bid_px + ask_px) * 0.5 ;
        
        // if count < 64 => only add , else add and subtract 
        if(st.count < 64) {
            ++st.count;
        }else {
            // Subtract
            const double curr_mid = st.mids[st.head];=
            st.mid_sum -= curr_mid;
            st.mid_squared_sum -= (curr_mid * curr_mid);
        }
        // Add
        st.mid_sum += mid;
        st.mid_squared_sum += (mid*mid);
        st.mids[st.head] = mid;

        // Increment Head
        st.head = (st.head + 1) & 63; 

        if(st.count < 64){
            return {};
        }

        const double mean = st.mid_sum * 0.015625; 
        const double variance = (st.mid_squared_sum * 0.015625) - (mean * mean);
        const double delta = mid - mean;
        const double delta_sq = delta * delta;

        if(variance < 1e-18){
            return {};
        }

        // Entry Logic
        if (st.position == 0) {
            if (delta_sq >= 4.0 * variance) {
                // Calculate the branchless parameters
                const bool is_sell = (delta >= 0.0); 
                const auto side  = is_sell ? csot::Order::Side::SELL : csot::Order::Side::BUY;
                const auto price = is_sell ? bid_px : ask_px;
                return {{side, t.symbol, price, 1}};
            }
            return {};
        }

        // Exit Logic
        if (st.position != 0 && delta_sq <= 0.25 * variance) {
            // We are definitely exiting. Now just check which way.
            if (st.position > 0) {
                return {{csot::Order::Side::SELL, t.symbol, bid_px, static_cast<uint32_t>(st.position)}};
            } else {
                return {{csot::Order::Side::BUY, t.symbol, ask_px, static_cast<uint32_t>(-st.position)}};
            }
        }
        return {};
    }


    void on_fill(const csot::Order& order, double price, uint32_t qty) override {
        auto& st = states[order.symbol[3] - '0'];
        
        if (order.side == csot::Order::Side::BUY) {
            st.position += qty;
        } else {
            st.position -= qty;
        }
    }
private: 
    alignas(64) SymbolState states[4];
};

extern "C" csot::Strategy* create_strategy() {
    return new strategy_spec();
}