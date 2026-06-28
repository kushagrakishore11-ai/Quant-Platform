// ============================================================================
//  aggregator.stub.cpp — STARTING POINT for the Week-3 parallel-aggregator
//  challenge.
//
//  Copy this to `aggregator.cpp`, then make it FAST:
//      cp samples/aggregator.stub.cpp aggregator.cpp
//      cmake -B build -DCSOT_AGG_SRC=aggregator.cpp && cmake --build build -j
//      ./build/agg_runner data/tiny.ticks       # compare to data/tiny.agg.json
//
//  Unlike the Week-2 stub, this one is CORRECT: it is the AGG_SPEC.md §6
//  clarity reference, single-threaded. It passes data/tiny.ticks out of the
//  box. That is on purpose — Week 3's challenge is NOT the (trivial, fully
//  specified) reduction; it is making that reduction scream across four cores
//  without tripping over your own threads.
//
//  YOUR JOB: keep it correct and deterministic, and make run() fast by:
//    1. partitioning the stream across std::threads        (02-stdthread-basics.md)
//    2. giving each thread its OWN partial table on its OWN
//       cache line  (alignas(64) — no false sharing)        (04-false-sharing.md)
//    3. pinning the threads to distinct cores                (05-scheduler-and-pinning.md)
//    4. merging the partials once at the end (AGG_SPEC.md §7)
//    5. allocating every buffer in on_init() — zero heap in run()
//
//  A correct single-threaded entry (this file, unchanged) is a VALID upload;
//  it just ranks near the bottom. The board is won on speed.
//
//  Everything must live in this ONE translation unit. The judge builds exactly
//  this file against its own main(); no extra .cpp, no custom CMake. Threads,
//  <atomic>, and sched_setaffinity ARE allowed this week (they were not in
//  Week 2) — see TROUBLESHOOTING.md.
// ============================================================================

#include "aggregate.hpp"
#include <vector>
#include <thread>
#include <pthread.h>     // pthread_self
#include <sched.h>       // cpu_set_t, CPU_ZERO, CPU_SET, sched_setaffinity

namespace {

class StubAggregator final : public csot::Aggregator {
    std::uint32_t num_symbols_ = 0;

private: 

   // Pin the calling thread to a single logical core. Call at the top of each worker.
    inline void pin_to_core(int core) {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(core, &set);
        sched_setaffinity(0, sizeof(set), &set);   // 0 = the calling thread
    }   

    std::vector<csot::SymbolAgg> partials[4]; // right now these are NOT cache aligned
    void reduce_chunk(const csot::AggTick* lo,const csot::AggTick* hi,const uint8_t k){
        pin_to_core(k);
        #pragma GCC unroll 8;
        for (long int i = 0; i < (hi-lo); ++i) {
            __builtin_prefetch(&lo[i + 80]);
            const csot::AggTick& t = lo[i];
            csot::SymbolAgg&     r = partials[k][t.symbol_id];
            if (r.count == 0) [[unlikely]] {
                r.min_price = t.price;
                r.max_price = t.price;
            } else [[likely]]{
                if (t.price < r.min_price) r.min_price = t.price;
                if (t.price > r.max_price) r.max_price = t.price;
            }
            r.count += 1;
            r.sum_price += t.price; 
            r.sum_qty += t.qty;
        }
    }

public:
    void on_init(std::uint32_t num_symbols) override {
        // COLD PATH: allocate everything you will ever need here. For the fast
        // version that means your per-thread partial tables (one padded table
        // per thread) and any thread bookkeeping — never inside run().
        num_symbols_ = num_symbols;
        
        // touching partials on the COLD PATH: 
        #pragma GCC unroll 4;
        for(auto &partial : partials){
            partial.resize(num_symbols_);
            for(std::uint32_t j=0;j<num_symbols_;++j){
                partial[j] = csot::SymbolAgg{0, 0, 0, 0, 0};
            }
        }

    }

    void run(const csot::AggTick* ticks, std::size_t n,
             csot::SymbolAgg* out) override {
        // Start from canonical empty rows (AGG_SPEC.md §3) so symbols that
        // never appear end up as {0,0,0,0,0}.
        for (std::uint32_t s = 0; s < num_symbols_; ++s) {
            __builtin_prefetch(&out[s + 8]);
            out[s] = csot::SymbolAgg{0, 0, 0, 0, 0};
        }
        

        // MULTI-THREADED
        // MAP
        std::size_t lo = 0;   // multiply BEFORE divide
        std::size_t hi = n / 4;
        std::thread t1([=,this]{ reduce_chunk(ticks + lo, ticks + hi, 0); });
        lo = n / 4;
        hi = (n * 2) / 4;   
        std::thread t2([=,this]{ reduce_chunk(ticks + lo, ticks + hi, 1); }); 
        lo = (n * 2) / 4;
        hi = (n * 3) / 4;
        std::thread t3([=,this]{ reduce_chunk(ticks + lo, ticks + hi, 2); }); 
        lo = (n * 3) / 4;
        hi = n;
        std::thread t4([=,this]{ reduce_chunk(ticks + lo, ticks + hi, 3); }); 

        t1.join();
        t2.join();
        t3.join();
        t4.join();


        // REDUCE
        #pragma GCC unroll 4;
        for(const auto &partial : partials){
            // each partial is a vector of SymbolAggs
            // partial[0] has contennts of symbol id - 0
            #pragma GCC unroll 8;
            for(std::uint32_t i=0;i<num_symbols_;++i){
                // i is the symbol id 
                csot::SymbolAgg& r = out[i]; // r is the SymbolAgg struct for 'i' symbol_id
                const csot::SymbolAgg& t = partial[i]; // t is the tick 


                if(r.count == 0) {
                    r.min_price = t.min_price;
                    r.max_price = t.max_price;
                } else {
                    if(t.min_price < r.min_price) r.min_price = t.min_price;
                    if(t.max_price > r.max_price) r.max_price = t.max_price;
                }

                r.count += t.count; // Increase the count for that symbol 
                r.sum_price += t.sum_price;
                r.sum_qty += t.sum_qty;

            }
        }

    };

};

}// namespace

extern "C" csot::Aggregator* create_aggregator() {
    return new StubAggregator();
}