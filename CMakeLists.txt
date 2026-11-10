cmake_minimum_required(VERSION 3.16)
project(rpiapriltag LANGUAGES ASM C CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_SKIP_INSTALL_RULES ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
)

if(WINDOWS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

include(cmake/fetch_external_dependencies.cmake)

add_executable(rpiapriltag ${SOURCES})

target_link_libraries(rpiapriltag PRIVATE
    apriltag
    ${OPENCV_MODULES_BUILD}
)

target_include_directories(rpiapriltag PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_options(rpiapriltag
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)