cmake_minimum_required(VERSION 3.16)
project(rpiapriltag LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(_OpenCV_REQUIRED_COMPONENTS core imgproc videoio highgui)
find_package(OpenCV COMPONENTS ${_OpenCV_REQUIRED_COMPONENTS} CONFIG REQUIRED)

find_package(apriltag CONFIG REQUIRED)

add_executable(rpiapriltag ${SOURCES})

target_link_libraries(rpiapriltag apriltag ${OpenCV_LIBRARIES})

target_include_directories(rpiapriltag
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(rpiapriltag
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)