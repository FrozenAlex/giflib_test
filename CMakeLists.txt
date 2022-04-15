cmake_minimum_required(VERSION 3.0.0)
project(giflib_test VERSION 0.1.0)

include(CTest)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_library(GifLib libgif.so)


add_library(png src/png/TinyPngOut.cpp)

add_executable(giflib_test src/main.cpp)

target_link_libraries(giflib_test PUBLIC "${GifLib}")
target_link_libraries(giflib_test PUBLIC png)
target_include_directories(giflib_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


include(CPack)

