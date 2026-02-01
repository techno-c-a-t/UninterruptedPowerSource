#include "src/utils/log_core.h"

namespace pso_logic {
    using namespace logger_internal;

    inline bool trace(const std::string& stat) noexcept {
        dispatch("PSO", "TRACE", stat, Priority::P_TRACE);
        return true;
    }
    inline bool debug(const std::string& inf) noexcept {
        dispatch("PSO", "DEBUG", inf, Priority::P_DEBUG);
        return true;
    }
    inline bool warning(const std::string& warn) noexcept {
        dispatch("PSO", "WARN", warn, Priority::P_WARN);
        return true;
    }
    inline bool error(const std::string& err) noexcept {
        dispatch("PSO", "ERROR", err, Priority::P_ERROR);
        return true;
    }
    inline void critical(const std::string& crit) noexcept {
        dispatch("PSO", "CRITICAL", crit, Priority::P_CRIT);
        flush_to_disk();
    }
}