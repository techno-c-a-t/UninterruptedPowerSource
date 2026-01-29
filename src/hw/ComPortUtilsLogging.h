// Сюда прилетает все что нужно отследить
namespace win_api_port {
    bool trace(cosnt std::string& stat) {
        return 1;
    }
    bool debug(const std::string& inf) {
        return 1;
    }
    bool warning(const std::string& warn) {
        return 1;
    }
    bool error (const std::string& err) {
        return 1;
    }
    void critical(const std::string& crit) {
        return;
    }

} // namespace win_api_port