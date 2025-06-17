cmake_minimum_required(VERSION 3.16)
project(MinecraftRD LANGUAGES C CXX)
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add root include directory
include_directories(${CMAKE_SOURCE_DIR}/include)


# Add glad include directory
include_directories(${CMAKE_SOURCE_DIR}/glad/include)
set(GLAD_ROOT ${CMAKE_SOURCE_DIR}/glad)

# Add GLFW as a subdirectory
add_subdirectory(glfw)

# Add zlib as a subdirectory
add_subdirectory(zlib)

# Add src directory (your application code)
add_subdirectory(Source)
