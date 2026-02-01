#include <chrono>
#include <cstdint>

class Timer {
    using clock_t = std::chrono::steady_clock;
    using micros_t = std::chrono::microseconds;

public:
    struct timerData {
        uint64_t start;
        uint64_t now;
        uint64_t duration;
    };

    void start() {
        start_point = clock_t::now();
        is_running = true;
    }

    uint64_t check() const {
        if (!is_running) return 0;
        return std::chrono::duration_cast<micros_t>(clock_t::now() - start_point).count();
    }

    timerData checkS() const {
        if (!is_running) return {0, 0, 0};
        const auto now_tp = clock_t::now();
        return {
            to_us(start_point),
            to_us(now_tp),
            static_cast<uint64_t>(std::chrono::duration_cast<micros_t>(now_tp - start_point).count())
        };
    }

    uint64_t stop() {
        if (!is_running) return 0;
        uint64_t result = check();
        is_running = false;
        return result;
    }

private:
    clock_t::time_point start_point;
    bool is_running = false;

    uint64_t to_us(clock_t::time_point tp) const {
        return std::chrono::duration_cast<micros_t>(tp.time_since_epoch()).count();
    }

};
