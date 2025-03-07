# Створюємо виконуваний файл з файлу main.cpp
add_executable(myapp main.cpp)

# Лінкуємо додаток із створеною статичною бібліотекою
target_link_libraries(myapp PRIVATE mylib)
