#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "log_core.h" // Убедись, что имя файла верное (LoggerCore.h или log_core.h)
#ifndef IS_BUILDER
    #define IS_BUILDER 0
#endif

namespace logger_internal {

    // Кэш и настройки
    std::vector<std::string> log_cache;

    std::string get_log_path() {
        // Для консольного теста файл создается в той же папке, откуда запускаешь exe
        return "log.txt"; 
    }

    void flush_to_disk() {
        if (log_cache.empty()) return;
        // Используем ios::app (дозапись)
        std::ofstream f(get_log_path(), std::ios::app);
        if (f.is_open()) {
            for (const auto& line : log_cache) f << line << "\n";
            f.close();
        }
        log_cache.clear();
    }

    void dispatch(const std::string& ns, const std::string& level_name, const std::string& msg, Priority priority) {
        std::string full_msg = "[" + ns + "][" + level_name + "] " + msg;

        // --- Ветка для КОНСОЛЬНОГО ТЕСТА (Makefile) ---
#if !IS_BUILDER
        // Пишем и в консоль, и в файл (чтобы наверняка)
        if (priority >= Priority::P_ERROR) {
            std::cerr << full_msg << std::endl;
        } else {
            std::cout << full_msg << std::endl;
        }
        
        // Пишем в файл мгновенно (для тестов кэш только мешает)
        std::ofstream f(get_log_path(), std::ios::app);
        if (f.is_open()) {
            f << full_msg << std::endl;
        }

        // --- Ветка для C++ BUILDER (GUI) ---
#else
        // 1. Критические ошибки и просто Ошибки - ПИШЕМ СРАЗУ
        // (Исправили логику: важные вещи не должны висеть в памяти)
        if (priority >= Priority::P_ERROR) {
            // Сначала сбросим то, что накопилось в кэше
            flush_to_disk();
            
            // Теперь запишем ошибку
            std::ofstream f(get_log_path(), std::ios::app);
            if (f.is_open()) {
                f << "[IMMEDIATE] " << full_msg << std::endl;
            }
        } 
        // 2. Дебаг, Трейс, Варнинги - В КЭШ
        else {
            log_cache.push_back(full_msg);
            if (log_cache.size() >= CACHE_MAX_SIZE) {
                flush_to_disk();
            }
        }
        
        // В Билдере дублируем в окно отладки (внизу IDE)
        OutputDebugStringA((full_msg + "\n").c_str());
#endif
    }
}