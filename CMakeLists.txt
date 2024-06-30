project(LiquidTracking)

cmake_minimum_required(VERSION 2.6)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

find_package(VISP REQUIRED)
if(VISP_FOUND)
 include(${VISP_USE_FILE})
endif(VISP_FOUND)

add_executable(LiquidTracking ${all_SRCS})
