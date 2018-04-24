cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(lp_hal INTERFACE)
add_library(lp::hal ALIAS lp_hal)

target_include_directories(lp_hal INTERFACE ${LIB_DIR}/include)

target_link_libraries(lp_hal
    INTERFACE lp::cc_lib
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
