# cmake/glad.cmake

include(FetchContent)

# Declare the glad dependency
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.34
)

# Ensure glad is downloaded and added to the build
FetchContent_MakeAvailable(glad)
set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
