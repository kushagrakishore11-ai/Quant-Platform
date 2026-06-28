// ============================================================================
//  cache_sim.cpp
// ============================================================================

#include "cache_sim.hpp"
#include <cstdint>

namespace {

constexpr uint64_t VALID_BIT = 0x8000000000000000ull;
constexpr uint64_t TAG_MASK  = 0x7FFFFFFFFFFFFFFFull;

class CacheSim final : public csot::CacheSim {
private:
    // Simple, flat, predictable Struct of Arrays.
    alignas(64) uint64_t l1_tags[64][8];
    alignas(64) uint8_t  l1_ages[64][8];
    alignas(64) uint8_t  l1_dirty[64][8];

    alignas(64) uint64_t l2_tags[512][8];
    alignas(64) uint8_t  l2_ages[512][8];
    alignas(64) uint8_t  l2_dirty[512][8];

    [[gnu::noinline]]
    void handle_l1_miss(uint64_t b, bool is_write, csot::CacheStats& s) {
        ++s.l1_misses;

        const uint64_t l1_set = b & 63;
        const uint64_t l2_set = b & 511;
        b >>= 6;
        const uint64_t l2_target = (b >> 3) | VALID_BIT;

        // --- 1. PROBE L2 ---
        int l2_hit_way = -1;
        int l2_vic_way = 0;

        #pragma GCC unroll 8
        for (int i = 0; i < 8; ++i)  {
            if (l2_tags[l2_set][i] == l2_target) l2_hit_way = i;
            if (l2_ages[l2_set][i] == 7) l2_vic_way = i;
        }

        if (l2_hit_way != -1) [[likely]] {
            ++s.l2_hits;
            const uint8_t hit_age = l2_ages[l2_set][l2_hit_way];
            #pragma GCC unroll 8
            for (int i = 0; i < 8; ++i) l2_ages[l2_set][i] += (l2_ages[l2_set][i] < hit_age);
            l2_ages[l2_set][l2_hit_way] = 0;
        } else [[unlikely]] {
            ++s.l2_misses;
            // Writeback if evicted L2 line was Valid AND Dirty
            s.dirty_writebacks += ((l2_tags[l2_set][l2_vic_way] & VALID_BIT) && l2_dirty[l2_set][l2_vic_way]);
            
            l2_tags[l2_set][l2_vic_way] = l2_target;
            l2_dirty[l2_set][l2_vic_way] = 0;
            
            #pragma GCC unroll 8
            for (int i = 0; i < 8; ++i) l2_ages[l2_set][i]++;
            l2_ages[l2_set][l2_vic_way] = 0;
        }

        // --- 2. PREPARE L1 EVICTION ---
        int l1_vic_way = 0;
        #pragma GCC unroll 8
        for (int i = 0; i < 8; ++i) {
            if (l1_ages[l1_set][i] == 7) l1_vic_way = i;
        }

        // If evicted L1 line is Valid AND Dirty, write it back to L2
        if ((l1_tags[l1_set][l1_vic_way] & VALID_BIT) && l1_dirty[l1_set][l1_vic_way]) [[unlikely]] {
            const uint64_t vic_b = ((l1_tags[l1_set][l1_vic_way] & TAG_MASK) << 6) | l1_set;
            const uint64_t vic_l2_set = vic_b & 511;
            const uint64_t vic_l2_target = (vic_b >> 9) | VALID_BIT;

            int wb_hit_way = -1;
            int wb_vic_way = 0;

            #pragma GCC unroll 8
            for (int i = 0; i < 8; ++i) {
                if (l2_tags[vic_l2_set][i] == vic_l2_target) wb_hit_way = i;
                if (l2_ages[vic_l2_set][i] == 7) wb_vic_way = i;
            }

            if (wb_hit_way != -1) {
                l2_dirty[vic_l2_set][wb_hit_way] = 1;
            } else {
                s.dirty_writebacks += ((l2_tags[vic_l2_set][wb_vic_way] & VALID_BIT) && l2_dirty[vic_l2_set][wb_vic_way]);
                l2_tags[vic_l2_set][wb_vic_way] = vic_l2_target;
                l2_dirty[vic_l2_set][wb_vic_way] = 1;
                
                #pragma GCC unroll 8
                for (int i = 0; i < 8; ++i) l2_ages[vic_l2_set][i]++;
                l2_ages[vic_l2_set][wb_vic_way] = 0;
            }
        }

        // --- 3. FINISH L1 FILL ---
        l1_tags[l1_set][l1_vic_way] = b| VALID_BIT;
        l1_dirty[l1_set][l1_vic_way] = is_write;
        
        #pragma GCC unroll 8
        for (int i = 0; i < 8; ++i) l1_ages[l1_set][i]++;
        l1_ages[l1_set][l1_vic_way] = 0;
    }

public:
    void on_init() override {
        for (uint32_t s = 0; s < 64; ++s) {
            for (uint32_t w = 0; w < 8; ++w) {
                l1_tags[s][w] = 0; // 0 has no VALID_BIT, naturally invalid
                l1_ages[s][w] = w;
                l1_dirty[s][w] = 0;
            }
        }
        for (uint32_t s = 0; s < 512; ++s) {
            for (uint32_t w = 0; w < 8; ++w) {
                l2_tags[s][w] = 0;
                l2_ages[s][w] = w;
                l2_dirty[s][w] = 0;
            }
        }
    }

    csot::CacheStats run(const csot::MemAccess* acc, std::size_t n) override {
        csot::CacheStats s{};

        for (std::size_t i = 0; i < n; ++i) {
            // THE HIGH-ROI TRICK: Tell the CPU to fetch future trace data from RAM
            __builtin_prefetch(&acc[i + 16], 0, 0); 

            const bool is_write = acc[i].is_write;
            s.writes += is_write;
            s.reads += !is_write;


            const uint64_t b = acc[i].address >> 6;
            const uint64_t l1_set = b & 63;
            const uint64_t target = (b >> 6) | VALID_BIT;

            int hit_way = -1;

            // Simple, predictable loop. Compiler handles the rest.
            #pragma GCC unroll 8
            for (int j = 0; j < 8; ++j) {
                if (l1_tags[l1_set][j] == target) {
                    hit_way = j;
                }
            }

            if (hit_way != -1) {
                ++s.l1_hits;
                const uint8_t hit_age = l1_ages[l1_set][hit_way];
                
                #pragma GCC unroll 8
                for (int j = 0; j < 8; ++j) {
                    l1_ages[l1_set][j] += (l1_ages[l1_set][j] < hit_age);
                }
                
                l1_ages[l1_set][hit_way] = 0;
                l1_dirty[l1_set][hit_way] |= is_write;
            } 
            else {
                handle_l1_miss(b, is_write, s);
            }
        }
        return s;
    }
};

}  // namespace

extern "C" csot::CacheSim* create_cache_sim() {
    return new CacheSim();
}
