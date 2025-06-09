include(FetchContent)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.34
)
FetchContent_MakeAvailable(glad)
# set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
