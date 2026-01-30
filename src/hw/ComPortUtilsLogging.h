// Сюда прилетает все что нужно отследить
#include "src/utils/log_core.h"

namespace win_api_port {
    using namespace logger_internal;

    inline bool trace(const std::string& stat) noexcept {
        dispatch("PORT", "TRACE", stat, Priority::P_TRACE);
        return true;
    }
    inline bool debug(const std::string& inf) noexcept {
        dispatch("PORT", "DEBUG", inf, Priority::P_DEBUG);
        return true;
    }
    inline bool warning(const std::string& warn) noexcept {
        dispatch("PORT", "WARN", warn, Priority::P_WARN);
        return true;
    }
    inline bool error(const std::string& err) noexcept {
        dispatch("PORT", "ERROR", err, Priority::P_ERROR);
        return true;
    }
    inline void critical(const std::string& crit) noexcept {
        flush_to_disk(); // При критической ошибке сбрасываем кеш немедленно
        dispatch("PORT", "CRITICAL", crit, Priority::P_CRIT);
        flush_to_disk(); // При критической ошибке сбрасываем кеш немедленно и второй раз тоже
    }
} // namespace win_api_port