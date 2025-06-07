cmake_minimum_required(VERSION 3.14)
project(RenderModuleDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


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

FetchContent_Declare(
    roboto_font
    GIT_REPOSITORY https://github.com/googlefonts/roboto-2.git 
    GIT_TAG main
)
FetchContent_MakeAvailable(roboto_font)

message(STATUS "Using Roboto font from: ${roboto_font_SOURCE_DIR}")

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

add_executable(RenderModuleDemo ${SRC_FILES} src/main.cpp)

target_link_libraries(RenderModuleDemo
    ${GLFW_LIBRARIES}
    glad
)

target_compile_definitions(RenderModuleDemo PRIVATE NANOVG_GL3_IMPLEMENTATION)


# Copy assets to the build directory
# Find all PNG files in this directory (recursively if needed)
file(GLOB_RECURSE ROBOTO_FILES
     "${roboto_font_SOURCE_DIR}/*.ttf"
)

foreach(ROBOTO_FILE IN LISTS ROBOTO_FILES)
    get_filename_component(FONT_NAME "${ROBOTO_FILE}" NAME)
    
    add_custom_command(TARGET RenderModuleDemo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${ROBOTO_FILE}"
                "$<TARGET_FILE_DIR:RenderModuleDemo>/fonts/${FONT_NAME}"
        COMMENT "Copying ${FONT_NAME} to output directory"
    )
endforeach()