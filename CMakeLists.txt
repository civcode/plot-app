cmake_minimum_required(VERSION 3.14)
project(RenderModule VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Release)
add_compile_options(-fmax-errors=3)

# Assume GLFW is installed on the system
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
# include_directories(${GLFW_INCLUDE_DIRS})
# link_libraries(${GLFW_LIBRARIES})

include(FetchContent)

# Fetch glad
include(cmake/glad.cmake)
# install(TARGETS glad EXPORT RenderModuleTargets)

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

# Fetch Roboto font
FetchContent_Declare(
    roboto_font
    GIT_REPOSITORY https://github.com/googlefonts/roboto-2.git 
    GIT_TAG v2.138 
)
FetchContent_MakeAvailable(roboto_font)


FetchContent_Declare(
    roboto_mono_font
    GIT_REPOSITORY https://github.com/googlefonts/RobotoMono.git
    GIT_TAG v3.001
)
FetchContent_MakeAvailable(roboto_mono_font)

include_directories(
    # ${CMAKE_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIRS}
    # ${glad_SOURCE_DIR}/include
    # ${imgui_SOURCE_DIR}
    ${implot_SOURCE_DIR}
    # ${nanovg_SOURCE_DIR}/src
)

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/render_module.cpp
    ${CMAKE_SOURCE_DIR}/src/nvg_wrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/zoom_view.cpp
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${implot_SOURCE_DIR}/*.cpp
    ${nanovg_SOURCE_DIR}/src/nanovg.c
)




# add_executable(RenderModuleDemo ${SRC_FILES} src/main.cpp)
# target_include_directories(RenderModuleDemo PRIVATE
#     ${CMAKE_SOURCE_DIR}/include
#     ${GLFW_INCLUDE_DIRS}
#     ${glad_SOURCE_DIR}/include
#     ${imgui_SOURCE_DIR}
#     ${implot_SOURCE_DIR}
#     ${nanovg_SOURCE_DIR}/src
# )
# target_link_libraries(RenderModuleDemo glad)
# target_compile_definitions(RenderModuleDemo PRIVATE NANOVG_GL3_IMPLEMENTATION RENDER_MODULE_DEVELOPMENT_MODE)


# Copy assets to the build directory
file(GLOB_RECURSE ROBOTO_TTF_FILES
     "${roboto_font_SOURCE_DIR}/*.ttf"
     "${roboto_mono_font_SOURCE_DIR}/*.ttf"
)

# foreach(ROBOTO_FILE IN LISTS ROBOTO_TTF_FILES)
#     get_filename_component(FONT_NAME "${ROBOTO_FILE}" NAME)
    
#     add_custom_command(TARGET RenderModuleDemo POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                 "${ROBOTO_FILE}"
#                 "$<TARGET_FILE_DIR:RenderModuleDemo>/fonts/${FONT_NAME}"
#         COMMENT "Copying ${FONT_NAME} to output directory"
#     )
# endforeach()









configure_file(${nanovg_SOURCE_DIR}/src/nanovg.h
    ${CMAKE_CURRENT_BINARY_DIR}/render_module/nanovg.h COPYONLY
)

set(IMGUI_HEADERS
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    # ${imgui_SOURCE_DIR}/imstb_rectpack.h
    # ${imgui_SOURCE_DIR}/imstb_textedit.h
    # ${imgui_SOURCE_DIR}/imstb_truetype.h
)
# Copy each header to <build>/render_module/
foreach(header IN LISTS IMGUI_HEADERS)
    get_filename_component(filename "${header}" NAME)
    configure_file(${header} ${CMAKE_CURRENT_BINARY_DIR}/render_module/imgui/${filename} COPYONLY)
endforeach()


set(IMPLOT_HEADERS
    ${implot_SOURCE_DIR}/implot.h
    ${implot_SOURCE_DIR}/implot_internal.h
)
foreach(header IN LISTS IMPLOT_HEADERS)
    get_filename_component(filename "${header}" NAME)
    configure_file(${header} ${CMAKE_CURRENT_BINARY_DIR}/render_module/${filename} COPYONLY)
endforeach()

set(NANOVG_HEADERS
    ${nanovg_SOURCE_DIR}/src/nanovg.h
    # ${nanovg_SOURCE_DIR}/src/nanovg_gl.h
    # ${nanovg_SOURCE_DIR}/src/nanovg_gl_utils.h
    # ${nanovg_SOURCE_DIR}/src/stb_truetype.h
)
foreach(header IN LISTS NANOVG_HEADERS)
    get_filename_component(filename "${header}" NAME)
    configure_file(${header} ${CMAKE_CURRENT_BINARY_DIR}/render_module/${filename} COPYONLY)
endforeach()

# set(GLAD_HEADERS
#     ${glad_BINARY_DIR}/include/glad/glad.h
#     ${glad_BINARY_DIR}/include/KHR/khrplatform.h
# )
# foreach(header IN LISTS GLAD_HEADERS)
#     get_filename_component(filename "${header}" NAME)
#     configure_file(${header} ${CMAKE_CURRENT_BINARY_DIR}/render_module/${filename} COPYONLY)
# endforeach()










# === Add dummy target to Generate glad ===
add_custom_target(PrepareGlad ALL
    DEPENDS glad
    # COMMAND ${CMAKE_COMMAND} -E copy ${glad_SOURCE_DIR}/generated/src/glad.c ${CMAKE_CURRENT_BINARY_DIR}/glad.c
    # BYPRODUCTS ${glad_BINARY_DIR}/src/glad.c
)





# === Create the library ===
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
# add_library(RenderModule src/render_module.cpp)
# add_library(RenderModule src/render_module.cpp ${glad_BINARY_DIR}/src/glad.c)
add_library(RenderModule src/render_module.cpp)
add_dependencies(RenderModule PrepareGlad)
target_include_directories(RenderModule PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    # $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/render_module>
    $<INSTALL_INTERFACE:include>
    # $<BUILD_INTERFACE:${glad_BINARY_DIR}/include>
    PRIVATE ${glad_BINARY_DIR}/include
    PRIVATE ${imgui_SOURCE_DIR}
    # PRIVATE ${implot_SOURCE_DIR}
    PRIVATE ${nanovg_SOURCE_DIR}/src
)
target_sources(RenderModule PRIVATE ${SRC_FILES})
# target_sources(RenderModule PRIVATE ${SRC_FILES} ${glad_BINARY_DIR}/src/glad.c)
target_link_libraries(RenderModule
    PRIVATE ${GLFW_LIBRARIES}
    PRIVATE glad
)
# target_link_libraries(RenderModule PUBLIC glad)
# target_link_libraries(RenderModule PRIVATE glad)

target_compile_definitions(RenderModule 
    PUBLIC NANOVG_GL3_IMPLEMENTATION 
    # PRIVATE RENDERMODULE_INTERNAL_BUILD
)

# === Install the library ===
include(GNUInstallDirs)
# set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local")
install(TARGETS RenderModule
    EXPORT RenderModuleTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    # CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# === Copy Library Headers ===
#  Copy ImGui headers into the install tree under 'include/imgui'
install(FILES ${IMGUI_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
)

install(FILES ${IMPLOT_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
)


install(FILES ${NANOVG_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
)


# install(DIRECTORY ${imgui_SOURCE_DIR}/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui
#     FILES_MATCHING PATTERN "*.h"
# )

# Install the fonts to ~/.local/share/render-module/fonts
install(FILES ${ROBOTO_TTF_FILES} DESTINATION "$ENV{HOME}/.local/share/render-module/fonts/roboto")





# === Create the config files ===
install(EXPORT RenderModuleTargets
    FILE RenderModuleTargets.cmake
    NAMESPACE RenderModule::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RenderModule
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RenderModuleConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RenderModuleConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RenderModule
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RenderModuleConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RenderModule
)




