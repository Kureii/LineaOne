cmake_minimum_required(VERSION 3.26)

project(
        LineaOne
        VERSION 0.0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include(cmake/headers.cmake)
include(cmake/lib.cmake)
include(cmake/source.cmake)

add_executable(${PROJECT_NAME} ${exe_sources})
include_directories(${PROJECT_NAME} headers)

