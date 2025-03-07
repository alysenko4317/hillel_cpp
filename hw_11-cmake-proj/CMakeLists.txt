cmake_minimum_required(VERSION 3.10)

project(MyProject)

# Додаємо підпроекти (папки з CMakeLists.txt)
add_subdirectory(lib)
add_subdirectory(app)
