
cmake_minimum_required(VERSION 3.20)
project(SharedMemoryApplication LANGUAGES CXX)

# Force standard compliance (replaces VS C++ Language Standard setting)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your individual sub-projects (folders where .vcxproj files sit)
add_subdirectory(Producer)
add_subdirectory(Consumer)