#include <vector>
#include "strategy.hpp"
class null_strategy : public csot::Strategy {
public: 
    std::vector<csot::Order> on_tick(const csot::Tick& t) override{
        return {};
    }
};
