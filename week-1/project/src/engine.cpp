#include <charconv>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <string_view>
#include "strategy.hpp"
#include "histogram.hpp"
#include "fast_float/fast_float.h"
#include "null_strategy.cpp"
#include "strategy_spec.cpp"

std::deque<std::string> symbol;
std::unordered_map<std::string_view,std::string_view> symbol_map;

csot::Tick parse_tick(const std::string& _line){
    csot::Tick tick;

    // Time Stamp
    std::string_view line(_line);
    size_t pos = line.find(',');
    std::string_view timestamp_str = line.substr(0,pos);
    std::from_chars(timestamp_str.data(), timestamp_str.data() + timestamp_str.size(), tick.timestamp_ns);
    line.remove_prefix(pos + 1);

    // Symbol 
    pos = line.find(',');
    std::string_view temp_view = line.substr(0,pos);
    auto it = symbol_map.find(temp_view);
    if(it != symbol_map.end()){
        // Found 
        tick.symbol = it->second;
    }else{
        // Not Found 
        symbol.emplace_back(temp_view);
        std::string_view perm_view = symbol.back();
        symbol_map[perm_view] = perm_view;
        tick.symbol = perm_view;
    }
    line.remove_prefix(pos + 1);

    // Bid Price
    pos = line.find(',');
    std::string_view bid_px_view = line.substr(0,pos);
    fast_float::from_chars(bid_px_view.data(), bid_px_view.data() + bid_px_view.size(), tick.bid_px);
    line.remove_prefix(pos + 1);

    // Ask Price
    pos = line.find(',');
    std::string_view ask_px_view = line.substr(0,pos);
    fast_float::from_chars(ask_px_view.data(), ask_px_view.data() + ask_px_view.size(), tick.ask_px);
    line.remove_prefix(pos + 1);

    // Bid Quantity 
    pos = line.find(',');
    std::string_view bid_qty_view = line.substr(0,pos);
    std::from_chars(bid_qty_view.data(), bid_qty_view.data() + bid_qty_view.size(), tick.bid_qty);
    line.remove_prefix(pos + 1);

    // Ask Quantity 
    std::string_view ask_qty_view = line;
    std::from_chars(ask_qty_view.data(), ask_qty_view.data() + ask_qty_view.size(), tick.ask_qty);
    return tick;

}

std::vector<csot::Tick> load_ticks(std::string_view path){
    std::vector<csot::Tick> ticks;
    ticks.reserve(1'000'000);
    std::ifstream fin;

    std::string line;
    fin.open(std::string(path));

    std::getline(fin,line);
    while(std::getline(fin,line)){
        ticks.push_back(parse_tick(line));
    }
    return ticks;
}

void log_ticks(const std::vector<csot::Tick>& ticks){
    for(auto tick : ticks){
        std::cout << tick.timestamp_ns << " " << tick.symbol << " " << tick.bid_px << " " << tick.ask_px << " " << tick.bid_qty << " " << tick.ask_qty << '\n';
    }
}



int main(int argc, char *argv[]){
    std::string_view data_path {argv[1]};
    std::vector<csot::Tick> ticks = load_ticks(data_path);

    //log_ticks(ticks);
    strategy_spec strategy;

    csot::LatencyHistogram histogram;

    for(const auto& tick : ticks){
        auto start = std::chrono::steady_clock::now();
        std::vector<csot::Order> orders = strategy.on_tick(tick);
        auto end = std::chrono::steady_clock::now();
        auto latency = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        histogram.record(latency);
    }
    std::cout << histogram.count() << " ticks processed\n";
    histogram.print(std::cout);

}

