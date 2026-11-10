cmake_minimum_required(VERSION 3.16)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
    M_PI=3.14159265358979323846
    M_2_PI=0.636619772367581343076
)

FetchContent_Declare(
    apriltag
    GIT_REPOSITORY https://github.com/AprilRobotics/apriltag.git
    GIT_TAG        v3.4.5
    GIT_SHALLOW 1
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_PYTHON_WRAPPER  OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING  OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(apriltag)

target_compile_options(apriltag PRIVATE
    -Wno-pedantic
)