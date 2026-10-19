#include "src/utils/log_core.h"

namespace engine_logic {
    using namespace logger_internal;

    inline bool trace(const std::string& stat) noexcept {
        dispatch("Engine", "TRACE", stat, Priority::P_TRACE);
        return true;
    }
    inline bool debug(const std::string& inf) noexcept {
        dispatch("Engine", "DEBUG", inf, Priority::P_DEBUG);
        return true;
    }
    inline bool warning(const std::string& warn) noexcept {
        dispatch("Engine", "WARN", warn, Priority::P_WARN);
        return true;
    }
    inline bool error(const std::string& err) noexcept {
        dispatch("Engine", "ERROR", err, Priority::P_ERROR);
        return true;
    }
    inline void critical(const std::string& crit) noexcept {
        dispatch("Engine", "CRITICAL", crit, Priority::P_CRIT);
        flush_to_disk();
    }
    
    struct debug_struct {
        bool operator()(const std::string& inf) {
            return debug(inf);
        }
    }     ;
}