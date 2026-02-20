#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <chrono>
#include <cstdint>
#include <sstream>
#include <time.h>
#include <vector>
#include <string>
#include <iomanip>

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

inline std::vector<int> TimeNowVector() {
     // 1. Получаем текущую точку времени
    auto now = std::chrono::system_clock::now();

    // 2. Получаем время в миллисекундах с начала эпохи
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    // 3. Переводим в time_t для работы с часами/минутами (локальное время)
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    // 4. Записываем в отдельные int переменные
    int hours   = now_tm->tm_hour;   // Часы (0-23)
    int minutes = now_tm->tm_min;    // Минуты (0-59)
    int seconds = now_tm->tm_sec;    // Секунды (0-60)
    int msec    = static_cast<int>(millis % 1000); // Оставшиеся миллисекунды (0-999)

    // Теперь эти переменные можно использовать в программе
    return std::vector<int> {hours, minutes, seconds, msec};
}

inline std::string TimeNowString (std::string delimiter = ":") {
    const auto t_now = TimeNowVector();
    std::stringstream ss;
    for (size_t i = 0; i + 2 < t_now.size(); i++) {
            ss << std::setfill('0') << std::setw(2) << t_now.at(i) << delimiter;
    }
    ss << std::setfill('0') << std::setw(2) << t_now.at(t_now.size() - 2);
    return ss.str();

}
#endif