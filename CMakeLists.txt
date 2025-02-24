cmake_minimum_required(VERSION 3.24)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Add platform-specific configurations
if(WIN32)
    message(STATUS "Configuring for Windows")
    # Windows-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /O2")
elseif(APPLE)
    message(STATUS "Configuring for macOS")
    # macOS-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPPLE -O2")
elseif(UNIX)
    message(STATUS "Configuring for Unix/Linux")
    # Unix/Linux-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIX -O2")
endif()

# Add SDL3 subdirectory
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

# Collect all source files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Link SDL3 library
add_executable(SnakeGame ${SOURCES})
target_link_libraries(SnakeGame PRIVATE SDL3::SDL3)