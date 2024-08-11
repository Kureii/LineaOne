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

add_compile_options(-g -Wall -Wformat)

if(UNIX AND NOT APPLE)
    message("Configuring for Linux")
    find_package(SDL3 REQUIRED)
    list(APPEND LIBS dl GL ${SDL3_LIBRARIES})
    include_directories(${SDL3_INCLUDE_DIRS})
elseif(APPLE)
    message("Configuring for Mac OS X")
    find_package(SDL3 REQUIRED)
    list(APPEND LIBS ${SDL3_LIBRARIES} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    include_directories(${SDL3_INCLUDE_DIRS} /usr/local/include /opt/local/include)
elseif(WIN32)
    message("Configuring for Windows")
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

target_link_libraries(${PROJECT_NAME} ${LIBS})

