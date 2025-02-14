cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Устанавливаем общую директорию для исполняемых файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Добавляем подкаталоги
add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)

