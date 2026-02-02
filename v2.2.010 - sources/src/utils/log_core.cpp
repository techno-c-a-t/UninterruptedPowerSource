#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "log_core.h"

namespace logger_internal {
    // Функция для получения пути к лог-файлу рядом с .exe
    std::string get_log_path() {
#if IS_BUILDER
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string s_path = path;
        return s_path.substr(0, s_path.find_last_of("\\/")) + "\\session_log.txt";
#endif
        return "log.txt";
    }

    void flush_to_disk() {
#if IS_BUILDER
        if (log_cache.empty()) return;
        std::ofstream f(get_log_path(), std::ios::app);
        for (const auto& line : log_cache) f << line << "\n";
        f.close();
        log_cache.clear();
#endif
    }

    void dispatch(const std::string& ns, const std::string& level_name, const std::string& msg, Priority priority) {
        std::string full_msg = "[" + ns + "][" + level_name + "] " + msg;

#if IS_BUILDER
        // Логика для Билдера: всё что ниже Error (0, 1, 2) — в кеш и файл
        if (static_cast<Priority>(priority) >= Priority::P_ERROR) {
            log_cache.push_back(full_msg);
            if (log_cache.size() >= CACHE_MAX_SIZE) flush_to_disk();
        } 
        // Error и Critical тоже пишем, но сразу (не кешируем для надежности)
        else {
            std::ofstream f(get_log_path(), std::ios::app);
            f << "[IMMEDIATE]" << full_msg << std::endl;
        }
        // В Билдере всегда дублируем в Debug Window (внизу среды)
        OutputDebugStringA((full_msg + "\n").c_str());
#else
        // Логика для VS Code / Консоли
        if (static_cast<Priority>(priority) >= Priority::P_ERROR) {
            std::cerr << full_msg << std::endl;
        } else {
            std::cout << full_msg << std::endl;
        }
#endif
    }
}