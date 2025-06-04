cmake_minimum_required(VERSION 3.14)
project(PlotAppDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Assume GLFW is installed on the system
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARIES})

# Fetch glad
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Fetch ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# Fetch ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG master
)
FetchContent_MakeAvailable(implot)

# Fetch NanoVG
FetchContent_Declare(
    nanovg
    GIT_REPOSITORY https://github.com/memononen/nanovg.git
    GIT_TAG master
)
FetchContent_MakeAvailable(nanovg)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${glad_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${implot_SOURCE_DIR}
    ${nanovg_SOURCE_DIR}/src
)

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${implot_SOURCE_DIR}/*.cpp
    ${nanovg_SOURCE_DIR}/src/nanovg.c
)

add_executable(PlotAppDemo ${SRC_FILES} src/main.cpp)

target_link_libraries(PlotAppDemo
    ${GLFW_LIBRARIES}
    glad
)

target_compile_definitions(PlotAppDemo PRIVATE NANOVG_GL3_IMPLEMENTATION)
