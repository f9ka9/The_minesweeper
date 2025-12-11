cmake_minimum_required(VERSION 3.20)

project(Seminar12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

set(FLTK_ROOT "D:\\FLTK")
find_package(FLTK REQUIRED)

set(TARGET "The_minesweeper")

add_executable(
    ${TARGET}
    src/main.cpp
    src/Board.cpp
    src/MainWindow.cpp
    src/Graph_lib/Graph.cpp
    src/Graph_lib/GUI.cpp
    src/Graph_lib/Window.cpp
)

target_include_directories(${TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${FLTK_INCLUDE_DIR}
)

target_link_directories(${TARGET} PRIVATE
    ${FLTK_INCLUDE_DIR}/../lib
)

target_link_libraries(${TARGET} PRIVATE
    ${FLTK_LIBRARIES}
    fltk_jpeg
    fltk_png
    fltk_z
    gdiplus
)

add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/resources
    COMMENT "Copying resources into runtime output directory"
)

add_dependencies(${TARGET} CopyResources)