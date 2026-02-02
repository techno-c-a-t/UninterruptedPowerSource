#include "cstddef"
#include <vector>
#include <string>
#ifndef LOG_CORE
#define LOG_CORE
namespace logger_internal {
        const size_t CACHE_MAX_SIZE = 10; 
}

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
    #include <windows.h>
    #define IS_BUILDER 1
#else
    #define IS_BUILDER 0
#endif

namespace logger_internal {
        // Приоритеты для фильтрации
    enum class Priority { P_TRACE = 0, P_DEBUG = 1, P_WARN = 2, P_ERROR = 3, P_CRIT = 4 };

    // Простой кеш для Билдера (чтобы не дергать диск на каждый чих)
    static std::vector<std::string> log_cache;
    void flush_to_disk();
    std::string get_log_path();

    /** @brief Функция форматирования лога, по сути, для единого вывода в одно место
    * @param ns - источник лога (Текст, условно, что это лог из порта)
    * @param level_name - по сути также просто строка, можно выводить какой уровень логгирования
    * @param msg - собственно сам лог
    * @param priority - вот это как раз таки настоящий приоритет, может быть:
    * 
    * { P_TRACE = 0, P_DEBUG = 1, P_WARN = 2, P_ERROR = 3, P_CRIT = 4 };
    * 
    * Ядро реализации: std::string full_msg = "[" + ns + "][" + level_name + "] " + msg;
    */
    void dispatch(const std::string& ns, const std::string& level_name, const std::string& msg, Priority priority);
}
#endif