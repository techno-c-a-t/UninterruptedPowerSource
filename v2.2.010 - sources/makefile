# 1. Переменные компилятора и флагов
CXX      := g++
# -Wall: все предупреждения, -std=c++20: для std::format и прочего
# -O2: оптимизация, -I.: корень проекта в путях поиска include
CXXFLAGS := -Wall -std=c++20 -O0 -I.

# 2. Имя выходного файла
TARGET := pso_test.exe

# 3. Список всех исходников (.cpp файлов)
# Мы перечисляем их явно, чтобы Makefile понимал зависимости
SRCS := main_handtest.cpp \
        src/hw/ComPortUtils.cpp \
        src/PS/PowerSourceObject.cpp \
        src/core/engine.cpp \
        src/utils/log_core.cpp

# 4. Превращаем список .cpp в список .o (объектных файлов)
# $(patsubst %.cpp, %.o, ...) заменяет расширение у всех файлов в списке SRCS
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

# 5. Главная цель (по умолчанию выполняется первая)
all: $(TARGET)

# 6. Правило линковки: собираем .o файлы в один .exe
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CXX) $(OBJS) -o $(TARGET)
	@echo "Done!"

# 7. Шаблонное правило компиляции: как из .cpp получить .o
# % - это паттерн (любое имя), $< - это исходник (.cpp), $@ - это цель (.o)
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 8. Очистка проекта (удаление мусора)
.PHONY: clean
clean:
	@echo "Cleaning up..."
	del /Q /S *.o $(TARGET) 2>nul