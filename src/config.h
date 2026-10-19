#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <cstddef>
#include <cstdint>

namespace config {

    // 1. Конфигурация главной формы и интерфейса
    namespace ui {
        // Базовые лимиты по умолчанию для PSEngine до загрузки Models.ini
        constexpr float DEFAULT_MAX_VOLTAGE = 300.0f;
        constexpr float DEFAULT_MAX_CURRENT = 1.0f;

        // Порог гистерезиса изменения значений регуляторов V и A
        constexpr float INPUT_HYSTERESIS_DELTA = 0.0001f;

        // Скейлинг ползунков (перевод в мВ / мА)
        constexpr float SCROLLBAR_SCALE = 1000.0f;
        constexpr float SCROLLBAR_THRESHOLD = 1000.0f;
        constexpr int SCROLLBAR_SMALL_STEP_LOW = 10;
        constexpr int SCROLLBAR_SMALL_STEP_HIGH = 100;

        // Настройки отображения и количества знаков
        constexpr int DIGITS_VOLTAGE_TOTAL = 4;
        constexpr int DIGITS_VOLTAGE_DECIMALS = 2;

        constexpr int DIGITS_CURRENT_TOTAL = 4;
        constexpr int DIGITS_CURRENT_DECIMALS = 3;

        constexpr int DIGITS_POWER_TOTAL = 4;
        constexpr int DIGITS_POWER_DECIMALS = 2;

        constexpr int DIGITS_ENERGY_CAPACITY_TOTAL = 5;
        constexpr int DIGITS_ENERGY_CAPACITY_DECIMALS = 3;

        // Имена конфигурационных файлов
        constexpr const char* FILE_MANUAL_PRESETS = "ManuallySavedData.ini";
        constexpr const char* FILE_AUTO_PRESETS = "AutomaticallySavedData.ini";
        constexpr const char* FILE_MODELS = "Models.ini";
    }

    // 2. Конфигурация таблицы пресетов памяти
    namespace presets {
        constexpr size_t PRESET_COUNT = 10;
        constexpr int COL_WIDTH_NUMBER = 40;
        constexpr int COL_WIDTH_VOLTAGE = 80;
        constexpr int COL_WIDTH_CURRENT = 80;
    }

    // 3. Конфигурация моделей БП по умолчанию (если нет ключей в Models.ini)
    namespace models {
        constexpr float DEFAULT_MIN_VOLTAGE = 0.0f;
        constexpr float DEFAULT_MIN_CURRENT = 0.0f;
        constexpr float DEFAULT_MAX_VOLTAGE = 5.0f;
        constexpr float DEFAULT_MAX_CURRENT = 0.1f;
        constexpr float DEFAULT_CURVE_CURRENT_POW = 1.0f;
        constexpr float DEFAULT_CURVE_VOLTAGE_POW = 1.0f;
    }

    // 4. Конфигурация рабочего цикла движка опроса
    namespace engine {
        constexpr double STEP_BASE_TRANSACTION_BYTES = 280.0;
        constexpr double STEP_HARDWARE_LAG_MS = 15.0;
        constexpr unsigned int STEP_MIN_DELAY_MS = 10;
        constexpr unsigned int STEP_MAX_DELAY_MS = 200;

        constexpr size_t ERROR_BUFFER_SIZE = 50;
        constexpr int STABLE_MODE_RETRIES = 3;
    }

    // 5. Конфигурация работы с прибором и протокола БП
    namespace power_source {
        // Аппаратные задержки при выключении выхода и отключении (Teardown)
        constexpr unsigned long SLEEP_BEFORE_UNLOCK_MS = 50;
        constexpr unsigned long SLEEP_AFTER_UNLOCK_RETRY_MS = 50;
        constexpr unsigned long SLEEP_BEFORE_DISCONNECT_MS = 150;

        // Лимиты команд протокола
        constexpr float MAX_PROTOCOL_VOLTAGE = 999.998f;
        constexpr float MAX_PROTOCOL_CURRENT = 999.998f;

        // Буферы ответов протокола
        constexpr size_t READ_PACKET_SIZE = 14;
        constexpr size_t MIN_VALID_RESPONSE_LEN = 9;
    }

    // 6. Конфигурация WinAPI COM-порта
    namespace com_port {
        constexpr uint32_t TIMEOUT_READ_INTERVAL = 15;
        constexpr uint32_t TIMEOUT_READ_MULTIPLIER = 2;
        constexpr uint32_t TIMEOUT_READ_CONSTANT = 70;
        constexpr uint32_t TIMEOUT_WRITE_MULTIPLIER = 2;
        constexpr uint32_t TIMEOUT_WRITE_CONSTANT = 100;
    }

    // 7. Конфигурация графиков и сессии
    namespace charts {
        constexpr size_t MAX_CHART_POINTS = 30;
        constexpr float AXIS_MARGIN_PERCENT = 0.10f; // 10% отступ осей Y
        constexpr float FLAT_GRAPH_EPSILON = 0.001f;
    }

    namespace session {
        constexpr float DEFAULT_DISCRET_INTERVAL_SEC = 1.0f;
    }

    // 8. Логгер приложения
    namespace logging {
        constexpr size_t CACHE_FLUSH_SIZE = 10;
    }

} // namespace config

#endif // APP_CONFIG_H
