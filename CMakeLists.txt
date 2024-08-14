cmake_minimum_required(VERSION 3.29.6)

project(
        LineaOne
        VERSION 0.0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include(cmake/headers.cmake)
include(cmake/lib.cmake)
include(cmake/source.cmake)

message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/src/config.h
)

add_compile_options(-g -Wall -Wformat)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-O0 -g)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O3)
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    add_compile_options(-O2 -g)
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    add_compile_options(-Os)
endif()

if(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
    find_package(SDL3 REQUIRED)
    list(APPEND LIBS dl GL ${SDL3_LIBRARIES})
    include_directories(${SDL3_INCLUDE_DIRS})
elseif(APPLE)
    message(STATUS "Configuring for Mac OS X")
    find_package(SDL3 REQUIRED)
    list(APPEND LIBS ${SDL3_LIBRARIES} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    include_directories(${SDL3_INCLUDE_DIRS} /usr/local/include /opt/local/include)
elseif(WIN32)
    message(STATUS "Configuring for Windows")
    find_package(SDL3 REQUIRED)
    list(APPEND LIBS gdi32 opengl32 imm32 ${SDL3_LIBRARIES})
    include_directories(${SDL3_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME}
        ${exe_sources}
        ${imgui_sources}
)
include_directories(${PROJECT_NAME}
        headers
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} ${LIBS})

