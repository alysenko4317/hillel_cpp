# Створюємо статичну бібліотеку з файлів mylib.cpp та mylib.h
add_library(mylib STATIC mylib.cpp mylib.h)

# Вказуємо директорію з заголовочними файлами для користувачів бібліотеки
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
