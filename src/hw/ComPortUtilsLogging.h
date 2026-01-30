// Сюда прилетает все что нужно отследить
namespace win_api_port {
    bool trace(const std::string& stat) noexcept {
        return 1;
    }
    bool debug(const std::string& inf) noexcept {
        return 1;
    }
    bool warning(const std::string& warn) noexcept {
        return 1;
    }
    bool error (const std::string& err) noexcept {
        return 1;
    }
    void critical(const std::string& crit) noexcept {
        return;
    }

} // namespace win_api_port