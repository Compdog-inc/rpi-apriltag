cmake_minimum_required(VERSION 3.16)

include(FetchContent)
set(FETCHCONTENT_QUIET off)

include(cmake/fetch_external/opencv.cmake)
include(cmake/fetch_external/apriltag.cmake)