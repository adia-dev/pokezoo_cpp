cmake_minimum_required(VERSION 3.22)

project(pokezoo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include the settings shared across the different platforms available
include(cmake/StaticAnalyzers.cmake)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(
    GLOB_RECURSE
    SOURCES
    src/*.cpp
    src/**/*.cpp
)

add_executable(pokezoo ${SOURCES})

target_include_directories(pokezoo PUBLIC src ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(pokezoo PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} -lSDL2 -lSDL2_image -lSDL2_ttf)