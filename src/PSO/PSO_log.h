#include "LoggerCore.h"

namespace ps_logic {
    using namespace logger_internal;

    inline bool trace(const std::string& stat) noexcept {
        dispatch("PSO", "TRACE", stat, P_TRACE);
        return true;
    }
    inline bool debug(const std::string& inf) noexcept {
        dispatch("PSO", "DEBUG", inf, P_DEBUG);
        return true;
    }
    inline bool warning(const std::string& warn) noexcept {
        dispatch("PSO", "WARN", warn, P_WARN);
        return true;
    }
    inline bool error(const std::string& err) noexcept {
        dispatch("PSO", "ERROR", err, P_ERROR);
        return true;
    }
    inline void critical(const std::string& crit) noexcept {
        dispatch("PSO", "CRITICAL", crit, P_CRIT);
        flush_to_disk();
    }
}