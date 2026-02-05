#include "MemTable.h"
#include <vector>

extern std::vector<storage::CVMemPair> CVMemoryArray;
void __fastcall TForm1::memoryGridMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int col, row;
    // 1. Определяем, на какую ячейку нажали (по координатам X, Y)
    memoryGrid->MouseToCell(X, Y, col, row);

    // Игнорируем заголовок (строка 0)
    if (row <= 0) return;

    if (Button == mbLeft) {
        // ЛКМ — "Применить" (Depack из вектора в интерфейс и БП)
        ApplyPreset(row - 1);
    }
    else if (Button == mbRight) {
        // ПКМ — "Запомнить" (Pack из интерфейса в вектор и файл)
        SaveCurrentToPreset(row - 1);
    }
    else if (Button == mbMiddle) {
        // Средняя кнопка — например, очистить пресет
    }
}

// Константы для колонок (чтобы не запутаться)
const int COL_VOLTAGE = 2;
const int COL_CURRENT = 1;

/**
 * Переносим данные из Вектора в Таблицу (Визуализация)
 * @param data - ссылка на вектор структур
 * @param grid - указатель на таблицу
 * @param start - начальный индекс в векторе
 * @param end - конечный индекс (не включая его). Если -1, то до конца вектора.
 */
void VectorToGrid(const std::vector<storage::CVMemPair>& data, TStringGrid* grid, int start, int end) {
    if (!grid) {
    // ShowMessage("Empty grid. Exiting...");
    return;

    }

    // ShowMessage("Vector Size is ");
    // ShowMessage(data.size());
    // Если end не задан, идем до конца вектора
    int last = (end == -1) ? static_cast<int>(data.size()) : end;

    for (int i = start; i < last; ++i) {
        // Проверка границ вектора на всякий случай
        if (i >= data.size()) break;

        // Строка в гриде = индекс в векторе + 1 (пропускаем заголовок)
        int row = i + 1;
        grid->Cells[COL_VOLTAGE][row] = TForm1::formatValue_c(data[i].voltage, 4, 2);
        grid->Cells[COL_CURRENT][row] = TForm1::formatValue_c(data[i].current, 4, 2);

        // Можно еще добавить номер в колонку 0
        grid->Cells[0][row] = IntToStr(i + 1);
    }
}

/**
 * Переносим данные из Таблицы в Вектор (Чтение правок пользователя)
 */
void GridToVector(TStringGrid* grid, std::vector<storage::CVMemPair>& data, int start, int end) {
    if (!grid) return;

    int last = (end == -1) ? static_cast<int>(data.size()) : end;

    for (int i = start; i < last; ++i) {
        if (i >= data.size()) break;

        int row = i + 1;

        // StrToFloatDef(строка, значение_по_умолчанию)
        // Позволяет избежать краша, если пользователь ввел буквы
        data[i].voltage = StrToFloatDef(grid->Cells[COL_VOLTAGE][row], 0.0f);
        data[i].current = StrToFloatDef(grid->Cells[COL_CURRENT][row], 0.0f);
    }
}


void __fastcall TForm1::SaveCurrentToPreset(size_t index)
{
    // 1. Проверка на дурака (выход за границы вектора)
    if (index >= CVMemoryArray.size()) return;

    // --- ЛОГИКА ОБРАБОТКИ ЗАПЯТОЙ ---
    // Получаем текст из полей
    String sVolt = Edit9->Text;
    String sCurr = Edit8->Text;

    // Узнаем, какой разделитель сейчас в системе (обычно ',' в RU локали)
    Char separator = FormatSettings.DecimalSeparator;

    // Если системный разделитель запятая, меняем точки на запятые (и наоборот)
    // Это позволяет "схавать" и "12.5", и "12,5" без ошибок
    if (separator == ',') {
        sVolt = StringReplace(sVolt, ".", ",", TReplaceFlags() << rfReplaceAll);
        sCurr = StringReplace(sCurr, ".", ",", TReplaceFlags() << rfReplaceAll);
    } else {
        sVolt = StringReplace(sVolt, ",", ".", TReplaceFlags() << rfReplaceAll);
        sCurr = StringReplace(sCurr, ",", ".", TReplaceFlags() << rfReplaceAll);
    }

    // 2. Парсинг (конвертация)
    // StrToFloatDef вернет 0.0, если в поле полная ерунда, и программа не вылетит
    float newVoltage = StrToFloatDef(sVolt, 0.0f);
    float newCurrent = StrToFloatDef(sCurr, 0.0f);

    // 3. Обновление Модели (Вектора)
    CVMemoryArray[index].voltage = newVoltage;
    CVMemoryArray[index].current = newCurrent;

    // 4. Обновление Вида (Таблицы)
    // Используем твою функцию-мост, обновляем только одну строку
    // index - это индекс вектора. В таблице это строка index + 1 (из-за шапки)
    VectorToGrid(CVMemoryArray, memoryGrid, index, index + 1);
}
void __fastcall TForm1::ApplyPreset(const size_t index) {
    if (index > CVMemoryArray.size()) {
        return;
    }
    Edit8->Text = formatValue_c(CVMemoryArray[index].current, 4, 2);
    Edit9->Text = formatValue_c(CVMemoryArray[index].voltage, 4, 2);
    Edit8Exit(this);
    Edit9Exit(this);
}
