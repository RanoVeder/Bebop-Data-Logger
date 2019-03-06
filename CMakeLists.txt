cmake_minimum_required(VERSION 3.5)
project(Bebop-Data-Logger VERSION 0.1 LANGUAGES C CXX)



#Dependencies


if (UNIX)
link_libraries(pthread)
endif()

execute_process(COMMAND git submodule update --init --recursive vendor/spdlog WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(spdlog_INLCUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include)

if(NOT realsense2_FOUND)
message("realsense2 not found. Downloading...")
execute_process(COMMAND git submodule update --init --recursive vendor/librealsense WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


set(BUILD_GRAPHICAL_EXAMPLES CACHE BOOL FALSE)
set(BUILD_EXAMPLES CACHE BOOL FALSE)
set(BUILD_SHARED_LIBS CACHE BOOL FALSE)
set(BUILD_WITH_STATIC_CRT CACHE BOOL FALSE)
set(BUILD_WITH_TM2 CACHE BOOL FALSE)
set(ENABLE_CCACHE CACHE BOOL FALSE)

add_subdirectory(vendor/librealsense EXCLUDE_FROM_ALL)

set(realsense2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/librealsense/include)
set(realsense2_LIBRARY realsense2)

endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(utils)

add_executable(BebopDataLogger src/main.cpp src/Server.cpp src/Logger.cpp src/App.cpp src/DataLogger.cpp)
target_link_libraries(BebopDataLogger ${realsense2_LIBRARY})

include_directories(include ${realsense2_INCLUDE_DIR})
include_directories(include ${spdlog_INLCUDE_DIR})






