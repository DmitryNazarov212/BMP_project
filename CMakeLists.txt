cmake_minimum_required(VERSION 3.28)
project("ProjectBMP" LANGUAGES CXX)

add_library(bmpInit src/BMP_file.cpp)

add_executable(mainExe src/main.cpp)

target_include_directories(bmpInit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mainExe bmpInit)